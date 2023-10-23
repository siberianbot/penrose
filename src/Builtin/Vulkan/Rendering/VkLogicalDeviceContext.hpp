#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_LOGICAL_DEVICE_CONTEXT_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_LOGICAL_DEVICE_CONTEXT_HPP

#include <optional>

#include <vulkan/vulkan.hpp>

#include <Penrose/Common/Log.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Rendering/VkPhysicalDeviceContext.hpp"

namespace Penrose {

    class VkLogicalDeviceContext : public Resource<VkLogicalDeviceContext>,
                                   public Initializable {
    public:
        explicit VkLogicalDeviceContext(ResourceSet *resources);
        ~VkLogicalDeviceContext() override = default;

        void init() override;
        void destroy() override;

        [[nodiscard]] const vk::Device &getHandle() const { return this->_state->handle; }

        [[nodiscard]] const vk::Queue &getGraphicsQueue() const { return this->_state->graphicsQueue; }

        [[nodiscard]] const vk::Queue &getTransferQueue() const { return this->_state->transferQueue; }

        [[nodiscard]] const vk::Queue &getPresentQueue() const { return this->_state->presentQueue; }

    private:
        struct State {
            vk::Device handle;
            vk::Queue graphicsQueue;
            vk::Queue transferQueue;
            vk::Queue presentQueue;
        };

        ResourceProxy<Log> _log;
        ResourceProxy<VkPhysicalDeviceContext> _physicalDeviceContext;

        std::optional<State> _state;

        [[nodiscard]] State createLogicalDevice();
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_LOGICAL_DEVICE_CONTEXT_HPP
