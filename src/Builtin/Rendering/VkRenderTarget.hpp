#ifndef PENROSE_BUILTIN_RENDERING_VK_RENDER_TARGET_HPP
#define PENROSE_BUILTIN_RENDERING_VK_RENDER_TARGET_HPP

#include <cstdint>
#include <optional>

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/RenderGraph.hpp>

namespace Penrose {

    class DeviceContext;
    class PresentContext;

    class VkRenderTarget {
    public:
        explicit VkRenderTarget(RenderTargetInfo target);
        virtual ~VkRenderTarget() = default;

        [[nodiscard]] virtual const vk::ImageView &getView(std::uint32_t imageIdx) const = 0;

        [[nodiscard]] const RenderTargetInfo &getTarget() const { return this->_target; }

        virtual void create() = 0;
        virtual void destroy() = 0;

    protected:
        RenderTargetInfo _target;
    };

    class VkSwapchainRenderTarget : public VkRenderTarget {
    public:
        explicit VkSwapchainRenderTarget(RenderTargetInfo target,
                                         PresentContext *presentContext);
        ~VkSwapchainRenderTarget() override = default;

        [[nodiscard]] const vk::ImageView &getView(std::uint32_t imageIdx) const override;

        void create() override { /* nothing to do */ }

        void destroy() override { /* nothing to do */}

    private:
        PresentContext *_presentContext;
    };

    class VkImageRenderTarget : public VkRenderTarget {
    public:
        VkImageRenderTarget(RenderTargetInfo target,
                            DeviceContext *deviceContext,
                            PresentContext *presentContext);
        ~VkImageRenderTarget() override = default;

        [[nodiscard]] const vk::ImageView &getView(std::uint32_t) const override {
            return this->_state->imageView;
        }

        void create() override;
        void destroy() override;

    private:
        struct State {
            vk::Image image;
            vk::DeviceMemory imageMemory;
            vk::ImageView imageView;
        };

        DeviceContext *_deviceContext;
        PresentContext *_presentContext;

        std::optional<State> _state;
    };

    [[nodiscard]] VkRenderTarget *makeVkRenderTarget(DeviceContext *deviceContext,
                                                     PresentContext *presentContext,
                                                     const RenderTargetInfo &target);
}

#endif // PENROSE_BUILTIN_RENDERING_VK_RENDER_TARGET_HPP
