#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_COMMAND_MANAGER_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_COMMAND_MANAGER_HPP

#include <array>
#include <cstdint>
#include <functional>
#include <optional>

#include <vulkan/vulkan.hpp>

#include <Penrose/Core/Log.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/Builtin/Vulkan/Constants.hpp"
#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkPhysicalDeviceContext.hpp"

namespace Penrose {

    class ResourceSet;

    class VkCommandManager : public Resource, public Initializable {
    public:
        using Command = std::function<void(vk::CommandBuffer &commandBuffer)>;

        explicit VkCommandManager(ResourceSet *resources);
        ~VkCommandManager() override = default;

        void init() override;
        void destroy() override;

        [[nodiscard]] vk::CommandBuffer &getGraphicsCommandBuffer(std::uint32_t frameIdx) {
            return this->_state->graphicsCommandBuffers.at(frameIdx);
        }

        void executeGraphicsOnce(const Command &command);
        void executeTransferOnce(const Command &command);

    private:
        struct State {
            vk::CommandPool graphicsCommandPool;
            std::array<vk::CommandBuffer, INFLIGHT_FRAME_COUNT> graphicsCommandBuffers;

            vk::CommandPool transferCommandPool;
        };

        Lazy<Log> _log;
        Lazy<VkLogicalDeviceContext> _logicalDeviceContext;
        Lazy<VkPhysicalDeviceContext> _physicalDeviceContext;

        std::optional<State> _state;

        [[nodiscard]] State createCommandManagerState();

        void executeOnce(const vk::CommandPool &commandPool, const vk::Queue &queue,
                         const Command &command);
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_COMMAND_MANAGER_HPP