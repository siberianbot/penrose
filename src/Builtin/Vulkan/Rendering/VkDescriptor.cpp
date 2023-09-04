#include "VkDescriptor.hpp"

#include "src/Rendering/DeviceContext.hpp"

#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"

namespace Penrose {

    VkDescriptor::VkDescriptor(DeviceContext *deviceContext,
                               VkLogicalDeviceContext *logicalDeviceContext,
                               std::vector<vk::DescriptorSet> &&descriptorSets)
            : _deviceContext(deviceContext),
              _logicalDeviceContext(logicalDeviceContext),
              _descriptorSets(descriptorSets) {
        //
    }

    VkDescriptor::~VkDescriptor() {
        this->_logicalDeviceContext->getHandle().free(this->_deviceContext->getDescriptorPool(),
                                                      this->_descriptorSets);
    }

    void VkDescriptor::setBindingValues(const std::unordered_set<DescriptorBindingValue> &values) {
        this->_bindingValues = values;
    }
}
