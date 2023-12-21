#ifndef PENROSE_BUILTIN_IMGUI_RENDERING_VK_IMGUI_RENDERER_HPP
#define PENROSE_BUILTIN_IMGUI_RENDERING_VK_IMGUI_RENDERER_HPP

#include <vulkan/vulkan.hpp>

#include <Penrose/Common/Log.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/UI/UIContext.hpp>
#include <Penrose/UI/UIManager.hpp>

#include <Penrose/Builtin/ImGui/Rendering/ImGuiRenderer.hpp>
#include <src/Builtin/Vulkan/Rendering/VkCommandRecorder.hpp>

#include "src/Builtin/Vulkan/Rendering/Objects/VkLogicalDeviceProvider.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkPhysicalDeviceProvider.hpp"
#include "src/Builtin/Vulkan/VulkanBackend.hpp"

#include "src/Builtin/ImGui/UI/ImGuiUIContextVisitor.hpp"

namespace Penrose {

    class VkImGuiRenderer final: public Resource<VkImGuiRenderer>,
                                 public ImGuiRenderer {
    public:
        explicit VkImGuiRenderer(const ResourceSet *resources);
        ~VkImGuiRenderer() override = default;

        void init() override;
        void destroy() override;

        void setRenderGraph(GraphInfo &&graphInfo) override { this->_graph = graphInfo; }

        [[nodiscard]] const GraphInfo &getRenderGraph() override { return this->_graph; }

        void execute(RendererContext *context, const Params &params) override;

    private:
        struct State {
            vk::RenderPass pass;
            std::uint32_t subpass;
        };

        ResourceProxy<Log> _log;
        ResourceProxy<VulkanBackend> _vulkanBackend;
        ResourceProxy<VkPhysicalDeviceProvider> _physicalDeviceProvider;
        ResourceProxy<VkLogicalDeviceProvider> _logicalDeviceProvider;
        ResourceProxy<UIManager> _uiManager;
        ResourceProxy<ImGuiUIContextVisitor> _uiContextVisitor;

        GraphInfo _graph;

        std::optional<State> _state;

        void initState(const VkRenderContext *renderContext, vk::RenderPass pass, uint32_t subpass);

        void draw(CommandRecorder *recorder, UIContext *context);
    };
}

#endif // PENROSE_BUILTIN_IMGUI_RENDERING_VK_IMGUI_RENDERER_HPP
