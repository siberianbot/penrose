#include "VkRenderPass.hpp"

#include <list>
#include <vector>

#include "src/Constants.hpp"
#include "src/Rendering/DeviceContext.hpp"
#include "src/Rendering/PresentContext.hpp"

#include "src/Builtin/Rendering/VkUtils.hpp"

namespace Penrose {

    VkRenderPass::VkRenderPass(DeviceContext *deviceContext,
                               vk::RenderPass renderPass,
                               std::array<vk::Semaphore, INFLIGHT_FRAME_COUNT> semaphores)
            : _deviceContext(deviceContext),
              _renderPass(renderPass),
              _semaphores(semaphores) {
        //
    }

    VkRenderPass::~VkRenderPass() {
        this->_deviceContext->getLogicalDevice().destroy(this->_renderPass);

        for (const auto &semaphore: this->_semaphores) {
            this->_deviceContext->getLogicalDevice().destroy(semaphore);
        }
    }

    VkRenderPass *makeVkRenderPass(DeviceContext *deviceContext,
                                   PresentContext *presentContext,
                                   const RenderSubgraph &subgraph) {
        auto defaultFormat = presentContext->getSwapchainFormat();

        auto attachments = std::vector<vk::AttachmentDescription>(subgraph.getAttachments().size());
        std::transform(subgraph.getAttachments().begin(), subgraph.getAttachments().end(), attachments.begin(),
                       [&defaultFormat](const RenderAttachment &attachment) {
                           return vk::AttachmentDescription()
                                   .setFormat(toVkFormat(attachment.getFormat()).value_or(defaultFormat))
                                   .setLoadOp(toVkAttachmentLoadOp(attachment.getLoadOp()))
                                   .setStoreOp(toVkAttachmentStoreOp(attachment.getStoreOp()))
                                   .setInitialLayout(toVkImageLayout(attachment.getInitialLayout()))
                                   .setFinalLayout(toVkImageLayout(attachment.getFinalLayout()));
                       });

        auto refsList = std::list<std::vector<vk::AttachmentReference>>(subgraph.getPasses().size());
        auto subpasses = std::vector<vk::SubpassDescription>(subgraph.getPasses().size());
        auto dependencies = std::vector<vk::SubpassDependency>();

        for (std::uint32_t passIdx = 0; passIdx < subgraph.getPasses().size(); passIdx++) {
            auto pass = subgraph.getPasses().at(passIdx);

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

        auto renderPass = deviceContext->getLogicalDevice().createRenderPass(renderPassCreateInfo);

        std::array<vk::Semaphore, INFLIGHT_FRAME_COUNT> semaphores;
        for (std::uint32_t idx = 0; idx < INFLIGHT_FRAME_COUNT; idx++) {
            semaphores[idx] = deviceContext->getLogicalDevice().createSemaphore(vk::SemaphoreCreateInfo());
        }

        return new VkRenderPass(deviceContext, renderPass, semaphores);
    }
}
