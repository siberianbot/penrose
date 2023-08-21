#ifndef PENROSE_RENDERING_RENDER_GRAPH_EXECUTOR_PROVIDER_HPP
#define PENROSE_RENDERING_RENDER_GRAPH_EXECUTOR_PROVIDER_HPP

#include <Penrose/Rendering/RenderGraphInfo.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ResourceSet;
    class DeviceContext;
    class PresentContext;
    class RenderGraphExecutor;

    class RenderGraphExecutorProvider : public Resource {
    public:
        explicit RenderGraphExecutorProvider(ResourceSet *resources);
        ~RenderGraphExecutorProvider() override = default;

        RenderGraphExecutor *createFor(const RenderGraphInfo &graph);

    private:
        ResourceSet *_resources;
        DeviceContext *_deviceContext;
        PresentContext *_presentContext;
    };
}

#endif // PENROSE_RENDERING_RENDER_GRAPH_EXECUTOR_PROVIDER_HPP
