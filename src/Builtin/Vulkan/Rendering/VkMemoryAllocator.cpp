#include "VkMemoryAllocator.hpp"

#include <Penrose/Common/EngineError.hpp>

namespace Penrose {

    VkMemoryAllocator::VkMemoryAllocator(const ResourceSet *resources)
        : _logicalDeviceProvider(resources->get<VkLogicalDeviceProvider>()),
          _physicalDeviceProvider(resources->get<VkPhysicalDeviceProvider>()) {
        //
    }

    vk::UniqueDeviceMemory VkMemoryAllocator::allocateBuffer(const vk::Buffer &buffer, const bool local) {
        auto &device = this->_logicalDeviceProvider->getLogicalDevice().handle;

        const auto requirements = device->getBufferMemoryRequirements(buffer);
        auto memory = this->allocate(requirements, local);

        device->bindBufferMemory(buffer, memory.get(), 0);

        return memory;
    }

    vk::UniqueDeviceMemory VkMemoryAllocator::allocateImage(const vk::Image &image) {
        auto &device = this->_logicalDeviceProvider->getLogicalDevice().handle;

        const auto requirements = device->getImageMemoryRequirements(image);
        auto memory = this->allocate(requirements, true);

        device->bindImageMemory(image, memory.get(), 0);

        return memory;
    }

    vk::UniqueDeviceMemory VkMemoryAllocator::allocate(const vk::MemoryRequirements &requirements, const bool local) {
        const vk::MemoryPropertyFlags flags = local ? vk::MemoryPropertyFlagBits::eDeviceLocal
                                                    : vk::MemoryPropertyFlagBits::eHostVisible
                                                          | vk::MemoryPropertyFlagBits::eHostCoherent;

        const auto properties = this->_physicalDeviceProvider->getPhysicalDevice().handle.getMemoryProperties();
        std::optional<uint32_t> memTypeIdx;

        for (uint32_t idx = 0; !memTypeIdx.has_value() && idx < properties.memoryTypeCount; idx++) {
            const bool typeMatches = requirements.memoryTypeBits & (1 << idx);
            const bool propertiesMatches = (properties.memoryTypes[idx].propertyFlags & flags) == flags;

            if (!typeMatches || !propertiesMatches) {
                continue;
            }

            memTypeIdx = idx;
        }

        if (!memTypeIdx.has_value()) {
            throw EngineError("No memory type available for required allocation");
        }

        const auto allocateInfo = vk::MemoryAllocateInfo()
                                      .setAllocationSize(requirements.size)
                                      .setMemoryTypeIndex(memTypeIdx.value());

        return this->_logicalDeviceProvider->getLogicalDevice().handle->allocateMemoryUnique(allocateInfo);
    }
}
