#include "VkRenderSubgraph.hpp"

#include "src/Rendering/DeviceContext.hpp"

namespace Penrose {

    VkRenderSubgraph::VkRenderSubgraph(RenderSubgraphInfo subgraphInfo,
                                       DeviceContext *deviceContext,
                                       vk::RenderPass renderPass,
                                       std::array<vk::Semaphore, INFLIGHT_FRAME_COUNT> semaphores)
            : _subgraphInfo(std::move(subgraphInfo)),
              _deviceContext(deviceContext),
              _renderPass(renderPass),
              _semaphores(semaphores) {
        //
    }

    VkRenderSubgraph::~VkRenderSubgraph() {
        for (const auto &semaphore: this->_semaphores) {
            this->_deviceContext->getLogicalDevice().destroy(semaphore);
        }

        this->_deviceContext->getLogicalDevice().destroy(this->_renderPass);
    }
}
