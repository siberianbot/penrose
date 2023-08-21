#ifndef PENROSE_BUILTIN_RENDERING_VK_FRAMEBUFFER_HPP
#define PENROSE_BUILTIN_RENDERING_VK_FRAMEBUFFER_HPP

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/RenderGraph.hpp>

namespace Penrose {

    class DeviceContext;
    class PresentContext;

    class VkRenderTarget;

    class VkFramebuffer {
    public:
        VkFramebuffer(DeviceContext *deviceContext,
                      std::vector<vk::Framebuffer> framebuffers,
                      std::vector<vk::ClearValue> clearValues,
                      vk::Rect2D renderArea);
        ~VkFramebuffer();

        [[nodiscard]] const vk::Framebuffer &getFramebuffer(std::uint32_t imageIdx) const {
            return this->_framebuffers.at(imageIdx);
        }

        [[nodiscard]] const std::vector<vk::ClearValue> &getClearValues() const { return this->_clearValues; }

        [[nodiscard]] const vk::Rect2D &getRenderArea() const { return this->_renderArea; }

    private:
        DeviceContext *_deviceContext;

        std::vector<vk::Framebuffer> _framebuffers;
        std::vector<vk::ClearValue> _clearValues;
        vk::Rect2D _renderArea;
    };

    [[nodiscard]] VkFramebuffer *makeVkFramebuffer(DeviceContext *deviceContext,
                                                   PresentContext *presentContext,
                                                   const std::map<std::string, VkRenderTarget *> &targets,
                                                   const vk::RenderPass &renderPass,
                                                   const RenderSubgraphInfo &subgraph);
}

#endif // PENROSE_BUILTIN_RENDERING_VK_FRAMEBUFFER_HPP
