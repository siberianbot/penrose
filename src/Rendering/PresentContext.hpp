#ifndef PENROSE_RENDERING_PRESENT_CONTEXT_HPP
#define PENROSE_RENDERING_PRESENT_CONTEXT_HPP

#include <memory>
#include <optional>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "src/Core/Resource.hpp"

namespace Penrose {

    class ResourceSet;
    class DeviceContext;
    class Surface;

    class PresentContext : public Resource {
    private:
        struct SwapchainProxy {
            vk::SwapchainKHR handle;
            std::vector<vk::ImageView> imageViews;
        };

        std::shared_ptr<DeviceContext> _deviceContext;
        std::shared_ptr<Surface> _surface;

        std::optional<SwapchainProxy> _swapchain;

        SwapchainProxy createSwapchain();
        void destroySwapchain(const SwapchainProxy &swapchain);

    public:
        explicit PresentContext(ResourceSet *resources);
        ~PresentContext() override = default;

        void init() override;
        void destroy() override;

        void recreate();
    };
}

#endif // PENROSE_RENDERING_PRESENT_CONTEXT_HPP
