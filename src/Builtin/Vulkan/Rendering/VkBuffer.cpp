#include "VkBuffer.hpp"

#include <Penrose/Common/EngineError.hpp>

#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"

namespace Penrose {

    VkBuffer::VkBuffer(VkLogicalDeviceContext *logicalDeviceContext,
                       BufferType type, std::uint64_t size, std::uint32_t count,
                       vk::Buffer buffer, vk::DeviceMemory bufferMemory,
                       std::optional<BufferPtr> ptr)
            : _logicalDeviceContext(logicalDeviceContext),
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
            this->_logicalDeviceContext->getHandle().unmapMemory(this->_bufferMemory);
        }

        this->_logicalDeviceContext->getHandle().free(this->_bufferMemory);
        this->_logicalDeviceContext->getHandle().destroy(this->_buffer);
    }

    BufferPtr VkBuffer::getPointer() const {
        if (!this->_ptr.has_value()) {
            throw EngineError("Buffer is not mapped");
        }

        return *this->_ptr;
    }
}
