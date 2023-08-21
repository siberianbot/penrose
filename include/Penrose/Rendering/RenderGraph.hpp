#ifndef PENROSE_RENDERING_RENDER_GRAPH_HPP
#define PENROSE_RENDERING_RENDER_GRAPH_HPP

#include <array>
#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include <Penrose/Common/Size.hpp>
#include <Penrose/Rendering/RenderAttachmentInfo.hpp>
#include <Penrose/Rendering/RenderFormat.hpp>
#include <Penrose/Rendering/RenderOperatorInfo.hpp>
#include <Penrose/Rendering/RenderTargetInfo.hpp>

namespace Penrose {

    class RenderPassInfo {
    public:
        explicit RenderPassInfo(std::vector<std::uint32_t> dependsOn = {},
                                std::vector<std::uint32_t> inputAttachments = {},
                                std::vector<std::uint32_t> colorAttachments = {},
                                std::optional<std::uint32_t> depthStencilAttachment = std::nullopt,
                                std::optional<RenderOperatorInfo> anOperator = std::nullopt);

        [[nodiscard]] RenderPassInfo &setDependsOn(const std::vector<std::uint32_t> &dependsOn);
        [[nodiscard]] RenderPassInfo &setInputAttachments(const std::vector<std::uint32_t> &inputAttachments);
        [[nodiscard]] RenderPassInfo &setColorAttachments(const std::vector<std::uint32_t> &colorAttachments);
        [[nodiscard]] RenderPassInfo &
        setDepthStencilAttachment(const std::optional<std::uint32_t> &depthStencilAttachment);
        [[nodiscard]] RenderPassInfo &setOperator(const std::optional<RenderOperatorInfo> &anOperator);

        [[nodiscard]] RenderPassInfo &addDependencyIdx(std::uint32_t dependencyIdx);
        [[nodiscard]] RenderPassInfo &addInputAttachmentIdx(std::uint32_t attachmentIdx);
        [[nodiscard]] RenderPassInfo &addColorAttachmentIdx(std::uint32_t attachmentIdx);

        [[nodiscard]] const std::vector<std::uint32_t> &getDependsOn() const { return this->_dependsOn; }

        [[nodiscard]] const std::vector<std::uint32_t> &getInputAttachments() const { return this->_inputAttachments; }

        [[nodiscard]] const std::vector<std::uint32_t> &getColorAttachments() const { return this->_colorAttachments; }

        [[nodiscard]] const std::optional<std::uint32_t> &getDepthStencilAttachment() const {
            return this->_depthStencilAttachment;
        }

        [[nodiscard]] const std::optional<RenderOperatorInfo> &getOperator() const { return this->_operator; }

        [[nodiscard]] bool operator==(const RenderPassInfo &rhs) const = default;

    private:
        std::vector<std::uint32_t> _dependsOn;
        std::vector<std::uint32_t> _inputAttachments;
        std::vector<std::uint32_t> _colorAttachments;
        std::optional<std::uint32_t> _depthStencilAttachment;
        std::optional<RenderOperatorInfo> _operator;
    };

    class RenderSubgraph {
    public:
        explicit RenderSubgraph(std::vector<std::string> dependsOn = {},
                                std::vector<RenderAttachmentInfo> attachments = {},
                                std::vector<RenderPassInfo> passes = {},
                                std::optional<Size> renderArea = std::nullopt);

        [[nodiscard]] RenderSubgraph &setDependsOn(const std::vector<std::string> &dependsOn);
        [[nodiscard]] RenderSubgraph &setAttachments(const std::vector<RenderAttachmentInfo> &attachments);
        [[nodiscard]] RenderSubgraph &setPasses(const std::vector<RenderPassInfo> &passes);
        [[nodiscard]] RenderSubgraph &setRenderArea(const std::optional<Size> &renderArea);

        [[nodiscard]] RenderSubgraph &addDependency(const std::string &dependency);
        [[nodiscard]] RenderSubgraph &addAttachment(const RenderAttachmentInfo &attachment);
        [[nodiscard]] RenderSubgraph &addPass(const RenderPassInfo &pass);

        [[nodiscard]] const std::vector<std::string> &getDependsOn() const { return this->_dependsOn; }

        [[nodiscard]] const std::vector<RenderAttachmentInfo> &getAttachments() const { return this->_attachments; }

        [[nodiscard]] const std::vector<RenderPassInfo> &getPasses() const { return this->_passes; }

        [[nodiscard]] const std::optional<Size> &getRenderArea() const { return this->_renderArea; }

        [[nodiscard]] bool operator==(const RenderSubgraph &rhs) const = default;

    private:
        std::vector<std::string> _dependsOn;
        std::vector<RenderAttachmentInfo> _attachments;
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
