#ifndef PENROSE_BUILTIN_DEBUG_RENDERING_VK_IMGUI_DEBUG_UI_DRAW_RENDER_OPERATOR_HPP
#define PENROSE_BUILTIN_DEBUG_RENDERING_VK_IMGUI_DEBUG_UI_DRAW_RENDER_OPERATOR_HPP

#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

#include <Penrose/Builtin/Debug/Rendering/DebugUIDrawRenderOperator.hpp>

#include "src/Builtin/Vulkan/VulkanBackend.hpp"
#include "src/Builtin/Vulkan/Rendering/VkCommandManager.hpp"
#include "src/Builtin/Vulkan/Rendering/VkDescriptorPoolManager.hpp"
#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkPhysicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSwapchainManager.hpp"

namespace Penrose {

    class ResourceSet;
    class RenderSubgraph;

    class VkImGuiDebugUIDrawRenderOperator : public Resource,
                                             public Initializable,
                                             public DebugUIDrawRenderOperator {
    public:
        explicit VkImGuiDebugUIDrawRenderOperator(ResourceSet *resources);
        ~VkImGuiDebugUIDrawRenderOperator() override = default;

        void init() override { /* nothing to do */ }

        void destroy() override;

        void execute(CommandRecording *commandRecording, const Context &context) override;

    private:
        struct State {
            RenderSubgraph *subgraph;
            std::uint32_t subgraphPassIdx;
        };

        Lazy<VulkanBackend> _vulkanBackend;
        Lazy<VkCommandManager> _commandManager;
        Lazy<VkDescriptorPoolManager> _descriptorPoolManager;
        Lazy<VkLogicalDeviceContext> _logicalDeviceContext;
        Lazy<VkPhysicalDeviceContext> _physicalDeviceContext;
        Lazy<VkSwapchainManager> _swapchainManager;

        std::optional<State> _state;

        void initFor(const RenderOperator::Context &context);
    };
}

#endif // PENROSE_BUILTIN_DEBUG_RENDERING_VK_IMGUI_DEBUG_UI_DRAW_RENDER_OPERATOR_HPP
