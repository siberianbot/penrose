#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_BUFFER_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_BUFFER_FACTORY_HPP

#include <Penrose/Rendering/BufferFactory.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkMemoryAllocator.hpp"

namespace Penrose {

    class ResourceSet;

    class VkBufferFactory : public Resource, public BufferFactory {
    public:
        explicit VkBufferFactory(ResourceSet *resources);
        ~VkBufferFactory() override = default;

        Buffer *makeBuffer(BufferType type, std::uint64_t size, std::uint32_t count, bool map) override;

    private:
        Lazy<VkLogicalDeviceContext> _logicalDeviceContext;
        Lazy<VkMemoryAllocator> _memoryAllocator;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_BUFFER_FACTORY_HPP
