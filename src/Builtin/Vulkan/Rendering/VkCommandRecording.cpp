#include "VkCommandRecording.hpp"

#include "src/Rendering/DeviceContext.hpp"

#include "src/Builtin/Vulkan/Rendering/VkBuffer.hpp"
#include "src/Builtin/Vulkan/Rendering/VkDescriptor.hpp"
#include "src/Builtin/Vulkan/Rendering/VkPipeline.hpp"
#include "src/Builtin/Vulkan/Rendering/VkUtils.hpp"

namespace Penrose {

    VkCommandRecording::VkCommandRecording(std::uint32_t frameIdx,
                                           DeviceContext *deviceContext,
                                           vk::CommandBuffer commandBuffer)
            : _frameIdx(frameIdx),
              _deviceContext(deviceContext),
              _commandBuffer(commandBuffer) {
        //
    }

    void VkCommandRecording::setViewport(FloatRect rect) {
        auto viewport = vk::Viewport(rect.x, rect.y, rect.w, rect.h, 0, 1);

        this->_commandBuffer.setViewport(0, viewport);
    }

    void VkCommandRecording::setScissor(IntRect rect) {
        auto scissor = vk::Rect2D(vk::Offset2D(rect.x, rect.y), vk::Extent2D(rect.w, rect.h));

        this->_commandBuffer.setScissor(0, scissor);
    }

    void VkCommandRecording::bindGraphicsPipeline(Pipeline *pipeline) {
        auto vkPipeline = dynamic_cast<VkPipeline *>(pipeline);

        this->_commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, vkPipeline->getPipeline());
    }

    void VkCommandRecording::bindPushConstants(Pipeline *pipeline, std::uint32_t constantIdx, void *data) {
        auto vkPipeline = dynamic_cast<VkPipeline *>(pipeline);

        auto constant = pipeline->getPipelineInfo()
                .getLayout()
                .getConstants()
                .at(constantIdx);

        this->_commandBuffer.pushConstants(vkPipeline->getPipelineLayout(),
                                           toVkShaderStageFlags(constant.getShaderStageType()),
                                           constant.getOffset(), constant.getSize(), data);
    }

    void VkCommandRecording::bindDescriptor(Pipeline *pipeline, Descriptor *descriptor) {
        auto vkPipeline = dynamic_cast<VkPipeline *>(pipeline);
        auto vkDescriptor = dynamic_cast<VkDescriptor *>(descriptor);

        this->_commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                                vkPipeline->getPipelineLayout(),
                                                0, vkDescriptor->getDescriptorSets().at(this->_frameIdx), {});
    }

    void VkCommandRecording::draw(Buffer *vertexBuffer, Buffer *indexBuffer) {
        auto vkVertexBuffer = dynamic_cast<VkBuffer *>(vertexBuffer);
        auto vkIndexBuffer = dynamic_cast<VkBuffer *>(indexBuffer);

        this->_commandBuffer.bindVertexBuffers(0, vkVertexBuffer->getBuffer(), {0});
        this->_commandBuffer.bindIndexBuffer(vkIndexBuffer->getBuffer(), 0, vk::IndexType::eUint32);

        // TODO: instancing
        this->_commandBuffer.drawIndexed(vkIndexBuffer->getCount(), 1, 0, 0, 0);
    }
}