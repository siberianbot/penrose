#include "ImGuiDrawRenderOperator.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include "src/Backends/VulkanBackend.hpp"
#include "src/Rendering/DeviceContext.hpp"
#include "src/Rendering/PresentContext.hpp"
#include "src/Resources/ResourceSet.hpp"

namespace Penrose {

    ImGuiDrawRenderOperator::ImGuiDrawRenderOperator(const vk::Device &logicalDevice,
                                                     const vk::DescriptorPool &descriptorPool)
            : _logicalDevice(logicalDevice),
              _descriptorPool(descriptorPool) {
        //
    }

    ImGuiDrawRenderOperator::~ImGuiDrawRenderOperator() {
        ImGui_ImplVulkan_Shutdown();

        this->_logicalDevice.destroy(this->_descriptorPool);
    }

    void ImGuiDrawRenderOperator::execute(const RenderOperatorExecutionContext &context) {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // TODO: draw here
        ImGui::ShowDemoWindow();

        ImGui::Render();
        ImDrawData *drawData = ImGui::GetDrawData();
        ImGui_ImplVulkan_RenderDrawData(drawData, context.commandBuffer);
    }

    std::unique_ptr<RenderOperator> ImGuiDrawRenderOperatorProducer::produce(
            const RenderOperatorProduceContext &context) const {
        auto vulkanBackend = context.resources->get<VulkanBackend>();
        auto deviceContext = context.resources->get<DeviceContext>();
        auto presentContext = context.resources->get<PresentContext>();

        auto descriptorPoolSizes = {
                vk::DescriptorPoolSize(vk::DescriptorType::eSampler, 1024),
                vk::DescriptorPoolSize(vk::DescriptorType::eCombinedImageSampler, 1024),
                vk::DescriptorPoolSize(vk::DescriptorType::eSampledImage, 1024),
                vk::DescriptorPoolSize(vk::DescriptorType::eStorageImage, 1024),
                vk::DescriptorPoolSize(vk::DescriptorType::eUniformTexelBuffer, 1024),
                vk::DescriptorPoolSize(vk::DescriptorType::eStorageTexelBuffer, 1024),
                vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, 1024),
                vk::DescriptorPoolSize(vk::DescriptorType::eStorageBuffer, 1024),
                vk::DescriptorPoolSize(vk::DescriptorType::eUniformBufferDynamic, 1024),
                vk::DescriptorPoolSize(vk::DescriptorType::eStorageBufferDynamic, 1024),
                vk::DescriptorPoolSize(vk::DescriptorType::eInputAttachment, 1024),
        };

        auto descriptorPoolCreateInfo = vk::DescriptorPoolCreateInfo()
                .setMaxSets(1024)
                .setPoolSizes(descriptorPoolSizes);

        auto logicalDevice = deviceContext->getLogicalDevice();
        auto descriptorPool = logicalDevice.createDescriptorPool(descriptorPoolCreateInfo);
        auto imageCount = static_cast<uint32_t>(presentContext->getSwapchainImages().size());

        auto initInfo = ImGui_ImplVulkan_InitInfo{
                .Instance = vulkanBackend->getInstance(),
                .PhysicalDevice = deviceContext->getPhysicalDevice(),
                .Device = logicalDevice,
                .QueueFamily = deviceContext->getGraphicsQueueFamily(),
                .Queue = deviceContext->getGraphicsQueue(),
                .PipelineCache = nullptr,
                .DescriptorPool = descriptorPool,
                .Subpass = context.subpassIdx,
                .MinImageCount = imageCount,
                .ImageCount = imageCount,
                .MSAASamples = VK_SAMPLE_COUNT_1_BIT,
                .Allocator = nullptr,
                .CheckVkResultFn = [](VkResult result) {
                    if (result != VK_SUCCESS) {
                        vk::throwResultException((vk::Result) result, "Vulkan assertion failed");
                    }
                }
        };

        ImGui_ImplVulkan_Init(&initInfo, context.renderPass);

        auto allocateInfo = vk::CommandBufferAllocateInfo()
                .setCommandPool(deviceContext->getCommandPool())
                .setCommandBufferCount(1);
        auto commandBuffers = logicalDevice.allocateCommandBuffers(allocateInfo);

        commandBuffers.at(0).begin(vk::CommandBufferBeginInfo());
        ImGui_ImplVulkan_CreateFontsTexture(commandBuffers.at(0));
        commandBuffers.at(0).end();

        auto submit = vk::SubmitInfo()
                .setCommandBuffers(commandBuffers);

        deviceContext->getGraphicsQueue().submit(submit);
        deviceContext->getGraphicsQueue().waitIdle();

        logicalDevice.freeCommandBuffers(deviceContext->getCommandPool(), commandBuffers);

        ImGui_ImplVulkan_DestroyFontUploadObjects();

        return std::make_unique<ImGuiDrawRenderOperator>(logicalDevice, descriptorPool);
    }
}
