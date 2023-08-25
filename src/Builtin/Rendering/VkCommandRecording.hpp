#ifndef PENROSE_BUILTIN_RENDERING_VK_COMMAND_RECORDING_HPP
#define PENROSE_BUILTIN_RENDERING_VK_COMMAND_RECORDING_HPP

#include <cstdint>

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/CommandRecording.hpp>

namespace Penrose {

    class DeviceContext;

    class VkCommandRecording : public CommandRecording {
    public:
        explicit VkCommandRecording(std::uint32_t frameIdx,
                                    DeviceContext *deviceContext,
                                    vk::CommandBuffer commandBuffer);
        ~VkCommandRecording() override = default;

        [[nodiscard]] const vk::CommandBuffer &getCommandBuffer() const { return this->_commandBuffer; }

        void bindGraphicsPipeline(Pipeline *pipeline) override;

        void bindPushConstants(Pipeline *pipeline, std::uint32_t constantIdx, void *data) override;
        void bindDescriptor(Pipeline *pipeline, Descriptor *descriptor) override;

        void draw(Buffer *vertexBuffer, Buffer *indexBuffer) override;

    private:
        std::uint32_t _frameIdx;

        DeviceContext *_deviceContext;

        vk::CommandBuffer _commandBuffer;
    };
}

#endif // PENROSE_BUILTIN_RENDERING_VK_COMMAND_RECORDING_HPP
