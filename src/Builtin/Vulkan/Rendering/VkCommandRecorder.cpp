#include "VkCommandRecorder.hpp"

#include <vector>

#include <Penrose/Common/EngineError.hpp>

#include "src/Builtin/Vulkan/Rendering/Objects/VkBuffer.hpp"
#include "src/Builtin/Vulkan/Rendering/VkUtils.hpp"

namespace Penrose {

    VkCommandRecorder::VkCommandRecorder(
        VkRenderContext *renderContext, const vk::CommandBuffer commandBuffer, const vk::RenderPass pass,
        const std::uint32_t subpass
    )
        : _renderContext(renderContext),
          _commandBuffer(commandBuffer),
          _pass(pass),
          _subpass(subpass) {
        //
    }

    void VkCommandRecorder::configureViewport(const FloatRect rect) {
        const auto extent = this->_renderContext->getSwapchain().extent;

        const auto viewport = vk::Viewport(
            rect.x * static_cast<float>(extent.width), rect.y * static_cast<float>(extent.height),
            rect.w * static_cast<float>(extent.width), rect.h * static_cast<float>(extent.height), 0, 1
        );

        this->_commandBuffer.setViewport(0, 1, &viewport);
    }

    void VkCommandRecorder::configureScissor(const FloatRect rect) {
        const auto extent = this->_renderContext->getSwapchain().extent;

        const auto scissor = vk::Rect2D(
            vk::Offset2D(
                static_cast<std::int32_t>(rect.x * static_cast<float>(extent.width)),
                static_cast<std::int32_t>(rect.y * static_cast<float>(extent.height))
            ),
            vk::Extent2D(
                static_cast<std::int32_t>(rect.w * static_cast<float>(extent.width)),
                static_cast<std::int32_t>(rect.h * static_cast<float>(extent.height))
            )
        );

        this->_commandBuffer.setScissor(0, 1, &scissor);
    }

    void VkCommandRecorder::bindPipeline(Pipeline *pipeline, const PipelineBindingInfo &bindingInfo) {
        if (this->_currentPipeline != pipeline) {
            this->_currentPipeline = asVkPipeline(pipeline);

            this->_commandBuffer.bindPipeline(
                vk::PipelineBindPoint::eGraphics,
                this->_renderContext->usePipeline(this->_currentPipeline, this->_pass, this->_subpass)
            );
        }

        if (this->_currentPipeline == nullptr) {
            return;
        }

        const auto &pipelineInfo = this->_currentPipeline->getPipelineInfo();
        const auto constantsCount = pipelineInfo.constants.size();
        if (constantsCount != bindingInfo.constants.size()) {
            throw EngineError(
                "Expected {} constant(s), got {} constant(s)", constantsCount, bindingInfo.constants.size()
            );
        }

        for (std::uint32_t constantIdx = 0; constantIdx < constantsCount; constantIdx++) {
            auto &constantInfo = pipelineInfo.constants.at(constantIdx);

            this->_commandBuffer.pushConstants(
                this->_currentPipeline->getPipelineLayoutHandle(), mapShaderStage(constantInfo.stage),
                constantInfo.offset, constantInfo.size, bindingInfo.constants.at(constantIdx).data
            );
        }

        const auto descriptorSets = this->_renderContext->useDescriptor(
            this->_currentPipeline, this->_pass, this->_subpass, bindingInfo
        );

        this->_commandBuffer.bindDescriptorSets(
            vk::PipelineBindPoint::eGraphics, this->_currentPipeline->getPipelineLayoutHandle(), 0, descriptorSets, {}
        );
    }

    void VkCommandRecorder::draw(const DrawInfo &drawInfo) {
        if (this->_currentPipeline == nullptr) {
            throw EngineError("No pipeline was bound");
        }

        const auto &pipelineInfo = this->_currentPipeline->getPipelineInfo();

        if (drawInfo.buffers.size() != pipelineInfo.inputs.size()) {
            throw EngineError(
                "Expected {} input buffers(s), got {} input buffer(s)", pipelineInfo.inputs.size(),
                drawInfo.buffers.size()
            );
        }

        auto buffers = std::vector<vk::Buffer>(drawInfo.buffers.size());
        auto offsets = std::vector<vk::DeviceSize>(drawInfo.buffers.size());

        for (std::uint32_t idx = 0; idx < drawInfo.buffers.size(); idx++) {
            const auto &bufferUsage = drawInfo.buffers.at(idx);

            buffers[idx] = asVkBuffer(bufferUsage.buffer)->getHandle();
            offsets[idx] = bufferUsage.offset;
        }

        this->_commandBuffer.bindVertexBuffers(0, drawInfo.buffers.size(), buffers.data(), offsets.data());

        if (drawInfo.index.has_value()) {
            this->_commandBuffer.bindIndexBuffer(
                asVkBuffer(drawInfo.index->buffer)->getHandle(), drawInfo.index->offset, vk::IndexType::eUint32
            );

            this->_commandBuffer.drawIndexed(drawInfo.count, drawInfo.instanceCount, 0, 0, 0);
        } else {
            this->_commandBuffer.draw(drawInfo.count, drawInfo.instanceCount, 0, 0);
        }
    }
}
