#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_SUBGRAPH_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_SUBGRAPH_HPP

#include <array>
#include <cstdint>

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/RenderSubgraph.hpp>

#include "src/Builtin/Vulkan/Constants.hpp"

namespace Penrose {

    class DeviceContext;

    class VkRenderSubgraph : public RenderSubgraph {
    public:
        VkRenderSubgraph(RenderSubgraphInfo subgraphInfo,
                         DeviceContext *deviceContext,
                         vk::RenderPass renderPass,
                         std::array<vk::Semaphore, INFLIGHT_FRAME_COUNT> semaphores);
        ~VkRenderSubgraph() override;

        [[nodiscard]] const RenderSubgraphInfo &getSubgraphInfo() const override { return this->_subgraphInfo; }

        [[nodiscard]] const vk::RenderPass &getRenderPass() const { return this->_renderPass; }

        [[nodiscard]] const vk::Semaphore &getSemaphore(std::uint32_t frameIdx) const {
            return this->_semaphores.at(frameIdx);
        }

    private:
        RenderSubgraphInfo _subgraphInfo;

        DeviceContext *_deviceContext;

        vk::RenderPass _renderPass;
        std::array<vk::Semaphore, INFLIGHT_FRAME_COUNT> _semaphores;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_SUBGRAPH_HPP
