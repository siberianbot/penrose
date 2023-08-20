#ifndef PENROSE_RENDERING_RENDER_GRAPH_HPP
#define PENROSE_RENDERING_RENDER_GRAPH_HPP

#include <array>
#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include <Penrose/Common/ParamsCollection.hpp>
#include <Penrose/Common/Size.hpp>

namespace Penrose {

    enum class RenderTargetSource {
        None,
        Image,
        Swapchain
    };

    enum class RenderTargetType : std::uint8_t {
        None = 0,
        Input = 1 << 0,
        Color = 1 << 1,
        DepthStencil = 1 << 2
    };

    enum class RenderFormat {
        None,
        R8UNorm,
        RGBA8UNorm,
        RGBA16Float,
        D32Float
    };

    enum class RenderAttachmentLayout {
        Undefined,
        ColorAttachment,
        DepthStencilAttachment,
        Present
    };

    enum class RenderAttachmentLoadOp {
        DontCare,
        Load,
        Clear
    };

    enum class RenderAttachmentStoreOp {
        DontCare,
        Store
    };

    class RenderTargetInfo {
    public:
        explicit RenderTargetInfo(RenderTargetSource source = RenderTargetSource::None,
                                  RenderTargetType type = RenderTargetType::None,
                                  RenderFormat format = RenderFormat::None,
                                  std::optional<Size> size = std::nullopt);

        [[nodiscard]] RenderTargetInfo &setSource(RenderTargetSource source);
        [[nodiscard]] RenderTargetInfo &setType(RenderTargetType type);
        [[nodiscard]] RenderTargetInfo &setFormat(RenderFormat format);
        [[nodiscard]] RenderTargetInfo &setSize(std::optional<Size> size);

        [[nodiscard]] const RenderTargetSource &getSource() const { return this->_source; }

        [[nodiscard]] const RenderTargetType &getType() const { return this->_type; }

        [[nodiscard]] const RenderFormat &getFormat() const { return this->_format; }

        [[nodiscard]] const std::optional<Size> &getSize() const { return this->_size; }

        [[nodiscard]] static RenderTargetInfo makeSwapchain();
        [[nodiscard]] static RenderTargetInfo makeImage(RenderTargetType type,
                                                        RenderFormat format,
                                                        std::optional<Size> size);

        [[nodiscard]] bool operator==(const RenderTargetInfo &rhs) const = default;

    private:
        RenderTargetSource _source;
        RenderTargetType _type;
        RenderFormat _format;
        std::optional<Size> _size;
    };

    class RenderAttachmentClearValue {
    public:
        explicit RenderAttachmentClearValue(std::array<float, 4> color = {},
                                            float depth = 0,
                                            std::uint32_t stencil = 0);

        [[nodiscard]] RenderAttachmentClearValue &setColor(std::array<float, 4> color);
        [[nodiscard]] RenderAttachmentClearValue &setDepth(float depth);
        [[nodiscard]] RenderAttachmentClearValue &setStencil(std::uint32_t stencil);

        [[nodiscard]] const std::array<float, 4> &getColor() const { return this->_color; }

        [[nodiscard]] const float &getDepth() const { return this->_depth; }

        [[nodiscard]] const std::uint32_t &getStencil() const { return this->_stencil; }

        [[nodiscard]] bool operator==(const RenderAttachmentClearValue &rhs) const = default;

    private:
        std::array<float, 4> _color;
        float _depth;
        std::uint32_t _stencil;
    };

    class RenderAttachment {
    public:
        explicit RenderAttachment(std::string target,
                                  RenderFormat format = RenderFormat::None,
                                  RenderAttachmentClearValue clearValue = RenderAttachmentClearValue{},
                                  RenderAttachmentLoadOp loadOp = RenderAttachmentLoadOp::DontCare,
                                  RenderAttachmentStoreOp storeOp = RenderAttachmentStoreOp::DontCare,
                                  RenderAttachmentLayout initialLayout = RenderAttachmentLayout::Undefined,
                                  RenderAttachmentLayout finalLayout = RenderAttachmentLayout::Undefined);

