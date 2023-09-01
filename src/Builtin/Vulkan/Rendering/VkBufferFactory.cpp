#include "VkBufferFactory.hpp"

#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Rendering/VkBuffer.hpp"
#include "src/Builtin/Vulkan/Rendering/VkUtils.hpp"

namespace Penrose {

    VkBufferFactory::VkBufferFactory(ResourceSet *resources)
            : _deviceContext(resources->getLazy<DeviceContext>()) {
        //
    }

    Buffer *VkBufferFactory::makeBuffer(BufferType type, std::uint64_t size, std::uint32_t count, bool map) {
        auto usage = toVkBufferUsageFlags(type);

        auto createInfo = vk::BufferCreateInfo()
                .setUsage(usage)
                .setSize(size);

        bool isVisible = (usage & vk::BufferUsageFlagBits::eTransferSrc) == vk::BufferUsageFlagBits::eTransferSrc ||
                         (usage & vk::BufferUsageFlagBits::eUniformBuffer) == vk::BufferUsageFlagBits::eUniformBuffer;

        auto buffer = this->_deviceContext->getLogicalDevice().createBuffer(createInfo);
        auto bufferMemory = makeDeviceMemory(this->_deviceContext.get(), buffer, !isVisible);

        std::optional<BufferPtr> ptr;
        if (map) {
            ptr = this->_deviceContext->getLogicalDevice().mapMemory(bufferMemory, 0, size);
        }

        return new VkBuffer(this->_deviceContext.get(), type, size, count, buffer, bufferMemory, ptr);
    }
}
