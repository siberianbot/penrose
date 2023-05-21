#include "DeviceMemoryAllocator.hpp"

#include <optional>

#include <vulkan/vulkan_hash.hpp>

#include "src/Common/EngineError.hpp"
#include "src/Rendering/DeviceContext.hpp"
#include "src/Resources/ResourceSet.hpp"

namespace Penrose {

    vk::DeviceMemory DeviceMemoryAllocator::allocate(const vk::MemoryRequirements &requirements,
                                                     const vk::MemoryPropertyFlags &requiredProperties) {

        auto memoryProperties = this->_deviceContext->getPhysicalDevice().getMemoryProperties();

        std::optional<uint32_t> memTypeIdx;

        for (uint32_t idx = 0; idx < memoryProperties.memoryTypeCount; idx++) {
            bool typeMatches = requirements.memoryTypeBits & (1 << idx);
            bool propertiesMatches =
                    (memoryProperties.memoryTypes[idx].propertyFlags & requiredProperties) == requiredProperties;

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

        return this->_deviceContext->getLogicalDevice().allocateMemory(allocateInfo);
    }

    void DeviceMemoryAllocator::free(const size_t &hash) {
        auto it = this->_allocatedMemory.find(hash);

        if (it == this->_allocatedMemory.end()) {
            return;
        }

        this->_deviceContext->getLogicalDevice().free(it->second);
        this->_allocatedMemory.erase(it);
    }

    DeviceMemoryAllocator::DeviceMemoryAllocator(ResourceSet *resources)
            : _deviceContext(resources->get<DeviceContext>()) {
        //
    }

    void DeviceMemoryAllocator::destroy() {
        for (const auto &[hash, memory]: this->_allocatedMemory) {
            this->_deviceContext->getLogicalDevice().free(memory);
        }

        this->_allocatedMemory.clear();
    }

    void DeviceMemoryAllocator::allocateFor(const vk::Image &image, const vk::MemoryPropertyFlags &requiredProperties) {
        auto objectHash = std::hash<vk::Image>{}(image);

        if (this->_allocatedMemory.contains(objectHash)) {
            throw EngineError("Attempt to reallocate memory for image");
        }

        auto memoryRequirements = this->_deviceContext->getLogicalDevice().getImageMemoryRequirements(image);

        vk::DeviceMemory memory;
        try {
            memory = this->allocate(memoryRequirements, requiredProperties);
        } catch (...) {
            std::throw_with_nested(EngineError("Failed to allocate memory for image"));
        }

        try {
            this->_deviceContext->getLogicalDevice().bindImageMemory(image, memory, 0);
        } catch (...) {
            std::throw_with_nested(EngineError("Failed to bind memory to image"));
        }
    }

    void DeviceMemoryAllocator::freeFor(const vk::Image &image) {
        this->free(std::hash<vk::Image>{}(image));
    }
}
