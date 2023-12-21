#include "VkBufferFactory.hpp"

#include "src/Builtin/Vulkan/Rendering/Objects/VkBuffer.hpp"
#include "src/Builtin/Vulkan/Rendering/VkUtils.hpp"

namespace Penrose {

    VkBufferFactory::VkBufferFactory(const ResourceSet *resources)
        : _physicalDeviceProvider(resources->get<VkPhysicalDeviceProvider>()),
          _logicalDeviceProvider(resources->get<VkLogicalDeviceProvider>()),
          _memoryAllocator(resources->get<VkMemoryAllocator>()) {
        //
    }

    void VkBufferFactory::init() {
        const auto createInfo = vk::CommandPoolCreateInfo().setQueueFamilyIndex(
            this->_physicalDeviceProvider->getPhysicalDevice().transferFamilyIdx
        );

        this->_commandPool = this->_logicalDeviceProvider->getLogicalDevice().handle->createCommandPoolUnique(createInfo
        );
    }

    void VkBufferFactory::destroy() {
        this->_commandPool.reset();
    }

    Buffer *VkBufferFactory::makeBuffer(const BufferType type, const std::uint64_t size, const bool map) {
        auto [buffer, bufferMemory, data] = this->makeBuffer(mapBufferType(type), size, map);

        return new VkBuffer(type, size, data, std::move(buffer), std::move(bufferMemory));
    }

    Buffer *VkBufferFactory::makeBuffer(
        const BufferType type, const std::uint64_t size, const bool map, void *srcData
    ) {
        auto [stagingBuffer, stagingBufferMemory, stagingData] = this->makeBuffer(
            vk::BufferUsageFlagBits::eTransferSrc, size, true
        );
        auto [destBuffer, destBufferMemory, destData] = this->makeBuffer(mapBufferType(type), size, map);

        std::memcpy(stagingData, srcData, size);

        {
            auto &device = this->_logicalDeviceProvider->getLogicalDevice().handle;
            auto &transferQueue = this->_logicalDeviceProvider->getLogicalDevice().transferQueue;

            const auto allocateInfo = vk::CommandBufferAllocateInfo()
                                          .setLevel(vk::CommandBufferLevel::ePrimary)
                                          .setCommandPool(this->_commandPool.get())
                                          .setCommandBufferCount(1);

            auto commandBuffer = std::move(device->allocateCommandBuffersUnique(allocateInfo).at(0));

            commandBuffer->begin(vk::CommandBufferBeginInfo().setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));
            commandBuffer->copyBuffer(stagingBuffer.get(), destBuffer.get(), vk::BufferCopy(0, 0, size));
            commandBuffer->end();

            transferQueue.submit(vk::SubmitInfo().setCommandBuffers(commandBuffer.get()));
            transferQueue.waitIdle();
        }

        return new VkBuffer(type, size, destData, std::move(destBuffer), std::move(destBufferMemory));
    }

    VkBufferInternal VkBufferFactory::makeBuffer(
        const vk::BufferUsageFlags usage, const std::uint64_t size, const bool map
    ) {
        const auto createInfo = vk::BufferCreateInfo().setUsage(usage).setSize(size);

        auto &device = this->_logicalDeviceProvider->getLogicalDevice().handle;

        auto buffer = device->createBufferUnique(createInfo);
        auto bufferMemory = this->_memoryAllocator->allocateBuffer(buffer.get(), !map);
        auto data = map ? device->mapMemory(bufferMemory.get(), 0, size) : nullptr;

        return {
            std::move(buffer),
            std::move(bufferMemory),
            data,
        };
    }
}
