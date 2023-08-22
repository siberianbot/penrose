#include <Penrose/Builtin/Rendering/ImGuiDrawRenderOperator.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Rendering/DeviceContext.hpp"
#include "src/Rendering/PresentContext.hpp"

#include "src/Builtin/Backends/VulkanBackend.hpp"
#include "src/Builtin/Rendering/VkRenderSubgraph.hpp"

namespace Penrose {

    ImGuiDrawRenderOperator::~ImGuiDrawRenderOperator() {
        ImGui_ImplVulkan_Shutdown();
    }

    void ImGuiDrawRenderOperator::execute(const RenderOperator::Context &context) {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // TODO: draw here
        ImGui::ShowDemoWindow();

        ImGui::Render();
        ImDrawData *drawData = ImGui::GetDrawData();
        ImGui_ImplVulkan_RenderDrawData(drawData, context.commandBuffer);
    }

    ImGuiDrawRenderOperatorFactory::ImGuiDrawRenderOperatorFactory(ResourceSet *resources)
            : _vulkanBackend(resources->get<VulkanBackend>()),
              _deviceContext(resources->get<DeviceContext>()),
              _presentContext(resources->get<PresentContext>()) {
        //
    }

    RenderOperator *ImGuiDrawRenderOperatorFactory::create(const RenderOperatorFactory::Context &context) const {
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
                .Subpass = context.passIdx,
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

        return new ImGuiDrawRenderOperator();
    }
}
