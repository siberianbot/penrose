#include <Penrose/Rendering/RenderGraph.hpp>

#include <utility>

namespace Penrose {

    RenderTargetInfo::RenderTargetInfo(RenderTargetSource source,
                                       RenderTargetType type,
                                       RenderFormat format,
                                       std::optional<Size> size)
            : _source(source),
              _type(type),
              _format(format),
              _size(std::move(size)) {
        //
    }

    RenderTargetInfo &RenderTargetInfo::setSource(RenderTargetSource source) {
        this->_source = source;

        return *this;
    }

    RenderTargetInfo &RenderTargetInfo::setType(RenderTargetType type) {
        this->_type = type;

        return *this;
    }

    RenderTargetInfo &RenderTargetInfo::setFormat(RenderFormat format) {
        this->_format = format;

        return *this;
    }

    RenderTargetInfo &RenderTargetInfo::setSize(std::optional<Size> size) {
        this->_size = size;

        return *this;
    }

    RenderTargetInfo RenderTargetInfo::makeSwapchain() {
        return RenderTargetInfo(RenderTargetSource::Swapchain);
    }

    RenderTargetInfo RenderTargetInfo::makeImage(RenderTargetType type,
                                                 RenderFormat format,
                                                 std::optional<Size> size) {
        return RenderTargetInfo(RenderTargetSource::Image,
                                type, format, size);
    }

    RenderAttachmentClearValue::RenderAttachmentClearValue(std::array<float, 4> color,
                                                           float depth,
                                                           std::uint32_t stencil)
            : _color(color),
              _depth(depth),
              _stencil(stencil) {
        //
    }

    RenderAttachmentClearValue &RenderAttachmentClearValue::setColor(std::array<float, 4> color) {
        this->_color = color;

        return *this;
    }

    RenderAttachmentClearValue &RenderAttachmentClearValue::setDepth(float depth) {
        this->_depth = depth;

        return *this;
    }

    RenderAttachmentClearValue &RenderAttachmentClearValue::setStencil(std::uint32_t stencil) {
        this->_stencil = stencil;

        return *this;
    }

    RenderAttachment::RenderAttachment(std::string target,
                                       RenderFormat format,
                                       RenderAttachmentClearValue clearValue,
                                       RenderAttachmentLoadOp loadOp,
                                       RenderAttachmentStoreOp storeOp,
                                       RenderAttachmentLayout initialLayout,
                                       RenderAttachmentLayout finalLayout)
            : _target(std::move(target)),
              _format(format),
              _clearValue(clearValue),
              _loadOp(loadOp),
              _storeOp(storeOp),
              _initialLayout(initialLayout),
              _finalLayout(finalLayout) {
        //
    }

    RenderAttachment &RenderAttachment::setTarget(std::string target) {
        this->_target = std::move(target);

        return *this;
    }

    RenderAttachment &RenderAttachment::setFormat(RenderFormat format) {
        this->_format = format;

        return *this;
    }

    RenderAttachment &RenderAttachment::setClearValue(RenderAttachmentClearValue clearValue) {
        this->_clearValue = clearValue;

        return *this;
    }

    RenderAttachment &RenderAttachment::setLoadOp(RenderAttachmentLoadOp loadOp) {
        this->_loadOp = loadOp;

        return *this;
    }

    RenderAttachment &RenderAttachment::setStoreOp(RenderAttachmentStoreOp storeOp) {
        this->_storeOp = storeOp;

        return *this;
    }

    RenderAttachment &RenderAttachment::setInitialLayout(RenderAttachmentLayout initialLayout) {
        this->_initialLayout = initialLayout;

        return *this;
    }

    RenderAttachment &RenderAttachment::setFinalLayout(RenderAttachmentLayout finalLayout) {
        this->_finalLayout = finalLayout;

        return *this;
    }

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

    RenderPassInfo &RenderPassInfo::setDepthStencilAttachment(const std::optional<std::uint32_t> &depthStencilAttachment) {
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
                                   std::vector<RenderAttachment> attachments,
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

    RenderSubgraph &RenderSubgraph::setAttachments(const std::vector<RenderAttachment> &attachments) {
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

    RenderSubgraph &RenderSubgraph::addAttachment(const RenderAttachment &attachment) {
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

        auto defaultTarget = RenderTargetInfo::makeSwapchain();
        auto defaultPass = RenderSubgraph()
                .addAttachment(RenderAttachment(SWAPCHAIN_TARGET)
                                       .setClearValue(RenderAttachmentClearValue().setColor({0, 0, 0, 1}))
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
