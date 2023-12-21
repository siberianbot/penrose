#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_SWAPCHAIN_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_SWAPCHAIN_HPP

#include <memory>

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/Surface.hpp>

namespace Penrose {

    struct VkSwapchain {
        std::shared_ptr<Surface> surface;
        vk::Extent2D extent;
        vk::Format format;
        vk::UniqueSwapchainKHR handle;
        std::vector<vk::UniqueImageView> imageViews;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_SWAPCHAIN_HPP
