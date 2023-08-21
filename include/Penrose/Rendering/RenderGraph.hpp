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
#include <Penrose/Rendering/RenderSubgraphPassInfo.hpp>
#include <Penrose/Rendering/RenderTargetInfo.hpp>

namespace Penrose {

    class RenderSubgraph {
    public:
        explicit RenderSubgraph(std::vector<std::string> dependsOn = {},
                                std::vector<RenderAttachmentInfo> attachments = {},
                                std::vector<RenderSubgraphPassInfo> passes = {},
                                std::optional<Size> renderArea = std::nullopt);

        [[nodiscard]] RenderSubgraph &setDependsOn(const std::vector<std::string> &dependsOn);
        [[nodiscard]] RenderSubgraph &setAttachments(const std::vector<RenderAttachmentInfo> &attachments);
        [[nodiscard]] RenderSubgraph &setPasses(const std::vector<RenderSubgraphPassInfo> &passes);
        [[nodiscard]] RenderSubgraph &setRenderArea(const std::optional<Size> &renderArea);

        [[nodiscard]] RenderSubgraph &addDependency(const std::string &dependency);
        [[nodiscard]] RenderSubgraph &addAttachment(const RenderAttachmentInfo &attachment);
        [[nodiscard]] RenderSubgraph &addPass(const RenderSubgraphPassInfo &pass);

        [[nodiscard]] const std::vector<std::string> &getDependsOn() const { return this->_dependsOn; }

        [[nodiscard]] const std::vector<RenderAttachmentInfo> &getAttachments() const { return this->_attachments; }

        [[nodiscard]] const std::vector<RenderSubgraphPassInfo> &getPasses() const { return this->_passes; }

        [[nodiscard]] const std::optional<Size> &getRenderArea() const { return this->_renderArea; }

        [[nodiscard]] bool operator==(const RenderSubgraph &rhs) const = default;

    private:
        std::vector<std::string> _dependsOn;
        std::vector<RenderAttachmentInfo> _attachments;
        std::vector<RenderSubgraphPassInfo> _passes;
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
