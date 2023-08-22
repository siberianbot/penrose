#ifndef PENROSE_RENDERING_RENDER_SUBGRAPH_HPP
#define PENROSE_RENDERING_RENDER_SUBGRAPH_HPP

#include <Penrose/Rendering/RenderSubgraphInfo.hpp>

namespace Penrose {

    class RenderSubgraph {
    public:
        virtual ~RenderSubgraph() = default;

        [[nodiscard]] virtual const RenderSubgraphInfo &getSubgraphInfo() const = 0;
    };
}

#endif // PENROSE_RENDERING_RENDER_SUBGRAPH_HPP
