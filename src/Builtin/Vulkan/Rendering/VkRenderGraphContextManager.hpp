#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_GRAPH_CONTEXT_MANAGER_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_GRAPH_CONTEXT_MANAGER_HPP

#include <memory>
#include <mutex>
#include <optional>

#include <Penrose/Rendering/RenderGraphContext.hpp>
#include <Penrose/Rendering/RenderGraphHook.hpp>
#include <Penrose/Rendering/RenderGraphInfo.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/Builtin/Vulkan/Rendering/VkFramebufferFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderGraphContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderSubgraphFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderTargetFactory.hpp"

namespace Penrose {

    class ResourceSet;

    class VkRenderGraphContextManager : public Resource,
                                        public Initializable,
                                        public RenderGraphHook {
    public:
        explicit VkRenderGraphContextManager(ResourceSet *resources);
        ~VkRenderGraphContextManager() override = default;

        void init() override;
        void destroy() override;

        void invalidate();

        void onRenderGraphModified(const RenderGraphInfo &graphInfo) override;

        [[nodiscard]] std::shared_ptr<VkRenderGraphContext> acquireContext();

    private:
        Lazy<VkFramebufferFactory> _framebufferFactory;
        Lazy<RenderGraphContext> _renderGraphContext;
        Lazy<VkRenderSubgraphFactory> _renderSubgraphFactory;
        Lazy<VkRenderTargetFactory> _renderTargetFactory;

        bool _initialized = false;

        std::mutex _mutex;
        std::optional<std::shared_ptr<VkRenderGraphContext>> _currentContext;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_GRAPH_CONTEXT_MANAGER_HPP
