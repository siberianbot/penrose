#ifndef PENROSE_RENDERING_RENDERER_CONTEXT_HPP
#define PENROSE_RENDERING_RENDERER_CONTEXT_HPP

#include <functional>
#include <initializer_list>
#include <map>
#include <string>

#include <Penrose/Rendering/CommandRecorder.hpp>
#include <Penrose/Rendering/Graph/GraphInfo.hpp>
#include <Penrose/Rendering/Graph/TargetInfo.hpp>

namespace Penrose {

    /**
     * \brief Renderer function: underlying render system invokes this functions to record rendering commands
     */
    using RendererFunction = std::function<void(CommandRecorder *)>;

    /**
     * \brief Collection of renderer functions
     */
    using RendererFunctionMap = std::map<std::string, RendererFunction>;

    /**
     * \brief Renderer context interface
     * \details Renderer context is used to control rendering process within single renderer invocation.
     */
    class PENROSE_API RendererContext {
    public:
        virtual ~RendererContext() = default;

        /**
         * \brief Execute render graph
         * \param targets List of required targets
         * \param graph Instance of graph info
         * \param functions Collection of renderer functions
         */
        virtual void executeGraph(
            const std::initializer_list<TargetInfo> &targets, const GraphInfo &graph,
            const RendererFunctionMap &functions
        ) = 0;
    };
}

#endif // PENROSE_RENDERING_RENDERER_CONTEXT_HPP
