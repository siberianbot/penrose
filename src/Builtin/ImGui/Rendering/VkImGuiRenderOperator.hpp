#ifndef PENROSE_BUILTIN_IMGUI_RENDERING_VK_IMGUI_RENDER_OPERATOR_HPP
#define PENROSE_BUILTIN_IMGUI_RENDERING_VK_IMGUI_RENDER_OPERATOR_HPP

#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/UI/UIManager.hpp>

#include <Penrose/Builtin/ImGui/Rendering/ImGuiRenderOperator.hpp>

#include "src/Builtin/Vulkan/VulkanBackend.hpp"
#include "src/Builtin/Vulkan/Rendering/VkCommandManager.hpp"
#include "src/Builtin/Vulkan/Rendering/VkDescriptorPoolManager.hpp"
#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkPhysicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSwapchainManager.hpp"

#include "src/Builtin/ImGui/UI/ImGuiUIInstanceVisitor.hpp"

namespace Penrose {

    class VkImGuiRenderOperator : public Resource<VkImGuiRenderOperator, ResourceGroup::RenderOperator>,
                                  public Initializable,
                                  public ImGuiRenderOperator {
    public:
        explicit VkImGuiRenderOperator(ResourceSet *resources);
        ~VkImGuiRenderOperator() override = default;

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
        ResourceProxy<UIManager> _uiManager;
        ResourceProxy<ImGuiUIInstanceVisitor> _uiInstanceVisitor;

        std::optional<State> _state;

        void initFor(const RenderOperator::Context &context);
    };
}

#endif // PENROSE_BUILTIN_IMGUI_RENDERING_VK_IMGUI_RENDER_OPERATOR_HPP
