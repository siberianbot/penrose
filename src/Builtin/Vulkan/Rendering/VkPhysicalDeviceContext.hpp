#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_PHYSICAL_DEVICE_CONTEXT_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_PHYSICAL_DEVICE_CONTEXT_HPP

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include <vulkan/vulkan.hpp>

#include <Penrose/Common/Log.hpp>
#include <Penrose/Rendering/SurfaceManager.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include <Penrose/Builtin/Vulkan/VkSurfaceProvider.hpp>

#include "src/Builtin/Vulkan/VulkanBackend.hpp"

namespace Penrose {

    class VkPhysicalDeviceContext final: public Resource<VkPhysicalDeviceContext> {
    public:
        explicit VkPhysicalDeviceContext(const ResourceSet *resources);
        ~VkPhysicalDeviceContext() override = default;

        void init();
        void destroy();

        [[nodiscard]] const vk::PhysicalDevice &getHandle() const { return this->_device->handle; }

        [[nodiscard]] const vk::PhysicalDeviceFeatures &getFeatures() const { return this->_device->features; }

        [[nodiscard]] const vk::PhysicalDeviceProperties &getProperties() const { return this->_device->properties; }

        [[nodiscard]] const std::uint32_t &getGraphicsFamilyIdx() const { return this->_device->graphicsFamilyIdx; }

        [[nodiscard]] const std::uint32_t &getTransferFamilyIdx() const { return this->_device->transferFamilyIdx; }

        [[nodiscard]] const std::uint32_t &getPresentFamilyIdx() const { return this->_device->presentFamilyIdx; }

        [[nodiscard]] const std::vector<std::string> &getExtensions() const { return this->_device->extensions; }

    private:
        struct PhysicalDeviceInfo {
            vk::PhysicalDevice handle;
            vk::PhysicalDeviceFeatures features;
            vk::PhysicalDeviceProperties properties;
            std::uint32_t graphicsFamilyIdx;
            std::uint32_t transferFamilyIdx;
            std::uint32_t presentFamilyIdx;
            std::vector<std::string> extensions;
            std::uint32_t priority;
        };

        ResourceProxy<Log> _log;
        ResourceProxy<SurfaceManager> _surfaceManager;
        ResourceProxy<VkSurfaceProvider> _vkSurfaceProvider;
        ResourceProxy<VulkanBackend> _vulkanBackend;

        std::optional<PhysicalDeviceInfo> _device;

        [[nodiscard]] PhysicalDeviceInfo selectPhysicalDevice();
        [[nodiscard]] std::optional<PhysicalDeviceInfo> makePhysicalDeviceInfo(
            const vk::PhysicalDevice &device, const vk::SurfaceKHR &surface
        );
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_PHYSICAL_DEVICE_CONTEXT_HPP
