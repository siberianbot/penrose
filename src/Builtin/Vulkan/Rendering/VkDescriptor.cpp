#include "VkDescriptor.hpp"

#include "src/Builtin/Vulkan/Rendering/VkDescriptorPoolManager.hpp"

namespace Penrose {

    VkDescriptor::VkDescriptor(VkDescriptorPoolManager *descriptorPoolManager,
                               DescriptorSets descriptorSets)
            : _descriptorPoolManager(descriptorPoolManager),
              _descriptorSets(descriptorSets) {
        //
    }

    VkDescriptor::~VkDescriptor() {
        this->_descriptorPoolManager->destroyDescriptorSets(this->_descriptorSets);
    }

    void VkDescriptor::setBindingValues(const std::unordered_set<DescriptorBindingValue> &values) {
        this->_bindingValues = values;
    }
}
