#ifndef PENROSE_RENDERING_PIPELINE_HPP
#define PENROSE_RENDERING_PIPELINE_HPP

#include <set>

#include <Penrose/ECS/Entity.hpp>
#include <Penrose/Rendering/DescriptorBindingValue.hpp>
#include <Penrose/Rendering/PipelineInfo.hpp>

namespace Penrose {

    class Descriptor;

    class Pipeline {
    public:
        virtual ~Pipeline() = default;

        [[nodiscard]] virtual const PipelineInfo &getPipelineInfo() const = 0;

        [[nodiscard]] virtual Descriptor *getDescriptorFor(const Entity &entity,
                                                           const std::set<DescriptorBindingValue> &values) = 0;
    };
}

#endif // PENROSE_RENDERING_PIPELINE_HPP
