#ifndef PENROSE_RENDERING_DEVICE_CONTEXT_HPP
#define PENROSE_RENDERING_DEVICE_CONTEXT_HPP

#include <memory>
#include <optional>

#include <vulkan/vulkan.hpp>

#include "src/Core/Resource.hpp"

namespace Penrose {

    class ResourceSet;
    class VulkanBackend;
    class Surface;

    class DeviceContext : public Resource {
    private:
        struct PhysicalDeviceProxy {
            vk::PhysicalDevice handle;
            std::uint32_t graphicsQueueFamilyIdx;
            std::uint32_t presentQueueFamilyIdx;
            vk::PhysicalDeviceFeatures features;
        };

        struct LogicalDeviceProxy {
            vk::Device handle;
            vk::Queue graphicsQueue;
            vk::Queue presentQueue;
        };

        std::shared_ptr<VulkanBackend> _vulkanBackend;
        std::shared_ptr<Surface> _surface;

        std::optional<PhysicalDeviceProxy> _physicalDevice;
        std::optional<LogicalDeviceProxy> _logicalDevice;

        PhysicalDeviceProxy selectPhysicalDevice();
        LogicalDeviceProxy createLogicalDevice();

    public:
        explicit DeviceContext(ResourceSet *resources);
        ~DeviceContext() override = default;

        void init() override;
        void destroy() override;

        [[nodiscard]] vk::PhysicalDevice &getPhysicalDevice() { return this->_physicalDevice.value().handle; }

        [[nodiscard]] vk::Device &getLogicalDevice() { return this->_logicalDevice.value().handle; }
    };
}

#endif // PENROSE_RENDERING_DEVICE_CONTEXT_HPP
