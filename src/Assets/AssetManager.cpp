#include "AssetManager.hpp"

#include <vector>
#include <string>
#include <tuple>

#include <fmt/core.h>
#include <stb/stb_image.h>
#include <tiny_obj_loader.h>
#include <vulkan/vulkan.hpp>

#include <Penrose/Assets/AssetDictionary.hpp>
#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Common/Vertex.hpp"
#include "src/Rendering/DeviceContext.hpp"

namespace Penrose {

    AssetManager::AssetManager(ResourceSet *resources)
            : _assetDictionary(resources->get<AssetDictionary>()),
              _deviceContext(resources->get<DeviceContext>()) {
        //
    }

    ShaderAsset AssetManager::loadShader(const AssetId &asset) const {
        auto openedAsset = this->_assetDictionary->openAsset(asset);
        auto createInfo = vk::ShaderModuleCreateInfo()
                .setPCode(reinterpret_cast<const std::uint32_t *>(openedAsset.data()))
                .setCodeSize(openedAsset.size());

        try {
            return ShaderAsset{
                    .shaderModule = makeShaderModule(this->_deviceContext, createInfo)
            };
        } catch (...) {
            std::throw_with_nested(EngineError(fmt::format("Failed to create shader module from asset {}", asset)));
        }
    }

    MeshAsset AssetManager::loadMesh(const AssetId &asset) const {
        auto openedAsset = this->_assetDictionary->openAsset(asset);
        auto assetText = std::string(reinterpret_cast<const char *>(openedAsset.data()), openedAsset.size());

        auto reader = tinyobj::ObjReader();
        if (!reader.ParseFromString(assetText, "") || !reader.Valid()) {
            throw EngineError(fmt::format("Failed to read mesh from asset {}: {}", asset, reader.Error()));
        }

        // TODO: pass warning

        auto &attrib = reader.GetAttrib();
        auto &shapes = reader.GetShapes();
        std::vector<Vertex> vertices;
        std::vector<std::uint32_t> indices;

        for (const auto &shape: shapes) {
            for (const auto &index: shape.mesh.indices) {
                indices.push_back(vertices.size());

                auto vertex = Vertex{
                        .pos = {
                                attrib.vertices.at(3 * index.vertex_index + 0),
                                attrib.vertices.at(3 * index.vertex_index + 1),
                                attrib.vertices.at(3 * index.vertex_index + 2)
                        }
                };

                if (index.normal_index >= 0) {
                    vertex.normal = {
                            attrib.normals.at(3 * index.normal_index + 0),
                            attrib.normals.at(3 * index.normal_index + 1),
                            attrib.normals.at(3 * index.normal_index + 2)
                    };
                }

                if (index.texcoord_index >= 0) {
                    vertex.uv = {
                            attrib.texcoords.at(2 * index.texcoord_index + 0),
                            attrib.texcoords.at(2 * index.texcoord_index + 1)
                    };
                }

                if (!attrib.colors.empty()) {
                    vertex.color = {
                            attrib.colors.at(3 * index.vertex_index + 0),
                            attrib.colors.at(3 * index.vertex_index + 1),
                            attrib.colors.at(3 * index.vertex_index + 2)
                    };
                }

                vertices.push_back(vertex);
            }
        }

        auto makeStaging = [this](vk::DeviceSize size, void *data) {
            auto buffer = makeBuffer(this->_deviceContext, vk::BufferCreateInfo()
                    .setUsage(vk::BufferUsageFlagBits::eTransferSrc)
                    .setSize(size));
            auto memory = makeDeviceMemory(this->_deviceContext, buffer, false);

            void *bufferData;
            auto result = this->_deviceContext->getLogicalDevice().mapMemory(memory.getInstance(), 0, size,
                                                                             vk::MemoryMapFlags(), &bufferData);

            if (result != vk::Result::eSuccess) {
                throw EngineError("Failed to map buffer's memory");
            }

            std::memcpy(bufferData, data, size);
            this->_deviceContext->getLogicalDevice().unmapMemory(memory.getInstance());

            return std::make_tuple(std::move(buffer), std::move(memory));
        };

        auto makeTarget = [this](vk::DeviceSize size, vk::BufferUsageFlags additionalUsage) {
            auto buffer = makeBuffer(this->_deviceContext, vk::BufferCreateInfo()
                    .setUsage(vk::BufferUsageFlagBits::eTransferDst | additionalUsage)
                    .setSize(size));
            auto memory = makeDeviceMemory(this->_deviceContext, buffer);

            return std::make_tuple(std::move(buffer), std::move(memory));
        };

        auto vertexBufferSize = sizeof(Vertex) * vertices.size();
        auto indexBufferSize = sizeof(std::uint32_t) * indices.size();
        auto [stagingVertexBuffer, stagingVertexBufferMemory] = makeStaging(vertexBufferSize, vertices.data());
        auto [stagingIndexBuffer, stagingIndexBufferMemory] = makeStaging(indexBufferSize, indices.data());
        auto [targetVertexBuffer, targetVertexBufferMemory] = makeTarget(vertexBufferSize,
                                                                         vk::BufferUsageFlagBits::eVertexBuffer);
        auto [targetIndexBuffer, targetIndexBufferMemory] = makeTarget(vertexBufferSize,
                                                                       vk::BufferUsageFlagBits::eIndexBuffer);

        auto commandBufferAllocateInfo = vk::CommandBufferAllocateInfo()
                .setCommandBufferCount(1)
                .setCommandPool(this->_deviceContext->getCommandPool());
        auto commandBuffers = this->_deviceContext->getLogicalDevice()
                .allocateCommandBuffers(commandBufferAllocateInfo);

        auto beginInfo = vk::CommandBufferBeginInfo()
                .setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
        commandBuffers.at(0).begin(beginInfo);
        commandBuffers.at(0).copyBuffer(stagingVertexBuffer.getInstance(),
                                        targetVertexBuffer.getInstance(),
                                        vk::BufferCopy(0, 0, vertexBufferSize));
        commandBuffers.at(0).copyBuffer(stagingIndexBuffer.getInstance(),
                                        targetIndexBuffer.getInstance(),
                                        vk::BufferCopy(0, 0, indexBufferSize));
        commandBuffers.at(0).end();

        this->_deviceContext->getGraphicsQueue().submit(vk::SubmitInfo().setCommandBuffers(commandBuffers));
        this->_deviceContext->getGraphicsQueue().waitIdle();

        this->_deviceContext->getLogicalDevice().free(this->_deviceContext->getCommandPool(), commandBuffers);

        return MeshAsset{
                .vertexBuffer = std::move(targetVertexBuffer),
                .vertexBufferMemory = std::move(targetVertexBufferMemory),
                .indexBuffer = std::move(targetIndexBuffer),
                .indexBufferMemory = std::move(targetIndexBufferMemory),
                .indexCount = static_cast<std::uint32_t>(indices.size())
        };
    }

