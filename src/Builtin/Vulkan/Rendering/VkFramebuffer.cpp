#include "VkFramebuffer.hpp"

#include <utility>

#include <fmt/core.h>

#include <Penrose/Common/EngineError.hpp>

#include "src/Rendering/DeviceContext.hpp"
#include "src/Rendering/PresentContext.hpp"

#include "src/Builtin/Vulkan/Rendering/VkRenderTarget.hpp"

namespace Penrose {

    VkFramebuffer::VkFramebuffer(DeviceContext *deviceContext,
                                 std::vector<vk::Framebuffer> framebuffers,
                                 std::vector<vk::ClearValue> clearValues,
                                 vk::Rect2D renderArea)
            : _deviceContext(deviceContext),
              _framebuffers(std::move(framebuffers)),
              _clearValues(std::move(clearValues)),
              _renderArea(renderArea) {
        //
    }

    VkFramebuffer::~VkFramebuffer() {
        for (const auto &framebuffer: this->_framebuffers) {
            this->_deviceContext->getLogicalDevice().destroy(framebuffer);
        }
    }

    VkFramebuffer *makeVkFramebuffer(DeviceContext *deviceContext,
                                     PresentContext *presentContext,
                                     const std::map<std::string, VkRenderTarget *> &targets,
                                     const vk::RenderPass &renderPass,
                                     const RenderSubgraphInfo &subgraph) {
        auto swapchainExtent = presentContext->getSwapchainExtent();
        auto [width, height] = subgraph.getRenderArea()
                .value_or(Size(swapchainExtent.width, swapchainExtent.height));

        auto renderArea = vk::Rect2D(vk::Offset2D(), vk::Extent2D(width, height));

        auto attachmentCount = subgraph.getAttachments().size();
        auto attachmentTargets = std::vector<VkRenderTarget *>(attachmentCount);
        auto clearValues = std::vector<vk::ClearValue>(attachmentCount);
        for (std::uint32_t attachmentIdx = 0; attachmentIdx < attachmentCount; attachmentIdx++) {
            auto &attachment = subgraph.getAttachments().at(attachmentIdx);
            auto targetIt = targets.find(attachment.getTarget());

            if (targetIt == targets.end()) {
                throw EngineError(fmt::format("No such target {}", attachment.getTarget()));
            }

            attachmentTargets[attachmentIdx] = targetIt->second;
            clearValues[attachmentIdx] = vk::ClearValue()
                    .setColor(attachment.getClearValue().getColor())
                    .setDepthStencil(vk::ClearDepthStencilValue(attachment.getClearValue().getDepth(),
                                                                attachment.getClearValue().getStencil()));
        }

        auto imageCount = presentContext->getSwapchainImages().size();
        auto framebuffers = std::vector<vk::Framebuffer>(imageCount);
        for (std::uint32_t imageIdx = 0; imageIdx < imageCount; imageIdx++) {
            auto views = std::vector<vk::ImageView>(attachmentCount);

            for (std::uint32_t attachmentIdx = 0; attachmentIdx < attachmentCount; attachmentIdx++) {
                views[attachmentIdx] = attachmentTargets.at(attachmentIdx)->getView(imageIdx);
            }

            auto createInfo = vk::FramebufferCreateInfo()
                    .setRenderPass(renderPass)
                    .setWidth(width)
                    .setHeight(height)
                    .setAttachments(views)
                    .setLayers(1);

            framebuffers[imageIdx] = deviceContext->getLogicalDevice().createFramebuffer(createInfo);
        }

        return new VkFramebuffer(deviceContext,
                                 framebuffers,
                                 clearValues,
                                 renderArea);
    }
}
