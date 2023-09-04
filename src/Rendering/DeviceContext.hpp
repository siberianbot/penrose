#ifndef PENROSE_RENDERING_DEVICE_CONTEXT_HPP
#define PENROSE_RENDERING_DEVICE_CONTEXT_HPP

#include <cstdint>
#include <optional>

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/SurfaceManager.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/Builtin/Vulkan/VkSurfaceProvider.hpp"
#include "src/Builtin/Vulkan/VulkanBackend.hpp"
#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkPhysicalDeviceContext.hpp"

namespace Penrose {

    class ResourceSet;

    // TODO: split
    class DeviceContext : public Resource, public Initializable {
    private:
        Lazy<VulkanBackend> _vulkanBackend;
        Lazy<VkSurfaceProvider> _vkSurfaceProvider;
        Lazy<SurfaceManager> _surfaceManager;

        Lazy<VkLogicalDeviceContext> _logicalDeviceContext;
        Lazy<VkPhysicalDeviceContext> _physicalDeviceContext;

        std::optional<vk::DescriptorPool> _descriptorPool;

        vk::DescriptorPool createDescriptorPool();

    public:
        explicit DeviceContext(ResourceSet *resources);
        ~DeviceContext() override = default;

        void init() override;
        void destroy() override;

        [[nodiscard]] vk::DescriptorPool &getDescriptorPool() { return this->_descriptorPool.value(); }
    };
}

#endif // PENROSE_RENDERING_DEVICE_CONTEXT_HPP
