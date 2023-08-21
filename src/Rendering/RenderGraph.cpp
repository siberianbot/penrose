#include <Penrose/Rendering/RenderGraph.hpp>

#include <utility>

namespace Penrose {

    RenderGraph &RenderGraph::setTarget(const std::string &name, const RenderTargetInfo &target) {
        this->_targets.insert_or_assign(name, target);

        return *this;
    }

    void RenderGraph::removeTarget(const std::string &name) {
        this->_targets.erase(name);
    }

    RenderGraph &RenderGraph::setSubgraph(const std::string &name, const RenderSubgraphInfo &subgraph) {
        this->_subgraphs.insert_or_assign(name, subgraph);

        return *this;
    }

    void RenderGraph::removeSubgraph(const std::string &name) {
        this->_subgraphs.erase(name);
    }

    RenderGraph RenderGraph::makeDefault() {
        constexpr static const char *SWAPCHAIN_TARGET = "Swapchain";

        auto defaultTarget = RenderTargetInfo(RenderTargetSource::Swapchain);

        auto defaultPass = RenderSubgraphInfo()
                .addAttachment(RenderAttachmentInfo(SWAPCHAIN_TARGET)
                                       .setClearValue(RenderAttachmentClearValueInfo().setColor({0, 0, 0, 1}))
                                       .setLoadOp(RenderAttachmentLoadOp::Clear)
                                       .setStoreOp(RenderAttachmentStoreOp::Store)
                                       .setInitialLayout(RenderAttachmentLayout::Undefined)
                                       .setFinalLayout(RenderAttachmentLayout::Present))
                .addPass(RenderSubgraphPassInfo().addColorAttachmentIdx(0));

        return RenderGraph()
                .setTarget(SWAPCHAIN_TARGET, defaultTarget)
                .setSubgraph("Default", defaultPass);
    }

    std::uint8_t operator&(const RenderTargetType &lhs, const RenderTargetType &rhs) {
        return static_cast<std::uint8_t>(lhs) & static_cast<std::uint8_t>(rhs);
    }
}
