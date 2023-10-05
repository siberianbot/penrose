#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_COMMAND_RECORDING_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_COMMAND_RECORDING_HPP

#include <cstdint>

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/CommandRecording.hpp>

namespace Penrose {

    class VkBuffer;
    class VkDescriptor;
    class VkPipeline;

    class VkCommandRecording : public CommandRecording {
    public:
        explicit VkCommandRecording(std::uint32_t frameIdx,
                                    vk::CommandBuffer commandBuffer);
        ~VkCommandRecording() override = default;

        [[nodiscard]] const vk::CommandBuffer &getCommandBuffer() const { return this->_commandBuffer; }

        void setViewport(FloatRect rect) override;
        void setScissor(IntRect rect) override;

        void bindGraphicsPipeline(Pipeline *pipeline) override;

        void bindPushConstants(Pipeline *pipeline, std::uint32_t constantIdx, void *data) override;
        void bindDescriptor(Pipeline *pipeline, Descriptor *descriptor) override;

        void draw(Buffer *vertexBuffer, Buffer *indexBuffer) override;

    private:
        std::uint32_t _frameIdx;

        vk::CommandBuffer _commandBuffer;

        VkPipeline *_boundPipeline = nullptr;
        VkDescriptor *_boundDescriptor = nullptr;
        VkBuffer *_boundVertexBuffer = nullptr;
        VkBuffer *_boundIndexBuffer = nullptr;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_COMMAND_RECORDING_HPP
