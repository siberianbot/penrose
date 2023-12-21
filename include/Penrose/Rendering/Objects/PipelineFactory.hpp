#ifndef PENROSE_RENDERING_PIPELINE_FACTORY_HPP
#define PENROSE_RENDERING_PIPELINE_FACTORY_HPP

#include <Penrose/Rendering/Objects/Pipeline.hpp>
#include <Penrose/Rendering/Objects/PipelineInfo.hpp>

namespace Penrose {

    /**
     * \brief Pipeline object factory interface
     */
    class PENROSE_API PipelineFactory {
    public:
        virtual ~PipelineFactory() = default;

        /**
         * \brief Create instance of pipeline object
         * \param pipelineInfo Pipeline definition
         * \return Instance of pipeline object
         */
        [[nodiscard]] virtual Pipeline *makePipeline(PipelineInfo &&pipelineInfo) = 0;
    };
}

#endif // PENROSE_RENDERING_PIPELINE_FACTORY_HPP
