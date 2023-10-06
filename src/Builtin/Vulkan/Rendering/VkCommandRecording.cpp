#include "VkCommandRecording.hpp"

#include "src/Builtin/Vulkan/Rendering/VkBuffer.hpp"
#include "src/Builtin/Vulkan/Rendering/VkDescriptor.hpp"
#include "src/Builtin/Vulkan/Rendering/VkPipeline.hpp"
#include "src/Builtin/Vulkan/Rendering/VkUtils.hpp"

namespace Penrose {

    VkCommandRecording::VkCommandRecording(std::uint32_t frameIdx,
                                           vk::CommandBuffer commandBuffer)
            : _frameIdx(frameIdx),
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

        if (this->_boundPipeline == vkPipeline) {
            return;
        }

        this->_commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, vkPipeline->getPipeline());
        this->_boundPipeline = vkPipeline;
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

        if (this->_boundDescriptor == vkDescriptor) {
            return;
        }

        this->_commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                                vkPipeline->getPipelineLayout(),
                                                0, vkDescriptor->getDescriptorSets().at(this->_frameIdx), {});
        this->_boundDescriptor = vkDescriptor;
    }

    void VkCommandRecording::bindBuffer(std::uint32_t bindingIdx, Buffer *buffer, std::uint32_t offset) {
        auto vkBuffer = dynamic_cast<VkBuffer *>(buffer);

        this->_commandBuffer.bindVertexBuffers(bindingIdx, vkBuffer->getBuffer(), {offset});
    }

    void VkCommandRecording::bindIndexBuffer(Buffer *buffer, std::uint32_t offset) {
        auto vkBuffer = dynamic_cast<VkBuffer *>(buffer);

        this->_commandBuffer.bindIndexBuffer(vkBuffer->getBuffer(), offset, vk::IndexType::eUint32);
    }

    void VkCommandRecording::draw(std::uint32_t indexCount, std::uint32_t instanceCount) {
        this->_commandBuffer.drawIndexed(indexCount, instanceCount, 0, 0, 0);
    }
}
