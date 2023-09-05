#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_DESCRIPTOR_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_DESCRIPTOR_HPP

#include <array>

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/Descriptor.hpp>

#include "src/Builtin/Vulkan/Constants.hpp"

namespace Penrose {

    class VkDescriptorPoolManager;

    class VkDescriptor : public Descriptor {
    public:
        using DescriptorSets = std::array<vk::DescriptorSet, INFLIGHT_FRAME_COUNT>;

        VkDescriptor(VkDescriptorPoolManager *descriptorPoolManager,
                     DescriptorSets descriptorSets);
        ~VkDescriptor() override;

        void setBindingValues(const std::unordered_set<DescriptorBindingValue> &values);

        [[nodiscard]] const std::unordered_set<DescriptorBindingValue> &getBindingValues() const override {
            return this->_bindingValues;
        }

        [[nodiscard]] const DescriptorSets &getDescriptorSets() const { return this->_descriptorSets; }

    private:
        std::unordered_set<DescriptorBindingValue> _bindingValues;

        VkDescriptorPoolManager *_descriptorPoolManager;

        DescriptorSets _descriptorSets;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_DESCRIPTOR_HPP
