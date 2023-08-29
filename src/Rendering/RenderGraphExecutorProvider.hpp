#ifndef PENROSE_RENDERING_RENDER_GRAPH_EXECUTOR_PROVIDER_HPP
#define PENROSE_RENDERING_RENDER_GRAPH_EXECUTOR_PROVIDER_HPP

#include <Penrose/Rendering/RenderGraphInfo.hpp>
#include <Penrose/Rendering/RenderOperator.hpp>
#include <Penrose/Rendering/RenderTargetFactory.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ResourceSet;
    class DeviceContext;
    class PresentContext;
    class RenderGraphExecutor;
    class RenderSubgraphFactory;

    class RenderGraphExecutorProvider : public Resource {
    public:
        explicit RenderGraphExecutorProvider(ResourceSet *resources);
        ~RenderGraphExecutorProvider() override = default;

        RenderGraphExecutor *createFor(const RenderGraphInfo &graph);

    private:
        DeviceContext *_deviceContext;
        PresentContext *_presentContext;
        RenderSubgraphFactory *_renderSubgraphFactory;

        LazyCollection<RenderOperator> _renderOperators;
        Lazy<RenderTargetFactory> _renderTargetFactory;
    };
}

#endif // PENROSE_RENDERING_RENDER_GRAPH_EXECUTOR_PROVIDER_HPP
