#ifndef PENROSE_RENDERING_RENDER_GRAPH_HOOK_HPP
#define PENROSE_RENDERING_RENDER_GRAPH_HOOK_HPP

#include <Penrose/Rendering/RenderGraphInfo.hpp>

namespace Penrose {

    class RenderGraphHook {
    public:
        virtual ~RenderGraphHook() = default;

        virtual void onRenderGraphModified(const std::optional<RenderGraphInfo> &graphInfo) = 0;
    };
}

#endif // PENROSE_RENDERING_RENDER_GRAPH_HOOK_HPP
