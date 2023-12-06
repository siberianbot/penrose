#include "VkFramebufferFactory.hpp"

#include <utility>

#include <Penrose/Common/EngineError.hpp>

#include "src/Builtin/Vulkan/Rendering/VkRenderSubgraph.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderTarget.hpp"

namespace Penrose {

    VkFramebufferFactory::VkFramebufferFactory(const ResourceSet *resources)
            : _logicalDeviceContext(resources->get<VkLogicalDeviceContext>()),
              _swapchainManager(resources->get<VkSwapchainManager>()) {
        //
    }

    VkFramebuffer *VkFramebufferFactory::makeFramebuffer(const std::map<std::string, VkRenderTarget *> &targets,
                                                         VkRenderSubgraph *subgraph) {
        auto swapchainExtent = this->_swapchainManager->getSwapchain()->getExtent();
        auto [width, height] = subgraph->getSubgraphInfo().getRenderArea()
                .value_or(Size(swapchainExtent.width, swapchainExtent.height));

        auto renderArea = vk::Rect2D(vk::Offset2D(), vk::Extent2D(width, height));

        auto attachmentCount = subgraph->getSubgraphInfo().getAttachments().size();
        auto attachmentTargets = std::vector<VkRenderTarget *>(attachmentCount);
        auto clearValues = std::vector<vk::ClearValue>(attachmentCount);
        for (std::uint32_t attachmentIdx = 0; attachmentIdx < attachmentCount; attachmentIdx++) {
            auto &attachment = subgraph->getSubgraphInfo().getAttachments().at(attachmentIdx);
            auto targetIt = targets.find(attachment.getTarget());

            if (targetIt == targets.end()) {
                throw EngineError("No such target {}", attachment.getTarget());
            }

            attachmentTargets[attachmentIdx] = targetIt->second;
            clearValues[attachmentIdx] = vk::ClearValue()
                    .setColor(attachment.getClearValue().getColor())
                    .setDepthStencil(vk::ClearDepthStencilValue(attachment.getClearValue().getDepth(),
                                                                attachment.getClearValue().getStencil()));
        }

        auto imageCount = this->_swapchainManager->getSwapchain()->getImageCount();
        auto framebuffers = std::vector<vk::Framebuffer>(imageCount);
        for (std::uint32_t imageIdx = 0; imageIdx < imageCount; imageIdx++) {
            auto views = std::vector<vk::ImageView>(attachmentCount);

            for (std::uint32_t attachmentIdx = 0; attachmentIdx < attachmentCount; attachmentIdx++) {
                views[attachmentIdx] = attachmentTargets.at(attachmentIdx)->getView(imageIdx);
            }

            auto createInfo = vk::FramebufferCreateInfo()
                    .setRenderPass(subgraph->getRenderPass())
                    .setWidth(width)
                    .setHeight(height)
                    .setAttachments(views)
                    .setLayers(1);

            framebuffers[imageIdx] = this->_logicalDeviceContext->getHandle().createFramebuffer(createInfo);
        }

        return new VkFramebuffer(this->_logicalDeviceContext.get(),
                                 std::forward<decltype(framebuffers)>(framebuffers),
                                 std::forward<decltype(clearValues)>(clearValues),
                                 std::forward<decltype(renderArea)>(renderArea));
    }
}
