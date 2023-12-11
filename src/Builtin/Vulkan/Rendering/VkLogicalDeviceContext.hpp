#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_LOGICAL_DEVICE_CONTEXT_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_LOGICAL_DEVICE_CONTEXT_HPP

#include <optional>

#include <vulkan/vulkan.hpp>

#include <Penrose/Common/Log.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Rendering/VkPhysicalDeviceContext.hpp"

namespace Penrose {

    class VkLogicalDeviceContext final: public Resource<VkLogicalDeviceContext> {
    public:
        explicit VkLogicalDeviceContext(const ResourceSet *resources);
        ~VkLogicalDeviceContext() override = default;

        void init();
        void destroy();

        [[nodiscard]] const vk::Device &getHandle() const { return this->_state->handle; }

        [[nodiscard]] const vk::Queue &getGraphicsQueue() const { return this->_state->graphicsQueue; }

        [[nodiscard]] const vk::Queue &getTransferQueue() const { return this->_state->transferQueue; }

        [[nodiscard]] const vk::Queue &getPresentQueue() const { return this->_state->presentQueue; }

    private:
        struct LogicalDeviceState {
            vk::Device handle;
            vk::Queue graphicsQueue;
            vk::Queue transferQueue;
            vk::Queue presentQueue;
        };

        ResourceProxy<Log> _log;
        ResourceProxy<VkPhysicalDeviceContext> _physicalDeviceContext;

        std::optional<LogicalDeviceState> _state;

        [[nodiscard]] LogicalDeviceState makeLogicalDevice();
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_LOGICAL_DEVICE_CONTEXT_HPP
