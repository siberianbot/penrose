#ifndef PENROSE_RENDERING_PIPELINE_HPP
#define PENROSE_RENDERING_PIPELINE_HPP

#include <Penrose/ECS/Entity.hpp>
#include <Penrose/Rendering/PipelineInfo.hpp>

namespace Penrose {

    class Descriptor;

    class Pipeline {
    public:
        virtual ~Pipeline() = default;

        [[nodiscard]] virtual const PipelineInfo &getPipelineInfo() const = 0;

        [[nodiscard]] virtual Descriptor *allocateDescriptor() = 0;
    };
}

#endif // PENROSE_RENDERING_PIPELINE_HPP
