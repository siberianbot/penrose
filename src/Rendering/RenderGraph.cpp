#include <Penrose/Rendering/RenderGraph.hpp>

#include <utility>

namespace Penrose {

    RenderPassOperator::RenderPassOperator(std::string name,
                                           ParamsCollection params)
            : _name(std::move(name)),
              _params(std::move(params)) {
        //
    }

    RenderPassOperator &RenderPassOperator::setName(std::string name) {
        this->_name = std::move(name);

        return *this;
    }

    RenderPassOperator &RenderPassOperator::setParams(ParamsCollection params) {
        this->_params = std::move(params);

        return *this;
    }

    RenderPassOperator &RenderPassOperator::withParameter(const ParamsCollection::KeyView &key,
                                                          const ParamsCollection::Value &value) {
        this->_params.set(key, value);

        return *this;
    }

    RenderPassInfo::RenderPassInfo(std::vector<std::uint32_t> dependsOn,
                                   std::vector<std::uint32_t> inputAttachments,
                                   std::vector<std::uint32_t> colorAttachments,
                                   std::optional<std::uint32_t> depthStencilAttachment,
                                   std::optional<RenderPassOperator> anOperator)
            : _dependsOn(std::move(dependsOn)),
              _inputAttachments(std::move(inputAttachments)),
              _colorAttachments(std::move(colorAttachments)),
              _depthStencilAttachment(depthStencilAttachment),
              _operator(std::move(anOperator)) {
        //
    }

    RenderPassInfo &RenderPassInfo::setDependsOn(const std::vector<std::uint32_t> &dependsOn) {
        this->_dependsOn = dependsOn;

        return *this;
    }

    RenderPassInfo &RenderPassInfo::setInputAttachments(const std::vector<std::uint32_t> &inputAttachments) {
        this->_inputAttachments = inputAttachments;

        return *this;
    }

    RenderPassInfo &RenderPassInfo::setColorAttachments(const std::vector<std::uint32_t> &colorAttachments) {
        this->_colorAttachments = colorAttachments;

        return *this;
    }

    RenderPassInfo &
    RenderPassInfo::setDepthStencilAttachment(const std::optional<std::uint32_t> &depthStencilAttachment) {
        this->_depthStencilAttachment = depthStencilAttachment;

        return *this;
    }

    RenderPassInfo &RenderPassInfo::setOperator(const std::optional<RenderPassOperator> &anOperator) {
        this->_operator = anOperator;

        return *this;
    }

    RenderPassInfo &RenderPassInfo::addDependencyIdx(std::uint32_t dependencyIdx) {
        this->_dependsOn.push_back(dependencyIdx);

        return *this;
    }

    RenderPassInfo &RenderPassInfo::addInputAttachmentIdx(std::uint32_t attachmentIdx) {
        this->_inputAttachments.push_back(attachmentIdx);

        return *this;
    }

    RenderPassInfo &RenderPassInfo::addColorAttachmentIdx(std::uint32_t attachmentIdx) {
        this->_colorAttachments.push_back(attachmentIdx);

        return *this;
    }

    RenderSubgraph::RenderSubgraph(std::vector<std::string> dependsOn,
                                   std::vector<RenderAttachmentInfo> attachments,
                                   std::vector<RenderPassInfo> passes,
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

    RenderSubgraph &RenderSubgraph::setPasses(const std::vector<RenderPassInfo> &passes) {
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

    RenderSubgraph &RenderSubgraph::addPass(const RenderPassInfo &pass) {
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
                .addPass(RenderPassInfo().addColorAttachmentIdx(0));

        return RenderGraph()
                .setTarget(SWAPCHAIN_TARGET, defaultTarget)
                .setSubgraph("Default", defaultPass);
    }

    std::uint8_t operator&(const RenderTargetType &lhs, const RenderTargetType &rhs) {
        return static_cast<std::uint8_t>(lhs) & static_cast<std::uint8_t>(rhs);
    }
}
