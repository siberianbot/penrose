#include "VkUtils.hpp"

#include <Penrose/Common/EngineError.hpp>

#include "src/Rendering/DeviceContext.hpp"

namespace Penrose {

    vk::BufferUsageFlags toVkBufferUsageFlags(const BufferType &type) {
        switch (type) {
            case BufferType::Staging:
                return vk::BufferUsageFlagBits::eTransferSrc;

            case BufferType::Uniform:
                return vk::BufferUsageFlagBits::eUniformBuffer;

            case BufferType::Vertex:
                return vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer;

            case BufferType::Index:
                return vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer;

            default:
                throw EngineError("Buffer type is not supported");
        }
    }

    vk::ImageUsageFlags toVkImageUsageFlags(const RenderTargetType &type) {
        auto usage = vk::ImageUsageFlags();

        if (type & RenderTargetType::Input) {
            usage |= vk::ImageUsageFlagBits::eInputAttachment;
        }

        if (type & RenderTargetType::Color) {
            usage |= vk::ImageUsageFlagBits::eInputAttachment;
        }

        if (type & RenderTargetType::DepthStencil) {
            usage |= vk::ImageUsageFlagBits::eDepthStencilAttachment;
        }

        return usage;
    }

    vk::ImageAspectFlags toVkImageAspectFlags(const RenderTargetType &type) {
        auto aspect = vk::ImageAspectFlags();

        if (type & RenderTargetType::Color) {
            aspect |= vk::ImageAspectFlagBits::eColor;
        }

        if (type & RenderTargetType::DepthStencil) {
            aspect |= vk::ImageAspectFlagBits::eDepth;
        }

        return aspect;
    }

    std::optional<vk::Format> toVkFormat(const RenderFormat &format) {
        switch (format) {
            case RenderFormat::None:
                return std::nullopt;

            case RenderFormat::R8UNorm:
                return vk::Format::eR8Unorm;

            case RenderFormat::RGBA8UNorm:
                return vk::Format::eR8G8B8A8Unorm;

            case RenderFormat::RGBA16Float:
                return vk::Format::eR16G16B16A16Sfloat;

            case RenderFormat::D32Float:
                return vk::Format::eD32Sfloat;

            default:
                throw EngineError("Render target format is not supported");
        }
    }

    vk::AttachmentLoadOp toVkAttachmentLoadOp(const RenderAttachmentLoadOp &loadOp) {
        switch (loadOp) {
            case RenderAttachmentLoadOp::DontCare:
                return vk::AttachmentLoadOp::eDontCare;

            case RenderAttachmentLoadOp::Load:
                return vk::AttachmentLoadOp::eLoad;

            case RenderAttachmentLoadOp::Clear:
                return vk::AttachmentLoadOp::eClear;

            default:
                throw EngineError("Render attachment load op is not supported");
        }
    }

    vk::AttachmentStoreOp toVkAttachmentStoreOp(const RenderAttachmentStoreOp &storeOp) {
        switch (storeOp) {
            case RenderAttachmentStoreOp::DontCare:
                return vk::AttachmentStoreOp::eDontCare;

            case RenderAttachmentStoreOp::Store:
                return vk::AttachmentStoreOp::eStore;

            default:
                throw EngineError("Render attachment store op is not supported");
        }
    }

    vk::ImageLayout toVkImageLayout(const RenderAttachmentLayout &layout) {
        switch (layout) {
            case RenderAttachmentLayout::Undefined:
                return vk::ImageLayout::eUndefined;

            case RenderAttachmentLayout::ColorAttachment:
                return vk::ImageLayout::eColorAttachmentOptimal;

            case RenderAttachmentLayout::DepthStencilAttachment:
                return vk::ImageLayout::eDepthStencilAttachmentOptimal;

            case RenderAttachmentLayout::Present:
                return vk::ImageLayout::ePresentSrcKHR;

            default:
                throw EngineError("Render attachment layout is not supported");
        }
    }

    vk::DeviceMemory makeDeviceMemory(DeviceContext *deviceContext, const vk::MemoryRequirements requirements,
                                      bool local) {
        vk::MemoryPropertyFlags flags = local
                                        ? vk::MemoryPropertyFlagBits::eDeviceLocal
                                        : vk::MemoryPropertyFlagBits::eHostVisible |
                                          vk::MemoryPropertyFlagBits::eHostCoherent;

        auto properties = deviceContext->getPhysicalDevice().getMemoryProperties();
        std::optional<uint32_t> memTypeIdx;

        for (uint32_t idx = 0; idx < properties.memoryTypeCount; idx++) {
            bool typeMatches = requirements.memoryTypeBits & (1 << idx);
            bool propertiesMatches = (properties.memoryTypes[idx].propertyFlags & flags) == flags;

            if (!typeMatches || !propertiesMatches) {
                continue;
            }

            memTypeIdx = idx;
            break;
        }

        if (!memTypeIdx.has_value()) {
            throw EngineError("No memory type available for required allocation");
        }

        auto allocateInfo = vk::MemoryAllocateInfo()
                .setAllocationSize(requirements.size)
                .setMemoryTypeIndex(memTypeIdx.value());

        return deviceContext->getLogicalDevice().allocateMemory(allocateInfo);
    }

    vk::DeviceMemory makeDeviceMemory(DeviceContext *deviceContext, const vk::Buffer &buffer, bool local) {
        auto requirements = deviceContext->getLogicalDevice().getBufferMemoryRequirements(buffer);
        auto memory = makeDeviceMemory(deviceContext, requirements, local);

        deviceContext->getLogicalDevice().bindBufferMemory(buffer, memory, 0);

        return memory;
    }

    vk::DeviceMemory makeDeviceMemory(DeviceContext *deviceContext, const vk::Image &image, bool local) {
        auto requirements = deviceContext->getLogicalDevice().getImageMemoryRequirements(image);
        auto memory = makeDeviceMemory(deviceContext, requirements, local);

        deviceContext->getLogicalDevice().bindImageMemory(image, memory, 0);

        return memory;
    }
}
