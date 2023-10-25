#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_GRAPH_CONTEXT_MANAGER_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_GRAPH_CONTEXT_MANAGER_HPP

#include <memory>
#include <mutex>
#include <optional>

#include <Penrose/Rendering/RenderGraphContext.hpp>
#include <Penrose/Rendering/RenderGraphHook.hpp>
#include <Penrose/Rendering/RenderGraphInfo.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Rendering/VkFramebufferFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderGraphContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderSubgraphFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderTargetFactory.hpp"

namespace Penrose {

    class VkRenderGraphContextManager : public Resource<VkRenderGraphContextManager, ResourceGroup::Rendering>,
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
        ResourceProxy<VkFramebufferFactory> _framebufferFactory;
        ResourceProxy<RenderGraphContext> _renderGraphContext;
        ResourceProxy<VkRenderSubgraphFactory> _renderSubgraphFactory;
        ResourceProxy<VkRenderTargetFactory> _renderTargetFactory;

        bool _initialized = false;

        std::mutex _mutex;
        std::optional<std::shared_ptr<VkRenderGraphContext>> _currentContext;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_GRAPH_CONTEXT_MANAGER_HPP
