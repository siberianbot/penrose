#include "VkImGuiRenderer.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include <Penrose/Common/EngineError.hpp>

#include "src/Builtin/Vulkan/Rendering/VkCommandRecorder.hpp"
#include "src/Builtin/Vulkan/Rendering/VkUtils.hpp"

namespace Penrose {

    inline static constexpr std::string_view TAG = "VkImGuiRenderer";

    VkImGuiRenderer::VkImGuiRenderer(const ResourceSet *resources)
        : _log(resources->get<Log>()),
          _vulkanBackend(resources->get<VulkanBackend>()),
          _physicalDeviceProvider(resources->get<VkPhysicalDeviceProvider>()),
          _logicalDeviceProvider(resources->get<VkLogicalDeviceProvider>()),
          _uiManager(resources->get<UIManager>()),
          _uiContextVisitor(resources->get<ImGuiUIContextVisitor>()),
          _graph(GraphInfo {
              .name = "ImGuiRendererGraph",
              .attachments = {AttachmentInfo {
                  .format = std::nullopt,
                  .clearValue = {.color = {0, 0, 0, 1}},
                  .loadOp = AttachmentLoadOp::Load,
                  .storeOp = AttachmentStoreOp::Store,
                  .initialLayout = AttachmentLayout::Undefined,
                  .finalLayout = AttachmentLayout::Present,
              }},
              .passes = {PassInfo {
                  .dependsOn = {},
                  .colorTargets = {0},
                  .depthStencilTarget = std::nullopt,
                  .function = "Draw",
              }},
              .area = std::nullopt,
          }) {
        //
    }

    void VkImGuiRenderer::init() {
        //
    }

    void VkImGuiRenderer::destroy() {
        if (!this->_state.has_value()) {
            return;
        }

        ImGui_ImplVulkan_Shutdown();

        this->_state = std::nullopt;
    }

    void VkImGuiRenderer::execute(RendererContext *context, const Params &params) {
        const auto contextName = params.tryGet<std::string>("Context").value_or("Default");
        const auto maybeUIContext = this->_uiManager->tryGetContext(contextName);

        if (!maybeUIContext.has_value()) {
            this->_log->writeError(TAG, "No UI context {} available", contextName);

            return;
        }

        const auto targets = {
            TargetInfo {.type = TargetType::Swapchain},
        };

        const auto functions = RendererFunctionMap {
            {"Draw", [&, context = *maybeUIContext](CommandRecorder *recorder) { this->draw(recorder, context); }}
        };

        context->executeGraph(targets, this->_graph, functions);
    }

    void VkImGuiRenderer::initState(
        const VkRenderContext *renderContext, const vk::RenderPass pass, const uint32_t subpass
    ) {
        auto &device = this->_logicalDeviceProvider->getLogicalDevice().handle;
        auto &queue = this->_logicalDeviceProvider->getLogicalDevice().graphicsQueue;

        const std::uint32_t imageCount = renderContext->getSwapchain().imageViews.size();

        auto initInfo = ImGui_ImplVulkan_InitInfo {
            .Instance = this->_vulkanBackend->getInstance(),
            .PhysicalDevice = this->_physicalDeviceProvider->getPhysicalDevice().handle,
            .Device = device.get(),
            .QueueFamily = this->_physicalDeviceProvider->getPhysicalDevice().graphicsFamilyIdx,
            .Queue = queue,
            .PipelineCache = nullptr,
            .DescriptorPool = renderContext->getDescriptorPool(),
            .Subpass = subpass,
            .MinImageCount = imageCount,
            .ImageCount = imageCount,
            .MSAASamples = VK_SAMPLE_COUNT_1_BIT,
            .Allocator = nullptr,
            .CheckVkResultFn =
                [](VkResult result) {
                    if (result != VK_SUCCESS) {
                        vk::detail::throwResultException(static_cast<vk::Result>(result), "Vulkan assertion failed");
                    }
                }
        };

        ImGui_ImplVulkan_Init(&initInfo, pass);

        {
            const auto allocateInfo = vk::CommandBufferAllocateInfo()
                                          .setLevel(vk::CommandBufferLevel::ePrimary)
                                          .setCommandPool(renderContext->getCommandPool())
                                          .setCommandBufferCount(1);

            auto commandBuffers = device->allocateCommandBuffersUnique(allocateInfo);

            commandBuffers.at(0)->begin(vk::CommandBufferBeginInfo());

            ImGui_ImplVulkan_CreateFontsTexture(commandBuffers.at(0).get());

            commandBuffers.at(0)->end();

            queue.submit(vk::SubmitInfo().setCommandBuffers(commandBuffers.at(0).get()));
            queue.waitIdle();

            ImGui_ImplVulkan_DestroyFontUploadObjects();
        }

        this->_state = State {
            .pass = pass,
            .subpass = subpass,
        };
    }

    void VkImGuiRenderer::draw(CommandRecorder *recorder, UIContext *context) {
        const auto vkRecorder = asVkCommandRecorder(recorder);

        if (!this->_state) {
            this->initState(vkRecorder->getRenderContext(), vkRecorder->getPass(), vkRecorder->getSubpass());
        } else if (vkRecorder->getPass() != this->_state->pass || vkRecorder->getSubpass() != this->_state->subpass) {
            throw EngineError(
                "VkImGuiRenderer was initialized for pass 0x{:#x}, subpass {}",
                reinterpret_cast<std::size_t>(static_cast<VkRenderPass>(this->_state->pass)), this->_state->subpass
            );
        }

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        this->_uiContextVisitor->visit(context);

        ImGui::Render();
        ImDrawData *drawData = ImGui::GetDrawData();
        ImGui_ImplVulkan_RenderDrawData(drawData, vkRecorder->getCommandBuffer());
    }
}
