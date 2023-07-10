#ifndef PENROSE_RENDERING_RENDER_OPERATOR_HPP
#define PENROSE_RENDERING_RENDER_OPERATOR_HPP

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>

#include <vulkan/vulkan.hpp>

#include <Penrose/Common/ParamsCollection.hpp>

namespace Penrose {

    class ResourceSet;

    struct RenderOperatorCreateContext {
        ResourceSet *resources;
        ParamsCollection params;
        vk::RenderPass renderPass;
        std::uint32_t subpassIdx;
    };

    struct RenderOperatorExecutionContext {
        std::uint32_t frameIdx;
        vk::Rect2D renderArea;
        vk::CommandBuffer commandBuffer;
    };

    class RenderOperator {
    public:
        virtual ~RenderOperator() = default;

        virtual void execute(const RenderOperatorExecutionContext &context) = 0;
    };

    template<typename T>
    concept IsRenderOperator = std::is_base_of<RenderOperator, T>::value &&
                               requires(T, const RenderOperatorCreateContext &createContext) {
                                   { T::name() } -> std::same_as<std::string>;
                                   { T::create(createContext) } -> std::same_as<std::unique_ptr<RenderOperator>>;
                               };

    template<typename T>
    concept IsRenderOperatorWithDefaults = IsRenderOperator<T> && requires(T) {
        { T::defaults() } -> std::same_as<ParamsCollection>;
    };
}

#endif // PENROSE_RENDERING_RENDER_OPERATOR_HPP
