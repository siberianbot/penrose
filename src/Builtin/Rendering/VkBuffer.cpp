#include "VkBuffer.hpp"

#include <Penrose/Common/EngineError.hpp>

#include "src/Rendering/DeviceContext.hpp"

namespace Penrose {

    VkBuffer::VkBuffer(DeviceContext *deviceContext,
                       BufferType type, std::uint64_t size, std::uint32_t count,
                       vk::Buffer buffer, vk::DeviceMemory bufferMemory,
                       std::optional<BufferPtr> ptr)
            : _deviceContext(deviceContext),
              _type(type),
              _size(size),
              _count(count),
              _buffer(buffer),
              _bufferMemory(bufferMemory),
              _ptr(ptr) {
        //
    }

    VkBuffer::~VkBuffer() {
        if (this->_ptr.has_value()) {
            this->_deviceContext->getLogicalDevice().unmapMemory(this->_bufferMemory);
        }

        this->_deviceContext->getLogicalDevice().free(this->_bufferMemory);
        this->_deviceContext->getLogicalDevice().destroy(this->_buffer);
    }

    BufferPtr VkBuffer::getPointer() const {
        if (!this->_ptr.has_value()) {
            throw EngineError("Buffer is not mapped");
        }

        return *this->_ptr;
    }
}