    ImageAsset AssetManager::loadImage(const AssetId &asset) const {
        auto openedAsset = this->_assetDictionary->openAsset(asset);

        int width, height, channels;
        auto data = stbi_load_from_memory(reinterpret_cast<const stbi_uc *>(openedAsset.data()),
                                          static_cast<int>(openedAsset.size()),
                                          &width, &height, &channels, STBI_rgb_alpha);

        auto bufferSize = width * height * 4;
        auto bufferCreateInfo = vk::BufferCreateInfo()
                .setUsage(vk::BufferUsageFlagBits::eTransferSrc)
                .setSize(bufferSize);
        auto buffer = makeBuffer(this->_deviceContext, bufferCreateInfo);
        auto bufferMemory = makeDeviceMemory(this->_deviceContext, buffer, false);

        void *bufferData;
        auto result = this->_deviceContext->getLogicalDevice().mapMemory(bufferMemory.getInstance(), 0, bufferSize,
                                                                         vk::MemoryMapFlags(), &bufferData);

        if (result != vk::Result::eSuccess) {
            throw EngineError("Failed to map buffer's memory");
        }

        std::memcpy(bufferData, data, bufferSize);
        this->_deviceContext->getLogicalDevice().unmapMemory(bufferMemory.getInstance());

        stbi_image_free(data);

        auto imageCreateInfo = vk::ImageCreateInfo()
                .setUsage(vk::ImageUsageFlagBits::eTransferSrc |
                          vk::ImageUsageFlagBits::eTransferDst |
                          vk::ImageUsageFlagBits::eSampled)
                .setImageType(vk::ImageType::e2D)
                .setExtent(vk::Extent3D(width, height, 1))
                .setFormat(vk::Format::eR8G8B8A8Srgb)
                .setSamples(vk::SampleCountFlagBits::e1)
                .setArrayLayers(1)
                .setMipLevels(1)
                .setSharingMode(vk::SharingMode::eExclusive)
                .setTiling(vk::ImageTiling::eOptimal);
        auto image = makeImage(this->_deviceContext, imageCreateInfo);
        auto imageMemory = makeDeviceMemory(this->_deviceContext, image);

        auto range = vk::ImageSubresourceRange()
                .setAspectMask(vk::ImageAspectFlagBits::eColor)
                .setLayerCount(1)
                .setBaseArrayLayer(0)
                .setLevelCount(1)
                .setBaseMipLevel(0);

        auto imageViewCreateInfo = vk::ImageViewCreateInfo()
                .setViewType(vk::ImageViewType::e2D)
                .setImage(image.getInstance())
                .setFormat(vk::Format::eR8G8B8A8Srgb)
                .setSubresourceRange(range)
                .setComponents(vk::ComponentMapping());
        auto imageView = makeImageView(this->_deviceContext, imageViewCreateInfo);

        auto commandBufferAllocateInfo = vk::CommandBufferAllocateInfo()
                .setCommandBufferCount(1)
                .setCommandPool(this->_deviceContext->getCommandPool());
        auto commandBuffers = this->_deviceContext->getLogicalDevice()
                .allocateCommandBuffers(commandBufferAllocateInfo);

        auto beginInfo = vk::CommandBufferBeginInfo()
                .setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
        commandBuffers.at(0).begin(beginInfo);

        vk::ImageMemoryBarrier memoryBarrier;

        memoryBarrier = vk::ImageMemoryBarrier()
                .setImage(image.getInstance())
                .setSrcAccessMask(vk::AccessFlagBits::eNone)
                .setDstAccessMask(vk::AccessFlagBits::eTransferWrite)
                .setOldLayout(vk::ImageLayout::eUndefined)
                .setNewLayout(vk::ImageLayout::eTransferDstOptimal)
                .setSubresourceRange(range);
        commandBuffers.at(0).pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe,
                                             vk::PipelineStageFlagBits::eTransfer,
                                             vk::DependencyFlagBits(0),
                                             {}, {}, memoryBarrier);

