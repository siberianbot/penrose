#include "VkRenderGraphContext.hpp"

#include <utility>

#include "src/Builtin/Vulkan/Rendering/VkFramebufferFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderSubgraphFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderTargetFactory.hpp"

namespace Penrose {

    VkRenderGraphContext::VkRenderGraphContext(VkFramebufferFactory *framebufferFactory,
                                               VkRenderSubgraphFactory *renderSubgraphFactory,
                                               VkRenderTargetFactory *renderTargetFactory,
                                               RenderGraphInfo &&graphInfo,
                                               std::map<std::string, std::shared_ptr<VkRenderSubgraph>> &&allocatedSubgraphs)
            : _framebufferFactory(framebufferFactory),
              _renderSubgraphFactory(renderSubgraphFactory),
              _renderTargetFactory(renderTargetFactory),
              _graphInfo(graphInfo),
              _allocatedSubgraphs(allocatedSubgraphs) {
        //
    }

    std::shared_ptr<VkRenderSubgraph> VkRenderGraphContext::getOrCreateSubgraph(const std::string &name) {
        auto it = this->_allocatedSubgraphs.find(name);

        if (it != this->_allocatedSubgraphs.end()) {
            return it->second;
        }

        auto subgraphInfo = this->_graphInfo.getSubgraphs().at(name);
        auto ptr = std::shared_ptr<VkRenderSubgraph>(
                this->_renderSubgraphFactory->makeRenderSubgraph(std::forward<decltype(subgraphInfo)>(subgraphInfo))
        );

        this->_allocatedSubgraphs.emplace(name, ptr);

        return ptr;
    }

    std::shared_ptr<VkFramebuffer> VkRenderGraphContext::getOrCreateFramebuffer(const std::string &name) {
        auto it = this->_allocatedFramebuffers.find(name);

        if (it != this->_allocatedFramebuffers.end()) {
            return it->second;
        }

        auto subgraph = this->_allocatedSubgraphs.at(name);

        std::map<std::string, VkRenderTarget *> targets;
        for (const auto &attachment: subgraph->getSubgraphInfo().getAttachments()) {
            auto targetName = attachment.getTarget();
            auto target = this->getOrCreateTarget(targetName);

            targets.emplace(targetName, target.get());
        }

        auto ptr = std::shared_ptr<VkFramebuffer>(
                this->_framebufferFactory->makeFramebuffer(targets, subgraph.get())
        );

        this->_allocatedFramebuffers.emplace(name, ptr);

        return ptr;
    }

    std::shared_ptr<VkRenderTarget> VkRenderGraphContext::getOrCreateTarget(const std::string &name) {
        auto it = this->_allocatedTargets.find(name);

        if (it != this->_allocatedTargets.end()) {
            return it->second;
        }

        auto targetInfo = this->_graphInfo.getTargets().at(name);
        auto ptr = std::shared_ptr<VkRenderTarget>(
                this->_renderTargetFactory->makeRenderTarget(std::forward<decltype(targetInfo)>(targetInfo))
        );

        this->_allocatedTargets.emplace(name, ptr);

        return ptr;
    }
}
