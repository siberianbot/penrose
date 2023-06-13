#ifndef PENROSE_RENDERING_OPERATORS_RENDER_OPERATOR_HPP
#define PENROSE_RENDERING_OPERATORS_RENDER_OPERATOR_HPP

#include <cstdint>

#include <vulkan/vulkan.hpp>

namespace Penrose {

    struct RenderOperatorExecutionContext {
        vk::RenderPass renderPass;
        vk::Rect2D renderArea;
        vk::CommandBuffer commandBuffer;
    };

    class RenderOperator {
    public:
        virtual ~RenderOperator() = default;

        virtual void execute(const RenderOperatorExecutionContext &context) = 0;
    };
}

#endif // PENROSE_RENDERING_OPERATORS_RENDER_OPERATOR_HPP
