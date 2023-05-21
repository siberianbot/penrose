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
            vk::Extent2D extent;
            vk::Format format;
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

        [[nodiscard]] const vk::Extent2D &getSwapchainExtent() const { return this->_swapchain.value().extent; }

        [[nodiscard]] const vk::Format &getSwapchainFormat() const { return this->_swapchain.value().format; }

        [[nodiscard]] const std::vector<vk::Image> &getSwapchainImages() const {
            return this->_swapchain.value().images;
        }

        [[nodiscard]] const std::vector<vk::ImageView> &getSwapchainImageViews() const {
            return this->_swapchain.value().imageViews;
        }
    };
}

#endif // PENROSE_RENDERING_PRESENT_CONTEXT_HPP
