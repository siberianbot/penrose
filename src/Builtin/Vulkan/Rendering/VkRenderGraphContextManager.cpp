#include "VkRenderGraphContextManager.hpp"

#include <utility>

#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    VkRenderGraphContextManager::VkRenderGraphContextManager(ResourceSet *resources)
            : _framebufferFactory(resources->getLazy<VkFramebufferFactory>()),
              _renderGraphContext(resources->getLazy<RenderGraphContext>()),
              _renderSubgraphFactory(resources->getLazy<VkRenderSubgraphFactory>()),
              _renderTargetFactory(resources->getLazy<VkRenderTargetFactory>()) {
        //
    }

    void VkRenderGraphContextManager::init() {
        auto graphInfo = this->_renderGraphContext->getRenderGraph();

        this->_currentContext = std::make_shared<VkRenderGraphContext>(
                this->_framebufferFactory.get(),
                this->_renderSubgraphFactory.get(),
                this->_renderTargetFactory.get(),
                std::forward<decltype(graphInfo)>(graphInfo),
                std::forward<std::map<std::string, std::shared_ptr<VkRenderSubgraph>>>({}));

        this->_initialized = true;
    }

    void VkRenderGraphContextManager::destroy() {
        this->_initialized = false;

        this->_currentContext = std::nullopt;
    }

    void VkRenderGraphContextManager::invalidate() {
        if (!this->_initialized) {
            return;
        }

        auto lock = std::lock_guard<std::mutex>(this->_mutex);

        std::optional<RenderGraphInfo> graphInfo;
        std::map<std::string, std::shared_ptr<VkRenderSubgraph>> subgraphs;

        if (this->_currentContext.has_value()) {
            graphInfo = (*this->_currentContext)->getGraphInfo();
            subgraphs = (*this->_currentContext)->getAllocatedSubgraphs();
        }

        this->_currentContext = std::make_shared<VkRenderGraphContext>(
                this->_framebufferFactory.get(),
                this->_renderSubgraphFactory.get(),
                this->_renderTargetFactory.get(),
                std::forward<decltype(graphInfo)>(graphInfo),
                std::forward<decltype(subgraphs)>(subgraphs));
    }

    void VkRenderGraphContextManager::onRenderGraphModified(const std::optional<RenderGraphInfo> &graphInfo) {
        if (!this->_initialized) {
            return;
        }

        auto lock = std::lock_guard<std::mutex>(this->_mutex);

        auto oldGraphContext = this->_currentContext;

        std::map<std::string, std::shared_ptr<VkRenderSubgraph>> allocatedSubgraphs;

        if (oldGraphContext.has_value() && graphInfo.has_value()) {
            for (const auto &[subgraphName, subgraph]: (*oldGraphContext)->getAllocatedSubgraphs()) {
                auto it = graphInfo->getSubgraphs().find(subgraphName);

                if (it == graphInfo->getSubgraphs().end() || it->second != subgraph->getSubgraphInfo()) {
                    continue;
                }

                allocatedSubgraphs.emplace(subgraphName, subgraph);
            }
        }

        auto graphInfoCopy = graphInfo;
        this->_currentContext = std::make_shared<VkRenderGraphContext>(
                this->_framebufferFactory.get(),
                this->_renderSubgraphFactory.get(),
                this->_renderTargetFactory.get(),
                std::forward<decltype(graphInfoCopy)>(graphInfoCopy),
                std::forward<decltype(allocatedSubgraphs)>(allocatedSubgraphs));
    }

    std::shared_ptr<VkRenderGraphContext> VkRenderGraphContextManager::acquireContext() {
        auto lock = std::lock_guard<std::mutex>(this->_mutex);

        return *this->_currentContext;
    }
}
