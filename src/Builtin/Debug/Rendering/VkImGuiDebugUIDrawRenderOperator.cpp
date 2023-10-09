#include "VkImGuiDebugUIDrawRenderOperator.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Rendering/VkCommandRecording.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderSubgraph.hpp"

namespace Penrose {

    VkImGuiDebugUIDrawRenderOperator::VkImGuiDebugUIDrawRenderOperator(ResourceSet *resources)
            : _vulkanBackend(resources->getLazy<VulkanBackend>()),
              _commandManager(resources->getLazy<VkCommandManager>()),
              _descriptorPoolManager(resources->getLazy<VkDescriptorPoolManager>()),
              _logicalDeviceContext(resources->getLazy<VkLogicalDeviceContext>()),
              _physicalDeviceContext(resources->getLazy<VkPhysicalDeviceContext>()),
              _swapchainManager(resources->getLazy<VkSwapchainManager>()),
              _uiContext(resources->getLazy<UIContext>()),
              _uiDrawVisitor(resources->getLazy<UIDrawVisitor>()) {
        //
    }

    void VkImGuiDebugUIDrawRenderOperator::destroy() {
        if (!this->_state.has_value()) {
            return;
        }

        ImGui_ImplVulkan_Shutdown();

        this->_state = std::nullopt;
    }

    void VkImGuiDebugUIDrawRenderOperator::execute(CommandRecording *commandRecording,
                                                   const RenderOperator::Context &context) {

        if (this->_state.has_value() && (this->_state->subgraph != context.subgraph ||
                                         this->_state->subgraphPassIdx != context.subgraphPassIdx)) {
            throw EngineError("ImGui draw operator was bound to {:#x}/pass {}",
                              reinterpret_cast<std::size_t>(this->_state->subgraph), this->_state->subgraphPassIdx);
        }

        if (!this->_state.has_value()) {
            this->initFor(context);
        }

        auto commandBuffer = dynamic_cast<VkCommandRecording *>(commandRecording)->getCommandBuffer();

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        for (const auto &[_, root]: this->_uiContext->getRoots()) {
            this->_uiDrawVisitor->visit(root);
        }

        ImGui::Render();
        ImDrawData *drawData = ImGui::GetDrawData();
        ImGui_ImplVulkan_RenderDrawData(drawData, commandBuffer);
    }

    void VkImGuiDebugUIDrawRenderOperator::initFor(const RenderOperator::Context &context) {
        auto imageCount = this->_swapchainManager->getSwapchain()->getImageCount();

        auto initInfo = ImGui_ImplVulkan_InitInfo{
                .Instance = this->_vulkanBackend->getInstance(),
                .PhysicalDevice = this->_physicalDeviceContext->getHandle(),
                .Device = this->_logicalDeviceContext->getHandle(),
                .QueueFamily = this->_physicalDeviceContext->getGraphicsFamilyIdx(),
                .Queue = this->_logicalDeviceContext->getGraphicsQueue(),
                .PipelineCache = nullptr,
                .DescriptorPool = this->_descriptorPoolManager->getDescriptorPool(),
                .Subpass = context.subgraphPassIdx,
                .MinImageCount = imageCount,
                .ImageCount = imageCount,
                .MSAASamples = VK_SAMPLE_COUNT_1_BIT,
                .Allocator = nullptr,
                .CheckVkResultFn = [](VkResult result) {
                    if (result != VK_SUCCESS) {
                        vk::detail::throwResultException((vk::Result) result, "Vulkan assertion failed");
                    }
                }
        };

        ImGui_ImplVulkan_Init(&initInfo, dynamic_cast<VkRenderSubgraph *>(context.subgraph)->getRenderPass());

        this->_commandManager->executeGraphicsOnce([](vk::CommandBuffer &commandBuffer) {
            ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
        });

        ImGui_ImplVulkan_DestroyFontUploadObjects();

        this->_state = {context.subgraph, context.subgraphPassIdx};
    }
}
