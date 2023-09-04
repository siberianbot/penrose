#ifndef PENROSE_BUILTIN_VULKAN_CONSTANTS_HPP
#define PENROSE_BUILTIN_VULKAN_CONSTANTS_HPP

#include <array>
#include <cstdint>

namespace Penrose {

    constexpr static const std::uint32_t INFLIGHT_FRAME_COUNT = 2;

    constexpr static const std::array<const char *, 2> REQUIRED_DEVICE_EXTENSIONS = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            VK_KHR_SHADER_NON_SEMANTIC_INFO_EXTENSION_NAME
    };
}

#endif // PENROSE_BUILTIN_VULKAN_CONSTANTS_HPP