        auto region = vk::BufferImageCopy()
                .setBufferOffset(0)
                .setBufferRowLength(width)
                .setBufferImageHeight(height)
                .setImageExtent(vk::Extent3D(width, height, 1))
                .setImageOffset(vk::Offset3D(0, 0, 0))
                .setImageSubresource(vk::ImageSubresourceLayers()
                                             .setLayerCount(1)
                                             .setBaseArrayLayer(0)
                                             .setMipLevel(0)
                                             .setAspectMask(vk::ImageAspectFlagBits::eColor));
        commandBuffers.at(0).copyBufferToImage(buffer.getInstance(), image.getInstance(),
                                               vk::ImageLayout::eTransferDstOptimal, region);

        memoryBarrier = vk::ImageMemoryBarrier()
                .setImage(image.getInstance())
                .setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)
                .setDstAccessMask(vk::AccessFlagBits::eShaderRead)
                .setOldLayout(vk::ImageLayout::eTransferDstOptimal)
                .setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
                .setSubresourceRange(range);
        commandBuffers.at(0).pipelineBarrier(vk::PipelineStageFlagBits::eTransfer,
                                             vk::PipelineStageFlagBits::eFragmentShader,
                                             vk::DependencyFlagBits(0),
                                             {}, {}, memoryBarrier);

        commandBuffers.at(0).end();

        this->_deviceContext->getGraphicsQueue().submit(vk::SubmitInfo().setCommandBuffers(commandBuffers));
        this->_deviceContext->getGraphicsQueue().waitIdle();

        this->_deviceContext->getLogicalDevice().free(this->_deviceContext->getCommandPool(), commandBuffers);

        return ImageAsset{
                .image = std::move(image),
                .memory = std::move(imageMemory),
                .imageView = std::move(imageView)
        };
    }
}
