#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_CONTEXT_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_CONTEXT_HPP

#include <array>
#include <cstdint>
#include <initializer_list>
#include <map>
#include <optional>
#include <set>

#include <vulkan/vulkan.hpp>

#include <Penrose/Common/Log.hpp>
#include <Penrose/Rendering/Graph/GraphInfo.hpp>
#include <Penrose/Rendering/Graph/TargetInfo.hpp>
#include <Penrose/Rendering/RenderContext.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Constants.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkImageFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkInternalObjectFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkPipelineFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkPipelineInstance.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkSwapchain.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkSwapchainFactory.hpp"
#include "src/Builtin/Vulkan/VkOperators.hpp"

namespace Penrose {

    class VkRenderContext final: public RenderContext {
    public:
        struct FrameData {
            vk::UniqueFence fence;
            vk::UniqueCommandBuffer commandBuffer;
            vk::UniqueSemaphore imageReady;
            vk::UniqueSemaphore renderFinished;
        };

        VkRenderContext(
            ResourceProxy<Log> log, ResourceProxy<VkLogicalDeviceProvider> logicalDeviceProvider,
            ResourceProxy<VkInternalObjectFactory> internalObjectFactory, ResourceProxy<VkImageFactory> imageFactory,
            ResourceProxy<VkPipelineFactory> pipelineFactory, ResourceProxy<VkSwapchainFactory> swapchainFactory,
            VkSwapchain &&swapchain, vk::UniqueCommandPool &&commandPool, vk::UniqueDescriptorPool &&descriptorPool,
            std::array<FrameData, INFLIGHT_FRAME_COUNT> &&frameData
        );
        ~VkRenderContext() override;

        [[nodiscard]] bool beginRender() override;
        void submitRender() override;

        [[nodiscard]] RendererContext *makeRendererContext() override;

        [[nodiscard]] std::vector<vk::CommandBuffer> makeSecondaryCommandBuffers(std::uint32_t count);

        void invalidate() override;

        [[nodiscard]] vk::ImageView useTarget(const TargetInfo &target);
        [[nodiscard]] vk::RenderPass usePass(const GraphInfo &graph);
        [[nodiscard]] vk::Framebuffer useFramebuffer(
            const std::initializer_list<TargetInfo> &targets, const GraphInfo &graph, const vk::Extent2D &extent
        );
        [[nodiscard]] vk::Pipeline usePipeline(const VkPipeline *pipeline, vk::RenderPass pass, std::uint32_t subpass);
        [[nodiscard]] vk::DescriptorSet useDescriptor(
            const VkPipeline *pipeline, vk::RenderPass pass, std::uint32_t subpass, const PipelineBindingInfo &binding
        );

        [[nodiscard]] const VkSwapchain &getSwapchain() const { return this->_swapchain; }

        [[nodiscard]] vk::CommandPool getCommandPool() const { return this->_commandPool.get(); }

        [[nodiscard]] vk::DescriptorPool getDescriptorPool() const { return this->_descriptorPool.get(); }

        [[nodiscard]] const vk::CommandBuffer &getCurrentCommandBuffer() const {
            return this->_currentState->commandBuffer;
        }

    private:
        using ImageTarget = std::tuple<TargetInfo, VkImageInternal>;
        using PipelineKey = std::tuple<std::string, vk::RenderPass, std::uint32_t>;

        struct Pass {
            GraphInfo info;
            vk::UniqueRenderPass pass;
            std::map<std::vector<vk::ImageView>, vk::UniqueFramebuffer> framebuffers;
        };

        struct PipelineDescriptorData {
            std::vector<PipelineObjectBinding> info;
            vk::UniqueDescriptorSet descriptor;
        };

        struct PipelineData {
            std::unique_ptr<VkPipelineInstance> instance;
            std::map<std::string, std::array<PipelineDescriptorData, INFLIGHT_FRAME_COUNT>> descriptors;
        };

        struct State {
            std::uint32_t imageIdx;
            vk::CommandBuffer commandBuffer;
            vk::Fence fence;
            vk::Semaphore imageReady;
            vk::Semaphore renderFinished;
            std::set<vk::UniqueDescriptorSet, VkUniqueDescriptorSetLess> descriptors;
        };

        ResourceProxy<Log> _log;
        ResourceProxy<VkLogicalDeviceProvider> _logicalDeviceProvider;
        ResourceProxy<VkInternalObjectFactory> _internalObjectFactory;
        ResourceProxy<VkImageFactory> _imageFactory;
        ResourceProxy<VkPipelineFactory> _pipelineFactory;
        ResourceProxy<VkSwapchainFactory> _swapchainFactory;

        vk::UniqueCommandPool _commandPool;
        vk::UniqueDescriptorPool _descriptorPool;
        VkSwapchain _swapchain;
        std::array<FrameData, INFLIGHT_FRAME_COUNT> _frameData;

        std::uint32_t _currentFrameIdx = 0;
        std::optional<State> _currentState;

        std::map<std::string, ImageTarget> _imageTargets;
        std::map<std::string, Pass> _passes;
        std::map<PipelineKey, PipelineData> _pipelines;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_CONTEXT_HPP
