#include "VkFramebuffer.hpp"

#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"

namespace Penrose {

    VkFramebuffer::VkFramebuffer(VkLogicalDeviceContext *logicalDeviceContext,
                                 std::vector<vk::Framebuffer> &&framebuffers,
                                 std::vector<vk::ClearValue> &&clearValues,
                                 vk::Rect2D &&renderArea)
            : _logicalDeviceContext(logicalDeviceContext),
              _framebuffers(framebuffers),
              _clearValues(clearValues),
              _renderArea(renderArea) {
        //
    }

    VkFramebuffer::~VkFramebuffer() {
        for (const auto &framebuffer: this->_framebuffers) {
            this->_logicalDeviceContext->getHandle().destroy(framebuffer);
        }
    }
}
