#include <Penrose/Rendering/RenderGraph.hpp>

#include <utility>

namespace Penrose {

    RenderSubgraph::RenderSubgraph(std::vector<std::string> dependsOn,
                                   std::vector<RenderAttachmentInfo> attachments,
                                   std::vector<RenderSubgraphPassInfo> passes,
                                   std::optional<Size> renderArea)
            : _dependsOn(std::move(dependsOn)),
              _attachments(std::move(attachments)),
              _passes(std::move(passes)),
              _renderArea(std::move(renderArea)) {
        //
    }

    RenderSubgraph &RenderSubgraph::setDependsOn(const std::vector<std::string> &dependsOn) {
        this->_dependsOn = dependsOn;

        return *this;
    }

    RenderSubgraph &RenderSubgraph::setAttachments(const std::vector<RenderAttachmentInfo> &attachments) {
        this->_attachments = attachments;

        return *this;
    }

    RenderSubgraph &RenderSubgraph::setPasses(const std::vector<RenderSubgraphPassInfo> &passes) {
        this->_passes = passes;

        return *this;
    }

    RenderSubgraph &RenderSubgraph::setRenderArea(const std::optional<Size> &renderArea) {
        this->_renderArea = renderArea;

        return *this;
    }

    RenderSubgraph &RenderSubgraph::addDependency(const std::string &dependency) {
        this->_dependsOn.push_back(dependency);

        return *this;
    }

    RenderSubgraph &RenderSubgraph::addAttachment(const RenderAttachmentInfo &attachment) {
        this->_attachments.push_back(attachment);

        return *this;
    }

    RenderSubgraph &RenderSubgraph::addPass(const RenderSubgraphPassInfo &pass) {
        this->_passes.push_back(pass);

        return *this;
    }

    RenderGraph &RenderGraph::setTarget(const std::string &name, const RenderTargetInfo &target) {
        this->_targets.insert_or_assign(name, target);

        return *this;
    }

    void RenderGraph::removeTarget(const std::string &name) {
        this->_targets.erase(name);
    }

    RenderGraph &RenderGraph::setSubgraph(const std::string &name, const RenderSubgraph &subgraph) {
        this->_subgraphs.insert_or_assign(name, subgraph);

        return *this;
    }

    void RenderGraph::removeSubgraph(const std::string &name) {
        this->_subgraphs.erase(name);
    }

    RenderGraph RenderGraph::makeDefault() {
        constexpr static const char *SWAPCHAIN_TARGET = "Swapchain";

        auto defaultTarget = RenderTargetInfo(RenderTargetSource::Swapchain);

        auto defaultPass = RenderSubgraph()
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
