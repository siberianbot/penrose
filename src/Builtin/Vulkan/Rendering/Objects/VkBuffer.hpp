#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_BUFFER_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_BUFFER_HPP

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/Objects/Buffer.hpp>

namespace Penrose {

    class VkBuffer final: public Buffer {
    public:
        VkBuffer(
            const BufferType type, const std::uint64_t size, const DataPtr data, vk::UniqueBuffer &&buffer,
            vk::UniqueDeviceMemory &&bufferMemory
        )
            : _type(type),
              _size(size),
              _data(data),
              _buffer(std::forward<decltype(buffer)>(buffer)),
              _bufferMemory(std::forward<decltype(bufferMemory)>(bufferMemory)) {
            //
        }

        ~VkBuffer() override = default;

        [[nodiscard]] BufferType getType() const override { return this->_type; }

        [[nodiscard]] std::uint64_t getSize() const override { return this->_size; }

        [[nodiscard]] DataPtr getData() const override { return this->_data; }

        [[nodiscard]] const vk::Buffer &getHandle() const { return this->_buffer.get(); }

    private:
        BufferType _type;
        std::uint64_t _size;
        DataPtr _data;

        vk::UniqueBuffer _buffer;
        vk::UniqueDeviceMemory _bufferMemory;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_BUFFER_HPP
