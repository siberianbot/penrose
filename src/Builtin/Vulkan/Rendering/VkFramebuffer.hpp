#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_FRAMEBUFFER_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_FRAMEBUFFER_HPP

#include <cstdint>
#include <vector>

#include <vulkan/vulkan.hpp>

namespace Penrose {

    class VkLogicalDeviceContext;

    class VkFramebuffer {
    public:
        VkFramebuffer(VkLogicalDeviceContext *logicalDeviceContext,
                      std::vector<vk::Framebuffer> &&framebuffers,
                      std::vector<vk::ClearValue> &&clearValues,
                      vk::Rect2D &&renderArea);
        ~VkFramebuffer();

        [[nodiscard]] const vk::Framebuffer &getFramebuffer(std::uint32_t imageIdx) const {
            return this->_framebuffers.at(imageIdx);
        }

        [[nodiscard]] const std::vector<vk::ClearValue> &getClearValues() const { return this->_clearValues; }

        [[nodiscard]] const vk::Rect2D &getRenderArea() const { return this->_renderArea; }

    private:
        VkLogicalDeviceContext *_logicalDeviceContext;

        std::vector<vk::Framebuffer> _framebuffers;
        std::vector<vk::ClearValue> _clearValues;
        vk::Rect2D _renderArea;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_FRAMEBUFFER_HPP
