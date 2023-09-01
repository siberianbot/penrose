#ifndef PENROSE_BUILTIN_VULKAN_VK_SURFACE_PROVIDER_HPP
#define PENROSE_BUILTIN_VULKAN_VK_SURFACE_PROVIDER_HPP

#include <vulkan/vulkan.hpp>

namespace Penrose {

    class Surface;

    class VkSurfaceProvider {
    public:
        virtual ~VkSurfaceProvider() = default;

        [[nodiscard]] virtual vk::SurfaceKHR getVkSurfaceFor(Surface *surface) = 0;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_VK_SURFACE_PROVIDER_HPP