        [[nodiscard]] RenderAttachment &setTarget(std::string target);
        [[nodiscard]] RenderAttachment &setFormat(RenderFormat format);
        [[nodiscard]] RenderAttachment &setClearValue(RenderAttachmentClearValue clearValue);
        [[nodiscard]] RenderAttachment &setLoadOp(RenderAttachmentLoadOp loadOp);
        [[nodiscard]] RenderAttachment &setStoreOp(RenderAttachmentStoreOp storeOp);
        [[nodiscard]] RenderAttachment &setInitialLayout(RenderAttachmentLayout initialLayout);
        [[nodiscard]] RenderAttachment &setFinalLayout(RenderAttachmentLayout finalLayout);

        [[nodiscard]] const std::string &getTarget() const { return this->_target; }

        [[nodiscard]] const RenderFormat &getFormat() const { return this->_format; }

        [[nodiscard]] const RenderAttachmentClearValue &getClearValue() const { return this->_clearValue; }

        [[nodiscard]] const RenderAttachmentLoadOp &getLoadOp() const { return this->_loadOp; }

        [[nodiscard]] const RenderAttachmentStoreOp &getStoreOp() const { return this->_storeOp; }

        [[nodiscard]] const RenderAttachmentLayout &getInitialLayout() const { return this->_initialLayout; }

        [[nodiscard]] const RenderAttachmentLayout &getFinalLayout() const { return this->_finalLayout; }

        [[nodiscard]] bool operator==(const RenderAttachment &rhs) const = default;

    private:
        std::string _target;
        RenderFormat _format;
        RenderAttachmentClearValue _clearValue;
        RenderAttachmentLoadOp _loadOp;
        RenderAttachmentStoreOp _storeOp;
        RenderAttachmentLayout _initialLayout;
        RenderAttachmentLayout _finalLayout;
    };

    class RenderPassOperator {
    public:
        explicit RenderPassOperator(std::string name,
                                    ParamsCollection params = {});

        [[nodiscard]] RenderPassOperator &setName(std::string name);
        [[nodiscard]] RenderPassOperator &setParams(ParamsCollection params);

        [[nodiscard]] RenderPassOperator &withParameter(const ParamsCollection::KeyView &key,
                                                        const ParamsCollection::Value &value);

        [[nodiscard]] const std::string &getName() const { return this->_name; }

        [[nodiscard]] const ParamsCollection &getParams() const { return this->_params; }

        [[nodiscard]] bool operator==(const RenderPassOperator &rhs) const = default;

    private:
        std::string _name;
        ParamsCollection _params;
    };

    class RenderPassInfo {
    public:
        explicit RenderPassInfo(std::vector<std::uint32_t> dependsOn = {},
                                std::vector<std::uint32_t> inputAttachments = {},
                                std::vector<std::uint32_t> colorAttachments = {},
                                std::optional<std::uint32_t> depthStencilAttachment = std::nullopt,
                                std::optional<RenderPassOperator> anOperator = std::nullopt);

        [[nodiscard]] RenderPassInfo &setDependsOn(const std::vector<std::uint32_t> &dependsOn);
        [[nodiscard]] RenderPassInfo &setInputAttachments(const std::vector<std::uint32_t> &inputAttachments);
        [[nodiscard]] RenderPassInfo &setColorAttachments(const std::vector<std::uint32_t> &colorAttachments);
        [[nodiscard]] RenderPassInfo &setDepthStencilAttachment(const std::optional<std::uint32_t> &depthStencilAttachment);
        [[nodiscard]] RenderPassInfo &setOperator(const std::optional<RenderPassOperator> &anOperator);

