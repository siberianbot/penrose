#ifndef PENROSE_RENDERING_RENDER_GRAPH_EXECUTOR_PROVIDER_HPP
#define PENROSE_RENDERING_RENDER_GRAPH_EXECUTOR_PROVIDER_HPP

#include <Penrose/Rendering/RenderGraphInfo.hpp>
#include <Penrose/Rendering/RenderOperator.hpp>
#include <Penrose/Rendering/RenderTargetFactory.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSwapchainManager.hpp"

namespace Penrose {

    class ResourceSet;
    class RenderGraphExecutor;
    class RenderSubgraphFactory;

    class RenderGraphExecutorProvider : public Resource {
    public:
        explicit RenderGraphExecutorProvider(ResourceSet *resources);
        ~RenderGraphExecutorProvider() override = default;

        [[nodiscard]] RenderGraphExecutor *createFor(const RenderGraphInfo &graph);

    private:
        RenderSubgraphFactory *_renderSubgraphFactory;

        Lazy<VkLogicalDeviceContext> _logicalDeviceContext;
        Lazy<VkSwapchainManager> _swapchainManager;
        LazyCollection<RenderOperator> _renderOperators;
        Lazy<RenderTargetFactory> _renderTargetFactory;
    };
}

#endif // PENROSE_RENDERING_RENDER_GRAPH_EXECUTOR_PROVIDER_HPP
