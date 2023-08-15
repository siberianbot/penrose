#ifndef PENROSE_BUILTIN_RENDERING_VK_BUFFER_HPP
#define PENROSE_BUILTIN_RENDERING_VK_BUFFER_HPP

#include <optional>

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/Buffer.hpp>

namespace Penrose {

    class DeviceContext;

    class VkBuffer : public Buffer {
    public:
        VkBuffer(DeviceContext *deviceContext,
                 BufferType type, std::uint64_t size, std::uint32_t count,
                 vk::Buffer buffer, vk::DeviceMemory bufferMemory,
                 std::optional<BufferPtr> ptr);
        ~VkBuffer() override;

        [[nodiscard]] BufferType getType() const override { return this->_type; }

        [[nodiscard]] uint64_t getSize() const override { return this->_size; }

        [[nodiscard]] uint32_t getCount() const override { return this->_count; }

        [[nodiscard]] BufferPtr getPointer() const override;

        [[nodiscard]] const vk::Buffer &getBuffer() const { return this->_buffer; }

    private:
        DeviceContext *_deviceContext;

        BufferType _type;
        std::uint64_t _size;
        std::uint32_t _count;

        vk::Buffer _buffer;
        vk::DeviceMemory _bufferMemory;

        std::optional<BufferPtr> _ptr;
    };
}

#endif // PENROSE_BUILTIN_RENDERING_VK_BUFFER_HPP
