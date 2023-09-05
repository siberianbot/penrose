#include <Penrose/Builtin/Rendering/ImGuiDrawRenderOperator.hpp>

#include <fmt/core.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Rendering/PresentContext.hpp"

#include "src/Builtin/Vulkan/VulkanBackend.hpp"
#include "src/Builtin/Vulkan/Rendering/VkCommandManager.hpp"
#include "src/Builtin/Vulkan/Rendering/VkCommandRecording.hpp"
#include "src/Builtin/Vulkan/Rendering/VkDescriptorPoolManager.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderSubgraph.hpp"

namespace Penrose {

    ImGuiDrawRenderOperator::ImGuiDrawRenderOperator(ResourceSet *resources)
            : _vulkanBackend(resources->get<VulkanBackend>()),
              _presentContext(resources->get<PresentContext>()),
              _commandManager(resources->get<VkCommandManager>()),
              _descriptorPoolManager(resources->get<VkDescriptorPoolManager>()),
              _logicalDeviceContext(resources->get<VkLogicalDeviceContext>()),
              _physicalDeviceContext(resources->get<VkPhysicalDeviceContext>()) {
        //
    }

    void ImGuiDrawRenderOperator::destroy() {
        if (!this->_state.has_value()) {
            return;
        }

        ImGui_ImplVulkan_Shutdown();

        this->_state = std::nullopt;
    }

    void ImGuiDrawRenderOperator::execute(CommandRecording *commandRecording, const RenderOperator::Context &context) {

        if (this->_state.has_value() && (this->_state->subgraph != context.subgraph ||
                                         this->_state->subgraphPassIdx != context.subgraphPassIdx)) {
            throw EngineError(fmt::format("ImGui draw operator was bound to {:#x}/pass {}",
                                          (std::size_t) this->_state->subgraph, this->_state->subgraphPassIdx));
        }

        if (!this->_state.has_value()) {
            this->initFor(context);
        }

        auto commandBuffer = dynamic_cast<VkCommandRecording *>(commandRecording)->getCommandBuffer();

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // TODO: draw here
        ImGui::ShowDemoWindow();

        ImGui::Render();
        ImDrawData *drawData = ImGui::GetDrawData();
        ImGui_ImplVulkan_RenderDrawData(drawData, commandBuffer);
    }

    void ImGuiDrawRenderOperator::initFor(const RenderOperator::Context &context) {
        auto imageCount = static_cast<uint32_t>(this->_presentContext->getSwapchainImages().size());

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
