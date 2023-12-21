#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_BUFFER_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_BUFFER_FACTORY_HPP

#include <tuple>

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/Objects/BufferFactory.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Rendering/Objects/VkLogicalDeviceProvider.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkPhysicalDeviceProvider.hpp"
#include "src/Builtin/Vulkan/Rendering/VkMemoryAllocator.hpp"

namespace Penrose {

    using VkBufferInternal = std::tuple<vk::UniqueBuffer, vk::UniqueDeviceMemory, Buffer::DataPtr>;

    class VkBufferFactory final: public Resource<VkBufferFactory>,
                                 public BufferFactory {
    public:
        explicit VkBufferFactory(const ResourceSet *resources);
        ~VkBufferFactory() override = default;

        void init();
        void destroy();

        [[nodiscard]] Buffer *makeBuffer(BufferType type, std::uint64_t size, bool map) override;
        [[nodiscard]] Buffer *makeBuffer(BufferType type, std::uint64_t size, bool map, void *srcData) override;

        [[nodiscard]] VkBufferInternal makeBuffer(vk::BufferUsageFlags usage, std::uint64_t size, bool map);

    private:
        ResourceProxy<VkPhysicalDeviceProvider> _physicalDeviceProvider;
        ResourceProxy<VkLogicalDeviceProvider> _logicalDeviceProvider;
        ResourceProxy<VkMemoryAllocator> _memoryAllocator;

        vk::UniqueCommandPool _commandPool;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_BUFFER_FACTORY_HPP
