#ifndef PENROSE_RENDERING_RENDERER_HPP
#define PENROSE_RENDERING_RENDERER_HPP

#include <string>

// TODO: #include <Penrose/Rendering/Graph/GraphInfo.hpp>

namespace Penrose {

    /**
     * \brief Renderer interface
     * \details Renderer is responsible for actual rendering. Renderer must be independent from rendering library. Every
     * renderer uses render graph for configurable and flexible rendering process.
     */
    class Renderer {
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

        // TODO: invoke renderer

        /**
         * \brief Set current render graph
         * \param graphInfo Instance of render graph info
         */
        // TODO: virtual void setRenderGraph(GraphInfo &&graphInfo) = 0;
    };
}

#endif // PENROSE_RENDERING_RENDERER_HPP
