#include "VkFramebuffer.hpp"

#include <utility>

#include <fmt/core.h>

#include <Penrose/Common/EngineError.hpp>

#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderTarget.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSwapchainManager.hpp"

namespace Penrose {

    VkFramebuffer::VkFramebuffer(VkLogicalDeviceContext *logicalDeviceContext,
                                 std::vector<vk::Framebuffer> framebuffers,
                                 std::vector<vk::ClearValue> clearValues,
                                 vk::Rect2D renderArea)
            : _logicalDeviceContext(logicalDeviceContext),
              _framebuffers(std::move(framebuffers)),
              _clearValues(std::move(clearValues)),
              _renderArea(renderArea) {
        //
    }

    VkFramebuffer::~VkFramebuffer() {
        for (const auto &framebuffer: this->_framebuffers) {
            this->_logicalDeviceContext->getHandle().destroy(framebuffer);
        }
    }

    VkFramebuffer *makeVkFramebuffer(VkLogicalDeviceContext *logicalDeviceContext,
                                     VkSwapchainManager *swapchainManager,
                                     const std::map<std::string, VkRenderTarget *> &targets,
                                     const vk::RenderPass &renderPass,
                                     const RenderSubgraphInfo &subgraph) {
        auto swapchainExtent = swapchainManager->getSwapchain()->getExtent();
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

        auto imageCount = swapchainManager->getSwapchain()->getImageCount();
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

            framebuffers[imageIdx] = logicalDeviceContext->getHandle().createFramebuffer(createInfo);
        }

        return new VkFramebuffer(logicalDeviceContext,
                                 framebuffers,
                                 clearValues,
                                 renderArea);
    }
}
