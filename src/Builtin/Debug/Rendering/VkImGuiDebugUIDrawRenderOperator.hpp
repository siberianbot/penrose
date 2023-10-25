#ifndef PENROSE_BUILTIN_DEBUG_RENDERING_VK_IMGUI_DEBUG_UI_DRAW_RENDER_OPERATOR_HPP
#define PENROSE_BUILTIN_DEBUG_RENDERING_VK_IMGUI_DEBUG_UI_DRAW_RENDER_OPERATOR_HPP

#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include <Penrose/Builtin/Debug/Rendering/DebugUIDrawRenderOperator.hpp>
#include <Penrose/Builtin/Debug/UI/UIContext.hpp>

#include "src/Builtin/Debug/UI/UIDrawVisitor.hpp"

#include "src/Builtin/Vulkan/VulkanBackend.hpp"
#include "src/Builtin/Vulkan/Rendering/VkCommandManager.hpp"
#include "src/Builtin/Vulkan/Rendering/VkDescriptorPoolManager.hpp"
#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkPhysicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSwapchainManager.hpp"

namespace Penrose {

    class RenderSubgraph;

    class VkImGuiDebugUIDrawRenderOperator
            : public Resource<VkImGuiDebugUIDrawRenderOperator, ResourceGroup::RenderOperator>,
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

        ResourceProxy<VulkanBackend> _vulkanBackend;
        ResourceProxy<VkCommandManager> _commandManager;
        ResourceProxy<VkDescriptorPoolManager> _descriptorPoolManager;
        ResourceProxy<VkLogicalDeviceContext> _logicalDeviceContext;
        ResourceProxy<VkPhysicalDeviceContext> _physicalDeviceContext;
        ResourceProxy<VkSwapchainManager> _swapchainManager;
        ResourceProxy<UIContext> _uiContext;
        ResourceProxy<UIDrawVisitor> _uiDrawVisitor;

        std::optional<State> _state;

        void initFor(const RenderOperator::Context &context);
    };
}

#endif // PENROSE_BUILTIN_DEBUG_RENDERING_VK_IMGUI_DEBUG_UI_DRAW_RENDER_OPERATOR_HPP
