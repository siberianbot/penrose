#include "VkPipelineFactory.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>

#include <Penrose/Assets/ShaderAsset.hpp>

#include "src/Builtin/Vulkan/Rendering/VkUtils.hpp"

namespace Penrose {

    inline static constexpr std::string_view TAG = "VkPipelineFactory";

    inline static constexpr std::string_view PIPELINE_CACHE_FILENAME = ".vk-pipeline-cache";

    VkPipelineFactory::VkPipelineFactory(const ResourceSet *resources)
        : _log(resources->get<Log>()),
          _assetManager(resources->get<AssetManager>()),
          _logicalDeviceProvider(resources->get<VkLogicalDeviceProvider>()) {
        //
    }

    void VkPipelineFactory::init() {
        auto createInfo = vk::PipelineCacheCreateInfo();

        std::vector<char> data;
        auto stream = std::ifstream(std::filesystem::path(PIPELINE_CACHE_FILENAME), std::ios::in | std::ios::binary);

        if (stream.is_open() && stream.good()) {
            std::copy(std::istream_iterator<char>(stream), std::istream_iterator<char>(), std::back_inserter(data));

            createInfo = createInfo.setPInitialData(data.data()).setInitialDataSize(data.size());
        }

        this->_cache = this->_logicalDeviceProvider->getLogicalDevice().handle->createPipelineCacheUnique(createInfo);
    }

    void VkPipelineFactory::destroy() {
        auto &device = this->_logicalDeviceProvider->getLogicalDevice().handle;

        const auto data = device->getPipelineCacheData(this->_cache.get());
        auto stream = std::ofstream(
            std::filesystem::path(PIPELINE_CACHE_FILENAME), std::ios::out | std::ios::binary | std::ios::trunc
        );

        if (stream.is_open() && stream.good()) {
            std::ranges::copy(std::as_const(data), std::ostream_iterator<char>(stream));
        } else {
            this->_log->writeWarning(TAG, "Failed to write pipeline cache, serialization skipped");
        }

        this->_cache.reset();
    }

    Pipeline *VkPipelineFactory::makePipeline(PipelineInfo &&pipelineInfo) {
        auto &device = this->_logicalDeviceProvider->getLogicalDevice().handle;

        auto descriptorBindings = std::vector<vk::DescriptorSetLayoutBinding>(pipelineInfo.objects.size());
        for (std::uint32_t idx = 0; idx < pipelineInfo.objects.size(); idx++) {
            const auto &object = pipelineInfo.objects.at(idx);

            descriptorBindings[idx] = vk::DescriptorSetLayoutBinding()
                                          .setBinding(idx)
                                          .setStageFlags(mapShaderStage(object.stage))
                                          .setDescriptorType(mapObjectType(object.type))
                                          .setDescriptorCount(object.count);
        }

        auto constants = std::vector<vk::PushConstantRange>(pipelineInfo.constants.size());
        std::ranges::transform(pipelineInfo.constants, constants.begin(), [](const PipelineConstant &constant) {
            return vk::PushConstantRange()
                .setStageFlags(mapShaderStage(constant.stage))
                .setOffset(constant.offset)
                .setSize(constant.size);
        });

        const auto descriptorSetLayoutCreateInfo = vk::DescriptorSetLayoutCreateInfo().setBindings(descriptorBindings);
        auto descriptorSetLayout = device->createDescriptorSetLayoutUnique(descriptorSetLayoutCreateInfo);

        const auto pipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo()
                                                  .setPushConstantRanges(constants)
                                                  .setSetLayouts(descriptorSetLayout.get());
        auto pipelineLayout = device->createPipelineLayoutUnique(pipelineLayoutCreateInfo);

        std::vector<vk::VertexInputBindingDescription> bindings;
        std::vector<vk::VertexInputAttributeDescription> attributes;
        std::uint32_t attributeIdx = 0;
        for (std::uint32_t bindingIdx = 0; bindingIdx < pipelineInfo.inputs.size(); bindingIdx++) {
            const auto &input = pipelineInfo.inputs.at(bindingIdx);

            bindings.emplace_back(bindingIdx, input.stride, mapInputRate(input.rate));

            for (const auto &attribute: input.attributes) {
                const auto format = mapAttributeFormat(attribute.format);

                switch (attribute.format) {
                    case PipelineInputAttributeFormat::UInt:
                    case PipelineInputAttributeFormat::Vec2:
                    case PipelineInputAttributeFormat::Vec3:
                        attributes.emplace_back(attributeIdx++, bindingIdx, format, attribute.offset);

                        break;

                    case PipelineInputAttributeFormat::Mat4:
                        {
                            constexpr std::uint32_t vecSize = 16;
                            constexpr std::uint32_t matSize = 4 * vecSize;

                            const auto baseOffset = attribute.offset;
                            const auto targetOffset = baseOffset + matSize;

                            for (std::uint32_t offset = baseOffset; offset < targetOffset; offset += vecSize) {
                                attributes.emplace_back(attributeIdx++, bindingIdx, format, offset);
                            }

                            break;
                        }

                    default:
                        throw EngineError("Binding attribute format is not supported");
                }
            }
        }

        return new VkPipeline(
            std::forward<decltype(pipelineInfo)>(pipelineInfo), std::move(bindings), std::move(attributes),
            vk::PipelineInputAssemblyStateCreateInfo({}, vk::PrimitiveTopology::eTriangleList, false),
            // TODO: tesselation
            vk::PipelineViewportStateCreateInfo({}, 1, nullptr, 1, nullptr),
            vk::PipelineRasterizationStateCreateInfo(
                {}, false, false, vk::PolygonMode::eFill, vk::CullModeFlagBits::eBack, vk::FrontFace::eCounterClockwise,
                false, 0, 0, 0, 1
            ),
            vk::PipelineMultisampleStateCreateInfo({}, vk::SampleCountFlagBits::e1, false, 0, nullptr, false, false),
            vk::PipelineDepthStencilStateCreateInfo(
                {}, true, true, vk::CompareOp::eLessOrEqual, false, false, {}, {}, 0, 0
            ),
            {
                vk::PipelineColorBlendAttachmentState(
                    false, vk::BlendFactor::eZero, vk::BlendFactor::eZero, vk::BlendOp::eAdd, vk::BlendFactor::eZero,
                    vk::BlendFactor::eZero, vk::BlendOp::eAdd,
                    vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB
                        | vk::ColorComponentFlagBits::eA
                ),
            },
            vk::PipelineColorBlendStateCreateInfo({}, false, vk::LogicOp::eCopy, 0, nullptr, {0, 0, 0, 0}),
            {
                vk::DynamicState::eViewport,
                vk::DynamicState::eScissor,
            },
            std::move(descriptorSetLayout), std::move(pipelineLayout)
        );
    }

