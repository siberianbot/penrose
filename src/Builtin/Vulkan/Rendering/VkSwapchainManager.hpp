#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_SWAPCHAIN_MANAGER_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_SWAPCHAIN_MANAGER_HPP

#include <memory>
#include <mutex>
#include <optional>

#include <Penrose/Core/Log.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/Builtin/Vulkan/Rendering/VkSwapchainFactory.hpp"

namespace Penrose {

    class ResourceSet;

    class VkSwapchainManager : public Resource, public Initializable {
    public:
        explicit VkSwapchainManager(ResourceSet *resources);
        ~VkSwapchainManager() override = default;

        void init() override;
        void destroy() override;

        void recreate();

        [[nodiscard]] std::shared_ptr<VkSwapchain> getSwapchain();

    private:
        Lazy<Log> _log;
        Lazy<VkSwapchainFactory> _swapchainFactory;

        std::mutex _mutex;
        std::optional<std::shared_ptr<VkSwapchain>> _swapchain;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_SWAPCHAIN_MANAGER_HPP