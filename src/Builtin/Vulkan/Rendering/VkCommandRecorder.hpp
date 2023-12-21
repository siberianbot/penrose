#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_COMMAND_RECORDER_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_COMMAND_RECORDER_HPP

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/CommandRecorder.hpp>

#include "src/Builtin/Vulkan/Rendering/Objects/VkPipeline.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderContext.hpp"

namespace Penrose {

    class VkCommandRecorder final: public CommandRecorder {
    public:
        explicit VkCommandRecorder(
            VkRenderContext *renderContext, vk::CommandBuffer commandBuffer, vk::RenderPass pass, std::uint32_t subpass
        );
        ~VkCommandRecorder() override = default;

        void configureViewport(FloatRect rect) override;
        void configureScissor(FloatRect rect) override;

        void bindPipeline(Pipeline *pipeline, const PipelineBindingInfo &bindingInfo) override;

        void draw(const DrawInfo &drawInfo) override;

        [[nodiscard]] VkRenderContext *getRenderContext() const { return this->_renderContext; }

        [[nodiscard]] const vk::CommandBuffer &getCommandBuffer() const { return this->_commandBuffer; }

        [[nodiscard]] const vk::RenderPass &getPass() const { return this->_pass; }

        [[nodiscard]] std::uint32_t getSubpass() const { return this->_subpass; }

    private:
        VkRenderContext *_renderContext;
        vk::CommandBuffer _commandBuffer;
        vk::RenderPass _pass;
        std::uint32_t _subpass;

        VkPipeline *_currentPipeline = nullptr;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_COMMAND_RECORDER_HPP
