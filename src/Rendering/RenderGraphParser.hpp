#ifndef PENROSE_RENDERING_RENDER_GRAPH_PARSER_HPP
#define PENROSE_RENDERING_RENDER_GRAPH_PARSER_HPP

#include <functional>
#include <typeindex>
#include <vector>

#include "src/Rendering/RenderGraph.hpp"

namespace Penrose {

    class DeviceContext;
    class DeviceMemoryAllocator;
    class PresentContext;

    class RenderGraphParser {
    private:
        using FinalizerFunc = std::function<void()>;

        struct Context {
            Context(DeviceContext *deviceContext,
                    DeviceMemoryAllocator *deviceMemoryAllocator,
                    PresentContext *presentContext,
                    const RenderGraph *graph);
            ~Context();

            DeviceContext *deviceContext;
            DeviceMemoryAllocator *deviceMemoryAllocator;
            PresentContext *presentContext;
            const RenderGraph *graph;

            std::vector<FinalizerFunc> finalizers;
        };

        Context _context;

    public:
        RenderGraphParser(DeviceContext *deviceContext,
                          DeviceMemoryAllocator *deviceMemoryAllocator,
                          PresentContext *presentContext,
                          const RenderGraph *graph);

        template<typename TIn, typename TOut>
        TOut parse(const TIn &);
    };
}

#include "RenderGraphParser.inl"

#endif // PENROSE_RENDERING_RENDER_GRAPH_PARSER_HPP
