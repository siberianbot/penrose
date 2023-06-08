#ifndef PENROSE_RENDERING_RENDER_GRAPH_HPP
#define PENROSE_RENDERING_RENDER_GRAPH_HPP

#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "src/Common/Types.hpp"

namespace Penrose {

    enum class RenderTargetSource {
        Image,
        Swapchain
    };

    enum class RenderTargetType : std::uint8_t {
        Input = 1 << 0,
        Color = 1 << 1,
        DepthStencil = 1 << 2
    };

    std::uint8_t operator&(const RenderTargetType &lhs, const RenderTargetType &rhs);

    enum class RenderFormat {
        R8UNorm,
        RGBA8UNorm,
        RGBA16Float,
        D32Float
    };

    struct RenderTarget {
        RenderTargetSource source;
        std::optional<RenderTargetType> type;
        std::optional<RenderFormat> format;
        std::optional<Size> size;
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

    struct RenderAttachmentClearValue {
        std::array<float, 4> color;
        float depth;
        std::uint32_t stencil;
    };

    struct RenderAttachment {
        std::uint32_t targetIdx;
        std::optional<RenderFormat> format;
        RenderAttachmentClearValue clearValue;
        RenderAttachmentLoadOp loadOp;
        RenderAttachmentStoreOp storeOp;
        RenderAttachmentLayout initialLayout;
        RenderAttachmentLayout finalLayout;
    };

    struct RenderPass {
        std::vector<std::uint32_t> dependsOn;
        std::vector<std::uint32_t> inputAttachments;
        std::vector<std::uint32_t> colorAttachments;
        std::optional<std::uint32_t> depthStencilAttachment;
        std::optional<std::string> operatorName;
    };

    struct RenderSubgraph {
        std::vector<std::uint32_t> dependsOn;
        std::vector<RenderAttachment> attachments;
        std::vector<RenderPass> passes;
        std::optional<Size> renderArea;
    };

    struct RenderGraph {
        std::vector<RenderTarget> targets;
        std::vector<RenderSubgraph> subgraphs;
    };

    RenderGraph makeDefaultRenderGraph();
}

#endif // PENROSE_RENDERING_RENDER_GRAPH_HPP
