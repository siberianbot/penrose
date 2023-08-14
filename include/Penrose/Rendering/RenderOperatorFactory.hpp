#ifndef PENROSE_RENDERING_RENDER_OPERATOR_FACTORY_HPP
#define PENROSE_RENDERING_RENDER_OPERATOR_FACTORY_HPP

#include <cstdint>
#include <string_view>

#include <utility>
#include <vulkan/vulkan.hpp>

#include <Penrose/Common/ParamsCollection.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class RenderOperator;

    class RenderOperatorFactory : public Resource {
    public:
        struct Context {
            ParamsCollection params;
            vk::RenderPass renderPass;
            std::uint32_t subpassIdx;
        };

        ~RenderOperatorFactory() override = default;

        [[nodiscard]] virtual std::string_view name() const = 0;

        [[nodiscard]] virtual ParamsCollection defaults() const = 0;

        [[nodiscard]] virtual RenderOperator *create(const Context &context) const = 0;
    };
}

#endif // PENROSE_RENDERING_RENDER_OPERATOR_FACTORY_HPP
