#include "VkRenderSubgraph.hpp"

#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"

namespace Penrose {

    VkRenderSubgraph::VkRenderSubgraph(RenderSubgraphInfo &&subgraphInfo,
                                       VkLogicalDeviceContext *logicalDeviceContext,
                                       vk::RenderPass renderPass,
                                       std::array<vk::Semaphore, INFLIGHT_FRAME_COUNT> semaphores)
            : _subgraphInfo(subgraphInfo),
              _logicalDeviceContext(logicalDeviceContext),
              _renderPass(renderPass),
              _semaphores(semaphores) {
        //
    }

    VkRenderSubgraph::~VkRenderSubgraph() {
        for (const auto &semaphore: this->_semaphores) {
            this->_logicalDeviceContext->getHandle().destroy(semaphore);
        }

        this->_logicalDeviceContext->getHandle().destroy(this->_renderPass);
    }
}
