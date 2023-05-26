#ifndef PENROSE_RENDERING_RENDER_GRAPH_PARSER_HPP
#define PENROSE_RENDERING_RENDER_GRAPH_PARSER_HPP

#include <functional>
#include <vector>

namespace Penrose {

    class DeviceContext;
    class DeviceMemoryAllocator;
    class PresentContext;

    class RenderGraphParser {
    private:
        using FinalizerFunc = std::function<void()>;

        PresentContext *_presentContext;

        std::vector<FinalizerFunc> _finalizers;

    public:
        explicit RenderGraphParser(PresentContext *presentContext);
        ~RenderGraphParser();

        template<typename TIn, typename TOut>
        TOut parse(const TIn &);
    };
}

#include "RenderGraphParser.inl"

#endif // PENROSE_RENDERING_RENDER_GRAPH_PARSER_HPP
