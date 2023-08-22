#ifndef PENROSE_BUILTIN_RENDERING_VK_RENDER_SUBGRAPH_FACTORY_HPP
#define PENROSE_BUILTIN_RENDERING_VK_RENDER_SUBGRAPH_FACTORY_HPP

#include <Penrose/Rendering/RenderSubgraphFactory.hpp>

namespace Penrose {

    class ResourceSet;
    class DeviceContext;
    class PresentContext;

    class VkRenderSubgraphFactory : public RenderSubgraphFactory {
    public:
        explicit VkRenderSubgraphFactory(ResourceSet *resources);
        ~VkRenderSubgraphFactory() override = default;

        [[nodiscard]] RenderSubgraph *makeRenderSubgraph(const RenderSubgraphInfo &subgraphInfo) override;

    private:
        DeviceContext *_deviceContext;
        PresentContext *_presentContext;
    };
}

#endif // PENROSE_BUILTIN_RENDERING_VK_RENDER_SUBGRAPH_FACTORY_HPP
