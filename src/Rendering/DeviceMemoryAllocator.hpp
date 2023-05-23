#ifndef PENROSE_RENDERING_DEVICE_MEMORY_ALLOCATOR_HPP
#define PENROSE_RENDERING_DEVICE_MEMORY_ALLOCATOR_HPP

#include <unordered_map>

#include <vulkan/vulkan.hpp>

#include "src/Common/Initializable.hpp"
#include "src/Resources/Resource.hpp"

namespace Penrose {

    class ResourceSet;
    class DeviceContext;

    class DeviceMemoryAllocator : public Resource, public Initializable {
    private:
        DeviceContext *_deviceContext;

        std::unordered_map<std::size_t, vk::DeviceMemory> _allocatedMemory;

        vk::DeviceMemory allocate(const vk::MemoryRequirements &requirements,
                                  const vk::MemoryPropertyFlags &requiredProperties);
        void free(const std::size_t &hash);

    public:
        explicit DeviceMemoryAllocator(ResourceSet *resources);
        ~DeviceMemoryAllocator() override = default;

        void init() override { /* nothing to do */ }

        void destroy() override;

        void allocateDeviceLocalFor(const vk::Image &image);
        void freeFor(const vk::Image &image);
    };
}

#endif // PENROSE_RENDERING_DEVICE_MEMORY_ALLOCATOR_HPP
