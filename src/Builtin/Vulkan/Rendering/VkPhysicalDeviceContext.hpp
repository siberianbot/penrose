#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_PHYSICAL_DEVICE_CONTEXT_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_PHYSICAL_DEVICE_CONTEXT_HPP

#include <cstdint>
#include <optional>

#include <vulkan/vulkan.hpp>

#include <Penrose/Core/Log.hpp>
#include <Penrose/Rendering/SurfaceManager.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/Builtin/Vulkan/VkSurfaceProvider.hpp"
#include "src/Builtin/Vulkan/VulkanBackend.hpp"

namespace Penrose {

    class ResourceSet;

    class VkPhysicalDeviceContext : public Resource, public Initializable {
    public:
        explicit VkPhysicalDeviceContext(ResourceSet *resources);
        ~VkPhysicalDeviceContext() override = default;

        void init() override;
        void destroy() override;

        [[nodiscard]] const vk::PhysicalDevice &getHandle() const { return this->_state->handle; }

        [[nodiscard]] const vk::PhysicalDeviceFeatures &getFeatures() const { return this->_state->features; }

        [[nodiscard]] const vk::PhysicalDeviceProperties &getProperties() const { return this->_state->properties; }

        [[nodiscard]] const std::uint32_t &getGraphicsFamilyIdx() const { return this->_state->graphicsFamilyIdx; }

        [[nodiscard]] const std::uint32_t &getTransferFamilyIdx() const { return this->_state->transferFamilyIdx; }

        [[nodiscard]] const std::uint32_t &getPresentFamilyIdx() const { return this->_state->presentFamilyIdx; }

    private:
        struct State {
            vk::PhysicalDevice handle;
            vk::PhysicalDeviceFeatures features;
            vk::PhysicalDeviceProperties properties;
            std::uint32_t graphicsFamilyIdx;
            std::uint32_t transferFamilyIdx;
            std::uint32_t presentFamilyIdx;
        };

        Lazy<Log> _log;
        Lazy<SurfaceManager> _surfaceManager;
        Lazy<VkSurfaceProvider> _vkSurfaceProvider;
        Lazy<VulkanBackend> _vulkanBackend;

        std::optional<State> _state;

        [[nodiscard]] State selectPhysicalDevice();
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_PHYSICAL_DEVICE_CONTEXT_HPP
