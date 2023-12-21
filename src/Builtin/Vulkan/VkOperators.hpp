#ifndef PENROSE_BUILTIN_VULKAN_VK_OPERATORS_HPP
#define PENROSE_BUILTIN_VULKAN_VK_OPERATORS_HPP

#include <vulkan/vulkan.hpp>

namespace Penrose {

    struct VkUniqueCommandBufferLess {
        bool operator()(const vk::UniqueCommandBuffer &lhs, const vk::UniqueCommandBuffer &rhs) const {
            return lhs.get() < rhs.get();
        }
    };

    struct VkUniqueFramebufferLess {
        bool operator()(const vk::UniqueFramebuffer &lhs, const vk::UniqueFramebuffer &rhs) const {
            return lhs.get() < rhs.get();
        }
    };

    struct VkUniqueDescriptorSetLess {
        bool operator()(const vk::UniqueDescriptorSet &lhs, const vk::UniqueDescriptorSet &rhs) const {
            return lhs.get() < rhs.get();
        }
    };
}

#endif // PENROSE_BUILTIN_VULKAN_VK_OPERATORS_HPP
