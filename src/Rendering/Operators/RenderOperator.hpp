#ifndef PENROSE_RENDERING_OPERATORS_RENDER_OPERATOR_HPP
#define PENROSE_RENDERING_OPERATORS_RENDER_OPERATOR_HPP

#include <cstdint>
#include <optional>

#include <vulkan/vulkan.hpp>

#include "src/Rendering/RenderData.hpp"

namespace Penrose {

    struct RenderOperatorExecutionContext {
        vk::RenderPass renderPass;
        vk::Rect2D renderArea;
        vk::CommandBuffer commandBuffer;
        std::optional<RenderList> renderList;
    };

    class RenderOperator {
    public:
        virtual ~RenderOperator() = default;

        virtual void execute(const RenderOperatorExecutionContext &context) = 0;
    };
}

#endif // PENROSE_RENDERING_OPERATORS_RENDER_OPERATOR_HPP
