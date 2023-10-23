#include "VkMemoryAllocator.hpp"

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    VkMemoryAllocator::VkMemoryAllocator(ResourceSet *resources)
            : _logicalDeviceContext(resources->get<VkLogicalDeviceContext>()),
              _physicalDeviceContext(resources->get<VkPhysicalDeviceContext>()) {
        //
    }

    vk::DeviceMemory VkMemoryAllocator::allocate(const vk::Buffer &buffer, bool local) {
        auto requirements = this->_logicalDeviceContext->getHandle().getBufferMemoryRequirements(buffer);
        auto memory = this->allocate(requirements, local);

        this->_logicalDeviceContext->getHandle().bindBufferMemory(buffer, memory, 0);

        return memory;
    }

    vk::DeviceMemory VkMemoryAllocator::allocate(const vk::Image &image, bool local) {
        auto requirements = this->_logicalDeviceContext->getHandle().getImageMemoryRequirements(image);
        auto memory = this->allocate(requirements, local);

        this->_logicalDeviceContext->getHandle().bindImageMemory(image, memory, 0);

        return memory;
    }

    vk::DeviceMemory VkMemoryAllocator::allocate(const vk::MemoryRequirements &requirements, bool local) {
        vk::MemoryPropertyFlags flags = local
                                        ? vk::MemoryPropertyFlagBits::eDeviceLocal
                                        : vk::MemoryPropertyFlagBits::eHostVisible |
                                          vk::MemoryPropertyFlagBits::eHostCoherent;

        auto properties = this->_physicalDeviceContext->getHandle().getMemoryProperties();
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

        return this->_logicalDeviceContext->getHandle().allocateMemory(allocateInfo);
    }
}
