#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_SWAPCHAIN_MANAGER_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_SWAPCHAIN_MANAGER_HPP

#include <memory>
#include <mutex>
#include <optional>

#include <Penrose/Common/Log.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Rendering/VkSwapchainFactory.hpp"

namespace Penrose {

    class VkSwapchainManager final: public Resource<VkSwapchainManager> {
    public:
        explicit VkSwapchainManager(const ResourceSet *resources);
        ~VkSwapchainManager() override = default;

        void init();
        void destroy();

        void recreate();

        [[nodiscard]] std::shared_ptr<VkSwapchain> getSwapchain();

    private:
        ResourceProxy<Log> _log;
        ResourceProxy<VkSwapchainFactory> _swapchainFactory;

        std::mutex _mutex;
        std::optional<std::shared_ptr<VkSwapchain>> _swapchain;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_SWAPCHAIN_MANAGER_HPP
