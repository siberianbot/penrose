#ifndef PENROSE_RENDERING_RENDER_EXECUTION_INFO
#define PENROSE_RENDERING_RENDER_EXECUTION_INFO

#include <string>
#include <vector>

#include <Penrose/Common/Params.hpp>

namespace Penrose {

    /**
     * \brief Renderer execution info defines execution parameters of renderer
     */
    struct PENROSE_API RendererExecutionInfo {

        /**
         * \brief Name of renderer
         */
        std::string name;

        /**
         * \brief Renderer execution parameters
         */
        Params params;
    };

    /**
     * \brief Render execution info defines execution order of renderers
     */
    struct PENROSE_API RenderExecutionInfo {

        /**
         * \brief Ordered collection of renderer execution info
         */
        std::vector<RendererExecutionInfo> renderers;
    };
}

#endif // PENROSE_RENDERING_RENDER_EXECUTION_INFO
