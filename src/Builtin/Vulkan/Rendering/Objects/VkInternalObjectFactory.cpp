#include "VkInternalObjectFactory.hpp"

#include "src/Builtin/Vulkan/Rendering/VkUtils.hpp"

namespace Penrose {

    VkInternalObjectFactory::VkInternalObjectFactory(const ResourceSet *resources)
        : _logicalDeviceProvider(resources->get<VkLogicalDeviceProvider>()) {
        //
    }

    vk::UniqueRenderPass VkInternalObjectFactory::makeRenderPass(const GraphInfo &graph, const VkSwapchain &swapchain) {
        auto attachments = std::vector<vk::AttachmentDescription>(graph.attachments.size());
        std::ranges::transform(graph.attachments, attachments.begin(), [&](const AttachmentInfo &attachment) {
            return vk::AttachmentDescription()
                .setFormat(mapRenderFormat(attachment.format, swapchain.format))
                .setLoadOp(mapLoadOp(attachment.loadOp))
                .setStoreOp(mapStoreOp(attachment.storeOp))
                .setInitialLayout(mapLayout(attachment.initialLayout))
                .setFinalLayout(mapLayout(attachment.finalLayout));
        });

        auto refsList = std::list<std::vector<vk::AttachmentReference>>();
        auto subpasses = std::vector<vk::SubpassDescription>(graph.passes.size());
        auto dependencies = std::vector<vk::SubpassDependency>();

        for (std::uint32_t passIdx = 0; passIdx < graph.passes.size(); passIdx++) {
            auto pass = graph.passes.at(passIdx);

            const auto inputCount = pass.inputTargets.size();
            const auto colorCount = pass.colorTargets.size();
            const auto depthCount = pass.depthStencilTarget.has_value() ? 1 : 0;
            auto &refs = refsList.emplace_back(inputCount + colorCount + depthCount);

            for (std::uint32_t idx = 0; idx < inputCount; idx++) {
                refs[idx] = vk::AttachmentReference()
                                .setAttachment(pass.inputTargets.at(idx))
                                .setLayout(vk::ImageLayout::eShaderReadOnlyOptimal);
            }

            for (std::uint32_t idx = 0; idx < colorCount; idx++) {
                refs[inputCount + idx] = vk::AttachmentReference()
                                             .setAttachment(pass.colorTargets.at(idx))
                                             .setLayout(vk::ImageLayout::eColorAttachmentOptimal);
            }

            if (pass.depthStencilTarget.has_value()) {
                refs[inputCount + colorCount] = vk::AttachmentReference()
                                                    .setAttachment(*pass.depthStencilTarget)
                                                    .setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);
            }

            const auto ptr = refs.data();
            subpasses[passIdx] = vk::SubpassDescription()
                                     .setInputAttachmentCount(inputCount)
                                     .setPInputAttachments(inputCount > 0 ? &ptr[0] : nullptr)
                                     .setColorAttachmentCount(colorCount)
                                     .setPColorAttachments(colorCount > 0 ? &ptr[inputCount] : nullptr)
                                     .setPDepthStencilAttachment(
                                         pass.depthStencilTarget.has_value() ? &ptr[inputCount + colorCount] : nullptr
                                     );

            if (!pass.dependsOn.empty()) {
                std::ranges::for_each(pass.dependsOn, [&](const std::uint32_t &srcIdx) {
                    dependencies.push_back(
                        vk::SubpassDependency()
                            .setSrcSubpass(srcIdx)
                            .setDstSubpass(passIdx)
                            .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
                            .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
                            .setSrcAccessMask(
                                vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eColorAttachmentRead
                            )
                            .setDstAccessMask(
                                vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eColorAttachmentRead
                            )
                    );
                });
            } else {
                dependencies.push_back(
                    vk::SubpassDependency()
                        .setSrcSubpass(VK_SUBPASS_EXTERNAL)
                        .setDstSubpass(passIdx)
                        .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
                        .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
                        .setSrcAccessMask(
                            vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eColorAttachmentRead
                        )
                        .setDstAccessMask(
                            vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eColorAttachmentRead
                        )
                );
            }
        }

        const auto createInfo = vk::RenderPassCreateInfo()
                                    .setAttachments(attachments)
                                    .setSubpasses(subpasses)
                                    .setDependencies(dependencies);

        return this->_logicalDeviceProvider->getLogicalDevice().handle->createRenderPassUnique(createInfo);
    }

    vk::UniqueFramebuffer VkInternalObjectFactory::makeFramebuffer(
        const vk::RenderPass &renderPass, const std::vector<vk::ImageView> &imageViews, const std::uint32_t width,
        const std::uint32_t height
    ) {
        const auto createInfo = vk::FramebufferCreateInfo()
                                    .setRenderPass(renderPass)
                                    .setAttachments(imageViews)
                                    .setWidth(width)
                                    .setHeight(height)
                                    .setLayers(1);

        return this->_logicalDeviceProvider->getLogicalDevice().handle->createFramebufferUnique(createInfo);
    }
}
