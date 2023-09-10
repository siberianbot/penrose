#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_GRAPH_CONTEXT_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_GRAPH_CONTEXT_HPP

#include <map>
#include <memory>
#include <optional>
#include <string>

#include <Penrose/Rendering/RenderGraphInfo.hpp>

#include "src/Builtin/Vulkan/Rendering/VkFramebuffer.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderSubgraph.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderTarget.hpp"

namespace Penrose {

    class VkFramebufferFactory;
    class VkRenderSubgraphFactory;
    class VkRenderTargetFactory;

    class VkRenderGraphContext {
    public:
        VkRenderGraphContext(VkFramebufferFactory *framebufferFactory,
                             VkRenderSubgraphFactory *renderSubgraphFactory,
                             VkRenderTargetFactory *renderTargetFactory,
                             std::optional<RenderGraphInfo> &&graphInfo,
                             std::map<std::string, std::shared_ptr<VkRenderSubgraph>> &&allocatedSubgraphs);

        [[nodiscard]] const std::optional<RenderGraphInfo> &getGraphInfo() const { return this->_graphInfo; }

        [[nodiscard]] std::shared_ptr<VkRenderSubgraph> getOrCreateSubgraph(const std::string &name);
        [[nodiscard]] std::shared_ptr<VkFramebuffer> getOrCreateFramebuffer(const std::string &name);

        [[nodiscard]] const std::map<std::string, std::shared_ptr<VkRenderSubgraph>> &getAllocatedSubgraphs() const {
            return this->_allocatedSubgraphs;
        }

    private:
        VkFramebufferFactory *_framebufferFactory;
        VkRenderSubgraphFactory *_renderSubgraphFactory;
        VkRenderTargetFactory *_renderTargetFactory;

        std::optional<RenderGraphInfo> _graphInfo;
        std::map<std::string, std::shared_ptr<VkRenderSubgraph>> _allocatedSubgraphs;
        std::map<std::string, std::shared_ptr<VkRenderTarget>> _allocatedTargets;
        std::map<std::string, std::shared_ptr<VkFramebuffer>> _allocatedFramebuffers;

        [[nodiscard]] std::shared_ptr<VkRenderTarget> getOrCreateTarget(const std::string &name);
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_GRAPH_CONTEXT_HPP
