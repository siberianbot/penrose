#ifndef PENROSE_BUILTIN_VULKAN_VK_SURFACE_PROVIDER_HPP
#define PENROSE_BUILTIN_VULKAN_VK_SURFACE_PROVIDER_HPP

#include <vulkan/vulkan.hpp>

namespace Penrose {

    class Surface;

    /**
     * \brief Interface of Vulkan surface instance provider
     */
    class VkSurfaceProvider {
    public:
        virtual ~VkSurfaceProvider() = default;

        /**
         * \brief Get instance of Vulkan surface object from surface
         * \param surface Instance of surface
         * \return Instance of Vulkan surface object
         */
        [[nodiscard]] virtual vk::SurfaceKHR getVkSurfaceFor(Surface *surface) = 0;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_VK_SURFACE_PROVIDER_HPP
