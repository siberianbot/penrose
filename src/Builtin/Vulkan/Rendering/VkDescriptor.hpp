#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_DESCRIPTOR_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_DESCRIPTOR_HPP

#include <array>

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/Descriptor.hpp>

#include "src/Builtin/Vulkan/Constants.hpp"

namespace Penrose {

    class Buffer;
    class Image;
    class Sampler;

    class VkDescriptorPoolManager;
    class VkLogicalDeviceContext;
    class VkPipeline;

    class VkDescriptor : public Descriptor {
    public:
        using DescriptorSets = std::array<vk::DescriptorSet, INFLIGHT_FRAME_COUNT>;

        VkDescriptor(VkDescriptorPoolManager *descriptorPoolManager,
                     VkLogicalDeviceContext *logicalDeviceContext,
                     DescriptorSets descriptorSets, VkPipeline *pipeline);
        ~VkDescriptor() override;

        void updateBindingValues(const std::unordered_set<DescriptorBindingValue> &values) override;

        [[nodiscard]] const std::unordered_set<DescriptorBindingValue> &getBindingValues() const override {
            return this->_bindingValues;
        }

        [[nodiscard]] const DescriptorSets &getDescriptorSets() const { return this->_descriptorSets; }

    private:
        VkDescriptorPoolManager *_descriptorPoolManager;
        VkLogicalDeviceContext *_logicalDeviceContext;

        DescriptorSets _descriptorSets;

        VkPipeline *_pipeline;
        std::unordered_set<DescriptorBindingValue> _bindingValues;

        [[nodiscard]] static std::vector<vk::DescriptorBufferInfo> *createUniformBufferData(
                const DescriptorBindingValue &value);
        [[nodiscard]] static std::vector<vk::DescriptorImageInfo> *createSamplerData(
                const DescriptorBindingValue &value);

        [[nodiscard]] static vk::DescriptorBufferInfo createBufferInfo(Buffer *buffer);
        [[nodiscard]] static vk::DescriptorImageInfo createImageInfo(Image *image, Sampler *sampler);
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_DESCRIPTOR_HPP
