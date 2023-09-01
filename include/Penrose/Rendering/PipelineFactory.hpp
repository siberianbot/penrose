#ifndef PENROSE_RENDERING_PIPELINE_FACTORY_HPP
#define PENROSE_RENDERING_PIPELINE_FACTORY_HPP

#include <cstdint>
#include <string>

#include <Penrose/Rendering/PipelineInfo.hpp>

namespace Penrose {

    class Pipeline;
    class RenderSubgraph;

    class PipelineFactory {
    public:
        virtual ~PipelineFactory() = default;

        virtual void addPipeline(const std::string &pipelineName, const PipelineInfo &pipelineInfo) = 0;

        [[nodiscard]] virtual Pipeline *getPipeline(const std::string &pipelineName,
                                                    const RenderSubgraph *subgraph,
                                                    std::uint32_t passIdx) = 0;
    };
}

#endif // PENROSE_RENDERING_PIPELINE_FACTORY_HPP
