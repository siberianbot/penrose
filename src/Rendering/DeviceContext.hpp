#ifndef PENROSE_RENDERING_DEVICE_CONTEXT_HPP
#define PENROSE_RENDERING_DEVICE_CONTEXT_HPP

#include <cstdint>
#include <optional>

#include <vulkan/vulkan.hpp>

#include "src/Common/Initializable.hpp"
#include "src/Resources/Resource.hpp"

namespace Penrose {

    class ResourceSet;
    class VulkanBackend;
    class Surface;

    class DeviceContext : public Resource, public Initializable {
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

        VulkanBackend *_vulkanBackend;
        Surface *_surface;

        std::optional<PhysicalDeviceProxy> _physicalDevice;
        std::optional<LogicalDeviceProxy> _logicalDevice;
        std::optional<vk::CommandPool> _commandPool;
        std::optional<vk::DescriptorPool> _descriptorPool;

        PhysicalDeviceProxy selectPhysicalDevice();
        LogicalDeviceProxy createLogicalDevice();
        vk::CommandPool createCommandPool();
        vk::DescriptorPool createDescriptorPool();

    public:
        explicit DeviceContext(ResourceSet *resources);
        ~DeviceContext() override = default;

        void init() override;
        void destroy() override;

        [[nodiscard]] vk::PhysicalDevice &getPhysicalDevice() { return this->_physicalDevice.value().handle; }

        [[nodiscard]] std::uint32_t getGraphicsQueueFamily() {
            return this->_physicalDevice.value().graphicsQueueFamilyIdx;
        }

        [[nodiscard]] std::uint32_t getPresentQueueFamily() {
            return this->_physicalDevice.value().presentQueueFamilyIdx;
        }

        [[nodiscard]] vk::Device &getLogicalDevice() { return this->_logicalDevice.value().handle; }

        [[nodiscard]] vk::Queue &getGraphicsQueue() { return this->_logicalDevice.value().graphicsQueue; }

        [[nodiscard]] vk::Queue &getPresentQueue() { return this->_logicalDevice.value().presentQueue; }

        [[nodiscard]] vk::CommandPool &getCommandPool() { return this->_commandPool.value(); }

        [[nodiscard]] vk::DescriptorPool &getDescriptorPool() { return this->_descriptorPool.value(); }
    };
}

#endif // PENROSE_RENDERING_DEVICE_CONTEXT_HPP
