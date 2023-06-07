#ifndef PENROSE_RENDERING_OPERATORS_RENDER_OPERATION_HPP
#define PENROSE_RENDERING_OPERATORS_RENDER_OPERATION_HPP

#include <cstdint>
#include <memory>

#include <vulkan/vulkan.hpp>

namespace Penrose {

    class ResourceSet;

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

    struct RenderOperatorProduceContext {
        ResourceSet *resources;
        vk::RenderPass renderPass;
        std::uint32_t subpassIdx;
    };

    class RenderOperatorProducer {
    public:
        virtual ~RenderOperatorProducer() = default;

        [[nodiscard]] virtual std::unique_ptr<RenderOperator> produce(
                const RenderOperatorProduceContext &context) const = 0;
    };
}

#endif // PENROSE_RENDERING_OPERATORS_RENDER_OPERATION_HPP
