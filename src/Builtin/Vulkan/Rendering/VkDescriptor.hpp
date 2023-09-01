#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_DESCRIPTOR_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_DESCRIPTOR_HPP

#include <vector>

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/Descriptor.hpp>

namespace Penrose {

    class DeviceContext;

    class VkDescriptor : public Descriptor {
    public:
        VkDescriptor(DeviceContext *deviceContext,
                     std::vector<vk::DescriptorSet> &&descriptorSets);
        ~VkDescriptor() override;

        void setBindingValues(const std::unordered_set<DescriptorBindingValue> &values);

        [[nodiscard]] const std::unordered_set<DescriptorBindingValue> &getBindingValues() const override {
            return this->_bindingValues;
        }

        [[nodiscard]] const std::vector<vk::DescriptorSet> &getDescriptorSets() const { return this->_descriptorSets; }

    private:
        std::unordered_set<DescriptorBindingValue> _bindingValues;

        DeviceContext *_deviceContext;

        std::vector<vk::DescriptorSet> _descriptorSets;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_DESCRIPTOR_HPP
