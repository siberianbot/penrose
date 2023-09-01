#ifndef PENROSE_RENDERING_RENDER_SUBGRAPH_FACTORY_HPP
#define PENROSE_RENDERING_RENDER_SUBGRAPH_FACTORY_HPP

#include <Penrose/Rendering/RenderSubgraphInfo.hpp>

namespace Penrose {

    class RenderSubgraph;

    class RenderSubgraphFactory {
    public:
        virtual ~RenderSubgraphFactory() = default;

        [[nodiscard]] virtual RenderSubgraph *makeRenderSubgraph(const RenderSubgraphInfo &subgraphInfo) = 0;
    };
}

#endif // PENROSE_RENDERING_RENDER_SUBGRAPH_FACTORY_HPP
