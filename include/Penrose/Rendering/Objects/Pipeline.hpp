#ifndef PENROSE_RENDERING_OBJECTS_PIPELINE_HPP
#define PENROSE_RENDERING_OBJECTS_PIPELINE_HPP

#include <Penrose/Rendering/Objects/PipelineInfo.hpp>

namespace Penrose {

    /**
     * \brief Pipeline object
     * \details Pipeline objects defines rendering pipeline.
     */
    class PENROSE_API Pipeline {
    public:
        virtual ~Pipeline() = default;

        /**
         * \brief Get pipeline definition
         * \return Pipeline definition
         */
        [[nodiscard]] virtual const PipelineInfo &getPipelineInfo() const = 0;
    };
}

#endif // PENROSE_RENDERING_OBJECTS_PIPELINE_HPP
