#ifndef PENROSE_RENDERING_RENDER_OPERATOR_HPP
#define PENROSE_RENDERING_RENDER_OPERATOR_HPP

#include <cstdint>

#include <vulkan/vulkan.hpp>

namespace Penrose {

    class RenderOperator {
    public:
        struct Context {
            std::uint32_t frameIdx;
            vk::Rect2D renderArea;
            vk::CommandBuffer commandBuffer;
        };

        virtual ~RenderOperator() = default;

        virtual void execute(const Context &context) = 0;
    };
}

#endif // PENROSE_RENDERING_RENDER_OPERATOR_HPP
