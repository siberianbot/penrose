#include "VkDescriptor.hpp"

#include "src/Rendering/DeviceContext.hpp"

namespace Penrose {

    VkDescriptor::VkDescriptor(DeviceContext *deviceContext,
                               std::vector<vk::DescriptorSet> &&descriptorSets)
            : _deviceContext(deviceContext),
              _descriptorSets(descriptorSets) {
        //
    }

    VkDescriptor::~VkDescriptor() {
        this->_deviceContext->getLogicalDevice().free(this->_deviceContext->getDescriptorPool(),
                                                      this->_descriptorSets);
    }

    void VkDescriptor::setBindingValues(const std::set<DescriptorBindingValue> &values) {
        this->_bindingValues = values;
    }
}
