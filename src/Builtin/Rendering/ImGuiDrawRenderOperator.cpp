#include <Penrose/Builtin/Rendering/ImGuiDrawRenderOperator.hpp>

#include <fmt/core.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Rendering/DeviceContext.hpp"
#include "src/Rendering/PresentContext.hpp"

#include "src/Builtin/Backends/VulkanBackend.hpp"
#include "src/Builtin/Rendering/VkCommandRecording.hpp"
#include "src/Builtin/Rendering/VkRenderSubgraph.hpp"

namespace Penrose {

    ImGuiDrawRenderOperator::ImGuiDrawRenderOperator(ResourceSet *resources)
            : _vulkanBackend(resources->get<VulkanBackend>()),
              _deviceContext(resources->get<DeviceContext>()),
              _presentContext(resources->get<PresentContext>()) {
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
        auto logicalDevice = this->_deviceContext->getLogicalDevice();
        auto imageCount = static_cast<uint32_t>(this->_presentContext->getSwapchainImages().size());

        auto initInfo = ImGui_ImplVulkan_InitInfo{
                .Instance = this->_vulkanBackend->getInstance(),
                .PhysicalDevice = this->_deviceContext->getPhysicalDevice(),
                .Device = logicalDevice,
                .QueueFamily = this->_deviceContext->getGraphicsQueueFamily(),
                .Queue = this->_deviceContext->getGraphicsQueue(),
                .PipelineCache = nullptr,
                .DescriptorPool = this->_deviceContext->getDescriptorPool(),
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

        auto allocateInfo = vk::CommandBufferAllocateInfo()
                .setCommandPool(this->_deviceContext->getCommandPool())
                .setCommandBufferCount(1);
        auto commandBuffers = logicalDevice.allocateCommandBuffers(allocateInfo);

        commandBuffers.at(0).begin(vk::CommandBufferBeginInfo());
        ImGui_ImplVulkan_CreateFontsTexture(commandBuffers.at(0));
        commandBuffers.at(0).end();

        auto submit = vk::SubmitInfo()
                .setCommandBuffers(commandBuffers);

        this->_deviceContext->getGraphicsQueue().submit(submit);
        this->_deviceContext->getGraphicsQueue().waitIdle();

        logicalDevice.freeCommandBuffers(this->_deviceContext->getCommandPool(), commandBuffers);

        ImGui_ImplVulkan_DestroyFontUploadObjects();

        this->_state = {context.subgraph, context.subgraphPassIdx};
    }
}
