#ifndef PENROSE_RENDERING_COMMAND_RECORDER_HPP
#define PENROSE_RENDERING_COMMAND_RECORDER_HPP

#include <Penrose/Rendering/Objects/DrawInfo.hpp>
#include <Penrose/Rendering/Objects/Pipeline.hpp>
#include <Penrose/Rendering/Objects/PipelineBindingInfo.hpp>
#include <Penrose/Types/Rect.hpp>

namespace Penrose {

    /**
     * \brief Command recorder interface
     * \details Command recorder is used to record all rendering commands within renderer execution which will be
     * submitted by renderer context
     */
    class PENROSE_API CommandRecorder {
    public:
        virtual ~CommandRecorder() = default;

        /**
         * \brief Configure viewport rectangle
         * \param rect Relative viewport rectangle
         */
        virtual void configureViewport(FloatRect rect) = 0;

        /**
         * \brief Configure scissor rectangle
         * \param rect Relative scissor rectangle
         */
        virtual void configureScissor(FloatRect rect) = 0;

        /**
         * \brief Bind pipeline with required objects and constants to bind
         * \param pipeline Pointer to instance of pipeline
         * \param bindingInfo Binding requirements
         */
        virtual void bindPipeline(Pipeline *pipeline, const PipelineBindingInfo &bindingInfo) = 0;

        /**
         * \brief Perform drawing
         * \param drawInfo Drawing operation
         */
        virtual void draw(const DrawInfo &drawInfo) = 0;
    };
}

#endif // PENROSE_RENDERING_COMMAND_RECORDER_HPP
