#ifndef PENROSE_RENDERING_RENDER_CONTEXT_HPP
#define PENROSE_RENDERING_RENDER_CONTEXT_HPP

#include <Penrose/Rendering/RendererContext.hpp>

namespace Penrose {

    /**
     * \brief Render context interface
     * \details Render context stores all resources (allocated images, render passes, etc), which are shared between
     * every rendering invocation. It provides methods for working with rendering process.
     */
    class PENROSE_API RenderContext {
    public:
        virtual ~RenderContext() = default;

        /**
         * \brief Prepare current context for rendering
         * \return Can current context be submitted
         */
        [[nodiscard]] virtual bool beginRender() = 0;

        /**
         * \brief Submit rendering commands in current context
         */
        virtual void submitRender() = 0;

        /**
         * \brief Create new renderer context
         * \return New instance of renderer context
         */
        [[nodiscard]] virtual RendererContext *makeRendererContext() = 0;

        /**
         * \brief Invalidate current context
         * \details Invalidation happens on various events, i.e. surface resize. This action drops created resources
         * which are strictly depends on swapchain.
         */
        virtual void invalidate() = 0;
    };
}

#endif // PENROSE_RENDERING_RENDER_CONTEXT_HPP
