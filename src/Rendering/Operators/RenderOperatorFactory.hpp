#ifndef PENROSE_RENDERING_OPERATORS_RENDER_OPERATOR_FACTORY_HPP
#define PENROSE_RENDERING_OPERATORS_RENDER_OPERATOR_FACTORY_HPP

#include <memory>
#include <optional>
#include <functional>

#include "src/Rendering/RenderGraph.hpp"
#include "src/Rendering/Operators/RenderOperator.hpp"

namespace Penrose {

    class ResourceSet;

    struct RenderOperatorFactoryContext {
        ResourceSet *resources;

        std::optional<RenderOperatorParams> params;

        vk::RenderPass renderPass;
        std::uint32_t subpassIdx;
    };

    using RenderOperatorFactory = std::function<std::unique_ptr<RenderOperator>(const RenderOperatorFactoryContext &)>;
}

#endif // PENROSE_RENDERING_OPERATORS_RENDER_OPERATOR_FACTORY_HPP