        [[nodiscard]] RenderPassInfo &addDependencyIdx(std::uint32_t dependencyIdx);
        [[nodiscard]] RenderPassInfo &addInputAttachmentIdx(std::uint32_t attachmentIdx);
        [[nodiscard]] RenderPassInfo &addColorAttachmentIdx(std::uint32_t attachmentIdx);

        [[nodiscard]] const std::vector<std::uint32_t> &getDependsOn() const { return this->_dependsOn; }

        [[nodiscard]] const std::vector<std::uint32_t> &getInputAttachments() const { return this->_inputAttachments; }

        [[nodiscard]] const std::vector<std::uint32_t> &getColorAttachments() const { return this->_colorAttachments; }

        [[nodiscard]] const std::optional<std::uint32_t> &getDepthStencilAttachment() const {
            return this->_depthStencilAttachment;
        }

        [[nodiscard]] const std::optional<RenderPassOperator> &getOperator() const { return this->_operator; }

        [[nodiscard]] bool operator==(const RenderPassInfo &rhs) const = default;

    private:
        std::vector<std::uint32_t> _dependsOn;
        std::vector<std::uint32_t> _inputAttachments;
        std::vector<std::uint32_t> _colorAttachments;
        std::optional<std::uint32_t> _depthStencilAttachment;
        std::optional<RenderPassOperator> _operator;
    };

    class RenderSubgraph {
    public:
        explicit RenderSubgraph(std::vector<std::string> dependsOn = {},
                                std::vector<RenderAttachment> attachments = {},
                                std::vector<RenderPassInfo> passes = {},
                                std::optional<Size> renderArea = std::nullopt);

        [[nodiscard]] RenderSubgraph &setDependsOn(const std::vector<std::string> &dependsOn);
        [[nodiscard]] RenderSubgraph &setAttachments(const std::vector<RenderAttachment> &attachments);
        [[nodiscard]] RenderSubgraph &setPasses(const std::vector<RenderPassInfo> &passes);
        [[nodiscard]] RenderSubgraph &setRenderArea(const std::optional<Size> &renderArea);

        [[nodiscard]] RenderSubgraph &addDependency(const std::string &dependency);
        [[nodiscard]] RenderSubgraph &addAttachment(const RenderAttachment &attachment);
        [[nodiscard]] RenderSubgraph &addPass(const RenderPassInfo &pass);

        [[nodiscard]] const std::vector<std::string> &getDependsOn() const { return this->_dependsOn; }

        [[nodiscard]] const std::vector<RenderAttachment> &getAttachments() const { return this->_attachments; }

        [[nodiscard]] const std::vector<RenderPassInfo> &getPasses() const { return this->_passes; }

        [[nodiscard]] const std::optional<Size> &getRenderArea() const { return this->_renderArea; }

        [[nodiscard]] bool operator==(const RenderSubgraph &rhs) const = default;

    private:
        std::vector<std::string> _dependsOn;
        std::vector<RenderAttachment> _attachments;
        std::vector<RenderPassInfo> _passes;
        std::optional<Size> _renderArea;
    };

    class RenderGraph {
    public:
        RenderGraph &setTarget(const std::string &name, const RenderTargetInfo &target);
        void removeTarget(const std::string &name);

        RenderGraph &setSubgraph(const std::string &name, const RenderSubgraph &subgraph);
        void removeSubgraph(const std::string &name);

        [[nodiscard]] const std::map<std::string, RenderTargetInfo> &getTargets() const { return this->_targets; }

        [[nodiscard]] const std::map<std::string, RenderSubgraph> &getSubgraphs() const { return this->_subgraphs; }

        static RenderGraph makeDefault();

        [[nodiscard]] bool operator==(const RenderGraph &rhs) const = default;

    private:
        std::map<std::string, RenderTargetInfo> _targets;
        std::map<std::string, RenderSubgraph> _subgraphs;
    };

    [[nodiscard]] std::uint8_t operator&(const RenderTargetType &lhs, const RenderTargetType &rhs);
}

#endif // PENROSE_RENDERING_RENDER_GRAPH_HPP