    VkPipelineInstance *VkPipelineFactory::makePipelineInstance(
        const VkPipeline *pipeline, const vk::RenderPass pass, const std::uint32_t subpass
    ) {
        std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;

        for (const auto &pipelineShader: pipeline->getPipelineInfo().shaders) {
            const auto shader = this->_assetManager->getAsset<ShaderAsset>(pipelineShader.shaderAssetName);

            shaderStages.emplace_back(vk::PipelineShaderStageCreateInfo()
                                          .setStage(mapShaderStage(pipelineShader.stage))
                                          .setModule(asVkShader(shader->getShader().lock().get())->getHandle())
                                          .setPName("main"));
        }

        const auto vertexInputState = vk::PipelineVertexInputStateCreateInfo()
                                          .setVertexBindingDescriptions(pipeline->getVertexInputBindings())
                                          .setVertexAttributeDescriptions(pipeline->getVertexInputAttributes());

        const auto colorBlendState = vk::PipelineColorBlendStateCreateInfo(pipeline->getColorBlendState())
                                         .setAttachments(pipeline->getColorBlendAttachments());

        const auto dynamicState = vk::PipelineDynamicStateCreateInfo().setDynamicStates(pipeline->getDynamicStates());

        const auto createInfo = vk::GraphicsPipelineCreateInfo()
                                    .setRenderPass(pass)
                                    .setSubpass(subpass)
                                    .setLayout(pipeline->getPipelineLayoutHandle())
                                    .setStages(shaderStages)
                                    .setPVertexInputState(&vertexInputState)
                                    .setPInputAssemblyState(&pipeline->getInputAssemblyState())
                                    .setPViewportState(&pipeline->getViewportState())
                                    .setPRasterizationState(&pipeline->getRasterizationState())
                                    .setPMultisampleState(&pipeline->getMultisampleState())
                                    .setPDepthStencilState(&pipeline->getDepthStencilState())
                                    .setPColorBlendState(&colorBlendState)
                                    .setPDynamicState(&dynamicState);

        auto [result, handle] = this->_logicalDeviceProvider->getLogicalDevice().handle->createGraphicsPipelineUnique(
            this->_cache.get(), createInfo
        );

        if (result != vk::Result::eSuccess) {
            this->_log->writeWarning(
                TAG, "Compilation of pipeline {:#x} wasn't performed: disabled by engine",
                reinterpret_cast<std::size_t>(pipeline)
            );
        }

        return new VkPipelineInstance(pipeline, pass, subpass, std::move(handle));
    }
}
