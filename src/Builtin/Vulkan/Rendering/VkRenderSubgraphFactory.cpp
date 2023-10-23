#include "VkRenderSubgraphFactory.hpp"

#include <utility>

#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Rendering/VkUtils.hpp"

namespace Penrose {

    VkRenderSubgraphFactory::VkRenderSubgraphFactory(ResourceSet *resources)
            : _logicalDeviceContext(resources->get<VkLogicalDeviceContext>()),
              _swapchainManager(resources->get<VkSwapchainManager>()) {
        //
    }

    VkRenderSubgraph *VkRenderSubgraphFactory::makeRenderSubgraph(RenderSubgraphInfo &&subgraphInfo) {
        auto defaultFormat = this->_swapchainManager->getSwapchain()->getFormat();

        auto attachments = std::vector<vk::AttachmentDescription>(subgraphInfo.getAttachments().size());
        std::transform(subgraphInfo.getAttachments().begin(), subgraphInfo.getAttachments().end(), attachments.begin(),
                       [&defaultFormat](const RenderAttachmentInfo &attachment) {
                           return vk::AttachmentDescription()
                                   .setFormat(toVkFormat(attachment.getFormat()).value_or(defaultFormat))
                                   .setLoadOp(toVkAttachmentLoadOp(attachment.getLoadOp()))
                                   .setStoreOp(toVkAttachmentStoreOp(attachment.getStoreOp()))
                                   .setInitialLayout(toVkImageLayout(attachment.getInitialLayout()))
                                   .setFinalLayout(toVkImageLayout(attachment.getFinalLayout()));
                       });

        auto refsList = std::list<std::vector<vk::AttachmentReference>>(subgraphInfo.getPasses().size());
        auto subpasses = std::vector<vk::SubpassDescription>(subgraphInfo.getPasses().size());
        auto dependencies = std::vector<vk::SubpassDependency>();

        for (std::uint32_t passIdx = 0; passIdx < subgraphInfo.getPasses().size(); passIdx++) {
            auto pass = subgraphInfo.getPasses().at(passIdx);

            auto inputCount = pass.getInputAttachments().size();
            auto colorCount = pass.getColorAttachments().size();
            auto depthCount = pass.getDepthStencilAttachment().has_value() ? 1 : 0;
            auto &refs = refsList.emplace_back(inputCount + colorCount + depthCount);

            for (std::uint32_t idx = 0; idx < inputCount; idx++) {
                refs[idx] = vk::AttachmentReference()
                        .setAttachment(pass.getInputAttachments().at(idx))
                        .setLayout(vk::ImageLayout::eShaderReadOnlyOptimal);
            }

            for (std::uint32_t idx = 0; idx < colorCount; idx++) {
                refs[inputCount + idx] = vk::AttachmentReference()
                        .setAttachment(pass.getColorAttachments().at(idx))
                        .setLayout(vk::ImageLayout::eColorAttachmentOptimal);
            }

            if (pass.getDepthStencilAttachment().has_value()) {
                refs[inputCount + colorCount] = vk::AttachmentReference()
                        .setAttachment(*pass.getDepthStencilAttachment())
                        .setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);
            }

            auto ptr = refs.data();
            subpasses[passIdx] = vk::SubpassDescription()
                    .setInputAttachmentCount(inputCount)
                    .setPInputAttachments(inputCount > 0 ? &ptr[0] : nullptr)
                    .setColorAttachmentCount(colorCount)
                    .setPColorAttachments(colorCount > 0 ? &ptr[inputCount] : nullptr)
                    .setPDepthStencilAttachment(pass.getDepthStencilAttachment().has_value()
                                                ? &ptr[inputCount + colorCount]
                                                : nullptr);

            if (!pass.getDependsOn().empty()) {
                std::for_each(pass.getDependsOn().begin(), pass.getDependsOn().end(), [&](const std::uint32_t &srcIdx) {
                    dependencies.push_back(vk::SubpassDependency()
                                                   .setSrcSubpass(srcIdx)
                                                   .setDstSubpass(passIdx)
                                                   .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
                                                   .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
                                                   .setSrcAccessMask(vk::AccessFlagBits::eColorAttachmentWrite |
                                                                     vk::AccessFlagBits::eColorAttachmentRead)
                                                   .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite |
                                                                     vk::AccessFlagBits::eColorAttachmentRead));
                });
            } else {
                dependencies.push_back(vk::SubpassDependency()
                                               .setSrcSubpass(VK_SUBPASS_EXTERNAL)
                                               .setDstSubpass(passIdx)
                                               .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
                                               .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
                                               .setSrcAccessMask(vk::AccessFlagBits::eColorAttachmentWrite |
                                                                 vk::AccessFlagBits::eColorAttachmentRead)
                                               .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite |
                                                                 vk::AccessFlagBits::eColorAttachmentRead));
            }
        }

        auto renderPassCreateInfo = vk::RenderPassCreateInfo()
                .setAttachments(attachments)
                .setSubpasses(subpasses)
                .setDependencies(dependencies);

        auto renderPass = this->_logicalDeviceContext->getHandle().createRenderPass(renderPassCreateInfo);

        std::array<vk::Semaphore, INFLIGHT_FRAME_COUNT> semaphores;
        for (std::uint32_t idx = 0; idx < INFLIGHT_FRAME_COUNT; idx++) {
            semaphores[idx] = this->_logicalDeviceContext->getHandle().createSemaphore(vk::SemaphoreCreateInfo());
        }

        return new VkRenderSubgraph(std::forward<decltype(subgraphInfo)>(subgraphInfo),
                                    this->_logicalDeviceContext.get(),
                                    renderPass, semaphores);
    }
}
