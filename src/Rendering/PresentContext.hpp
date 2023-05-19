#ifndef PENROSE_RENDERING_PRESENT_CONTEXT_HPP
#define PENROSE_RENDERING_PRESENT_CONTEXT_HPP

#include <optional>
#include <thread>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "src/Common/Initializable.hpp"
#include "src/Resources/Resource.hpp"

namespace Penrose {

    class ResourceSet;
    class DeviceContext;
    class Surface;

    class PresentContext : public Resource, public Initializable {
    private:
        struct SwapchainProxy {
            vk::SwapchainKHR handle;
            std::vector<vk::Image> images;
            std::vector<vk::ImageView> imageViews;
        };

        DeviceContext *_deviceContext;
        Surface *_surface;

        std::mutex _swapchainMutex;
        std::optional<SwapchainProxy> _swapchain;

        SwapchainProxy createSwapchain();
        void destroySwapchain(const SwapchainProxy &swapchain);

    public:
        explicit PresentContext(ResourceSet *resources);
        ~PresentContext() override = default;

        void init() override;
        void destroy() override;

        void recreate();

        [[nodiscard]] std::lock_guard<std::mutex> acquireSwapchainLock() {
            return std::lock_guard<std::mutex>(this->_swapchainMutex);
        }

        [[nodiscard]] vk::SwapchainKHR &getSwapchain() { return this->_swapchain.value().handle; }

        [[nodiscard]] const std::vector<vk::Image> &getSwapchainImage() const {
            return this->_swapchain.value().images;
        }
    };
}

#endif // PENROSE_RENDERING_PRESENT_CONTEXT_HPP
