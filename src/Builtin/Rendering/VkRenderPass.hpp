#ifndef PENROSE_BUILTIN_RENDERING_VK_RENDER_PASS_HPP
#define PENROSE_BUILTIN_RENDERING_VK_RENDER_PASS_HPP

#include <array>

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/RenderGraph.hpp>

#include "src/Constants.hpp"

namespace Penrose {

    class DeviceContext;
    class PresentContext;

    class VkRenderPass {
    public:
        VkRenderPass(DeviceContext *deviceContext,
                     vk::RenderPass renderPass,
                     std::array<vk::Semaphore, INFLIGHT_FRAME_COUNT> semaphores);
        ~VkRenderPass();

        [[nodiscard]] const vk::RenderPass &getRenderPass() const { return this->_renderPass; }

        [[nodiscard]] const vk::Semaphore &getSemaphore(std::uint32_t frameIdx) const {
            return this->_semaphores.at(frameIdx);
        }

    private:
        DeviceContext *_deviceContext;

        vk::RenderPass _renderPass;
        std::array<vk::Semaphore, INFLIGHT_FRAME_COUNT> _semaphores;
    };

    [[nodiscard]] VkRenderPass *makeVkRenderPass(DeviceContext *deviceContext,
                                                 PresentContext *presentContext,
                                                 const RenderSubgraphInfo &subgraph);
}

#endif // PENROSE_BUILTIN_RENDERING_VK_RENDER_PASS_HPP
