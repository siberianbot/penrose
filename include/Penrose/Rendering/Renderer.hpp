#ifndef PENROSE_RENDERING_RENDERER_HPP
#define PENROSE_RENDERING_RENDERER_HPP

#include <string>

#include <Penrose/Common/Params.hpp>
#include <Penrose/Rendering/Graph/GraphInfo.hpp>
#include <Penrose/Rendering/RendererContext.hpp>

namespace Penrose {

    /**
     * \brief Renderer interface
     * \details Renderer performs  actual rendering logic. Renderer must be independent from rendering library. Every
     * renderer uses render graph for configurable and flexible rendering process.
     */
    class PENROSE_API Renderer {
    public:
        virtual ~Renderer() = default;

        /**
         * \brief Get name of renderer
         * \return Name of renderer
         */
        [[nodiscard]] virtual std::string getName() const = 0;

        /**
         * \brief Initialize renderer
         */
        virtual void init() = 0;

        /**
         * \brief Deinitialize renderer
         */
        virtual void destroy() = 0;

        /**
         * \brief Set current render graph
         * \param graphInfo Instance of render graph info
         */
        virtual void setRenderGraph(GraphInfo &&graphInfo) = 0;

        /**
         * \brief Get current render graph
         * \return Instance of render graph info in current renderer
         */
        [[nodiscard]] virtual const GraphInfo &getRenderGraph() = 0;

        /**
         * \brief Execute renderer in current render context
         * \param context Current renderer context
         * \param params Renderer execution parameters
         */
        virtual void execute(RendererContext *context, const Params &params) = 0;
    };
}

#endif // PENROSE_RENDERING_RENDERER_HPP
