#ifndef PENROSE_BUILTIN_RENDERING_VK_BUFFER_FACTORY_HPP
#define PENROSE_BUILTIN_RENDERING_VK_BUFFER_FACTORY_HPP

#include <Penrose/Rendering/BufferFactory.hpp>

namespace Penrose {

    class ResourceSet;
    class DeviceContext;

    class VkBufferFactory : public BufferFactory {
    public:
        explicit VkBufferFactory(ResourceSet *resources);
        ~VkBufferFactory() override = default;

        Buffer *makeBuffer(BufferType type, std::uint64_t size, std::uint32_t count, bool map) override;

    private:
        DeviceContext *_deviceContext;
    };
}

#endif // PENROSE_BUILTIN_RENDERING_VK_BUFFER_FACTORY_HPP
