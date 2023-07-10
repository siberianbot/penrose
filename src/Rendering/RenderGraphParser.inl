#ifndef PENROSE_RENDERING_RENDER_GRAPH_PARSER_INL
#define PENROSE_RENDERING_RENDER_GRAPH_PARSER_INL

#include <fmt/core.h>

#include <Penrose/Common/EngineError.hpp>

namespace Penrose {

    template<typename TIn, typename TOut>
    TOut RenderGraphParser::parse(const TIn &) {
        throw EngineError(fmt::format("Unable to parse {} to {}", typeid(TIn).name(), typeid(TOut).name()));
    }
}

#endif // PENROSE_RENDERING_RENDER_GRAPH_PARSER_INL
