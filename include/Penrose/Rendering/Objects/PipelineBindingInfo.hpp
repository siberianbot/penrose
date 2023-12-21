#ifndef PENROSE_RENDERING_OBJECTS_PIPELINE_BINDING_INFO_HPP
#define PENROSE_RENDERING_OBJECTS_PIPELINE_BINDING_INFO_HPP

#include <optional>
#include <string>
#include <vector>

#include <Penrose/Rendering/Objects/Buffer.hpp>
#include <Penrose/Rendering/Objects/Image.hpp>
#include <Penrose/Rendering/Objects/Sampler.hpp>

namespace Penrose {

    /**
     * \brief Pipeline constant binding definition
     */
    struct PENROSE_API PipelineConstantBinding {

        /**
         * \brief Pointer to constant data
         */
        void *data;

        friend auto operator<=>(const PipelineConstantBinding &, const PipelineConstantBinding &) = default;
    };

    /**
     * \brief Pipeline object binding definition
     */
    struct PENROSE_API PipelineObjectBinding {

        /**
         * \brief List of buffers to bind
         */
        std::vector<Buffer *> buffers;

        /**
         * \brief List of images to bind
         */
        std::vector<Image *> images;

        /**
         * \brief List of samplers to bind
         */
        std::vector<Sampler *> samplers;

        friend auto operator<=>(const PipelineObjectBinding &, const PipelineObjectBinding &) = default;
    };

    /**
     * \brief Pipeline binding requirements definition
     */
    struct PENROSE_API PipelineBindingInfo {

        /**
         * \brief Optional tag for object binding re-usage
         */
        std::optional<std::string> tag;

        /**
         * \brief List of constants to bind
         */
        std::vector<PipelineConstantBinding> constants;

        /**
         * \brief List of objects to bind
         */
        std::vector<PipelineObjectBinding> objects;

        friend auto operator<=>(const PipelineBindingInfo &, const PipelineBindingInfo &) = default;
    };
}

#endif // PENROSE_RENDERING_OBJECTS_PIPELINE_BINDING_INFO_HPP
