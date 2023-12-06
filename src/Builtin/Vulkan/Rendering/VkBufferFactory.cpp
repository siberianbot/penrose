#include "VkBufferFactory.hpp"

#include "src/Builtin/Vulkan/Rendering/VkBuffer.hpp"
#include "src/Builtin/Vulkan/Rendering/VkUtils.hpp"

namespace Penrose {

    VkBufferFactory::VkBufferFactory(const ResourceSet *resources)
            : _logicalDeviceContext(resources->get<VkLogicalDeviceContext>()),
              _memoryAllocator(resources->get<VkMemoryAllocator>()) {
        //
    }

    Buffer *VkBufferFactory::makeBuffer(BufferType type, std::uint64_t size, std::uint32_t count, bool map) {
        auto usage = toVkBufferUsageFlags(type);

        auto createInfo = vk::BufferCreateInfo()
                .setUsage(usage)
                .setSize(size);

        auto buffer = this->_logicalDeviceContext->getHandle().createBuffer(createInfo);
        auto bufferMemory = this->_memoryAllocator->allocate(buffer, !map);

        std::optional<BufferPtr> ptr;
        if (map) {
            ptr = this->_logicalDeviceContext->getHandle().mapMemory(bufferMemory, 0, size);
        }

        return new VkBuffer(this->_logicalDeviceContext.get(), type, size, count, buffer, bufferMemory, ptr);
    }
}
