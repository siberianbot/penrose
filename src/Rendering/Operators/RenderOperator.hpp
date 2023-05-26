#ifndef PENROSE_RENDERING_OPERATORS_RENDER_OPERATION_HPP
#define PENROSE_RENDERING_OPERATORS_RENDER_OPERATION_HPP

#include <vulkan/vulkan.hpp>

namespace Penrose {

    class RenderOperator {
    public:
        virtual ~RenderOperator() = default;

        virtual void execute(const vk::CommandBuffer &commandBuffer) = 0;
    };
}

#endif // PENROSE_RENDERING_OPERATORS_RENDER_OPERATION_HPP
