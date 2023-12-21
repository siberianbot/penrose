#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_SYSTEM_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_SYSTEM_HPP

#include <optional>

#include <Penrose/Common/Log.hpp>
#include <Penrose/Rendering/RenderSystem.hpp>
#include <Penrose/Rendering/SurfaceManager.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Rendering/Objects/VkBufferFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkImageFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkLogicalDevice.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkLogicalDeviceFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkLogicalDeviceProvider.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkPhysicalDevice.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkPhysicalDeviceProvider.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkPhysicalDeviceSelector.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkPipelineFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkSwapchainFactory.hpp"

namespace Penrose {

    class VkRenderSystem final: public Resource<VkRenderSystem>,
                                public RenderSystem,
                                public VkLogicalDeviceProvider,
                                public VkPhysicalDeviceProvider {
    public:
        explicit VkRenderSystem(const ResourceSet *resources);
        ~VkRenderSystem() override = default;

        [[nodiscard]] std::string getName() const override { return "Vulkan"; }

        void init() override;
        void destroy() override;

        [[nodiscard]] RenderContext *makeRenderContext() override;

        [[nodiscard]] const VkPhysicalDevice &getPhysicalDevice() const override { return *this->_physicalDevice; }

        [[nodiscard]] const VkLogicalDevice &getLogicalDevice() const override { return *this->_logicalDevice; }

    private:
        const ResourceSet *_resources;

        ResourceProxy<Log> _log;
        ResourceProxy<SurfaceManager> _surfaceManager;
        ResourceProxy<VkPhysicalDeviceSelector> _physicalDeviceSelector;
        ResourceProxy<VkLogicalDeviceFactory> _logicalDeviceFactory;
        ResourceProxy<VkBufferFactory> _bufferFactory;
        ResourceProxy<VkImageFactory> _imageFactory;
        ResourceProxy<VkPipelineFactory> _pipelineFactory;
        ResourceProxy<VkSwapchainFactory> _swapchainFactory;

        std::optional<VkPhysicalDevice> _physicalDevice;
        std::optional<VkLogicalDevice> _logicalDevice;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_SYSTEM_HPP
