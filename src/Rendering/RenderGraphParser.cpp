#include "RenderGraphParser.hpp"

#include <algorithm>

#include <vulkan/vulkan.hpp>

#include "src/Rendering/PresentContext.hpp"
#include "src/Rendering/RenderGraph.hpp"
#include "src/Utils/OptionalUtils.hpp"

namespace Penrose {

    RenderGraphParser::RenderGraphParser(PresentContext *presentContext)
            : _presentContext(presentContext) {
        //
    }

    RenderGraphParser::~RenderGraphParser() {
        for (const auto &finalizer: this->_finalizers) {
            finalizer();
        }
    }

    template<>
    vk::ImageUsageFlags RenderGraphParser::parse<RenderTargetType, vk::ImageUsageFlags>(
            const RenderTargetType &type) {
        auto usage = vk::ImageUsageFlags();

        if (type & RenderTargetType::Input) {
            usage &= vk::ImageUsageFlagBits::eInputAttachment;
        }

        if (type & RenderTargetType::Color) {
            usage &= vk::ImageUsageFlagBits::eInputAttachment;
        }

        if (type & RenderTargetType::DepthStencil) {
            usage &= vk::ImageUsageFlagBits::eDepthStencilAttachment;
        }

        return usage;
    }

    template<>
    vk::ImageAspectFlags RenderGraphParser::parse<RenderTargetType, vk::ImageAspectFlags>(
            const RenderTargetType &type) {
        auto aspect = vk::ImageAspectFlags();

        if (type & RenderTargetType::Color) {
            aspect &= vk::ImageAspectFlagBits::eColor;
        }

        if (type & RenderTargetType::DepthStencil) {
            aspect &= vk::ImageAspectFlagBits::eDepth;
        }

        return aspect;
    }

    template<>
    vk::Format RenderGraphParser::parse<RenderFormat, vk::Format>(
            const RenderFormat &format) {
        switch (format) {
            case RenderFormat::R8UNorm:
                return vk::Format::eR8Unorm;

            case RenderFormat::RGBA8UNorm:
                return vk::Format::eR8G8B8A8Unorm;

            case RenderFormat::RGBA16Float:
                return vk::Format::eR16G16B16A16Sfloat;

            case RenderFormat::D32Float:
                return vk::Format::eD32Sfloat;

            default:
                throw EngineError("Render target format is not supported");
        }
    }

    template<>
    vk::ImageCreateInfo RenderGraphParser::parse<RenderTarget, vk::ImageCreateInfo>(
            const RenderTarget &target) {
        auto type = orElseThrow(target.type, EngineError("Type is required for image render target"));
        auto usage = this->parse<RenderTargetType, vk::ImageUsageFlags>(type);

        auto format = map(target.format, [&](RenderFormat format) {
            return this->parse<RenderFormat, vk::Format>(format);
        }).value_or(this->_presentContext->getSwapchainFormat());

        auto extent = map(target.size, [](Size size) {
            auto [w, h] = size;
            return vk::Extent3D(w, h, 1);
        }).value_or(vk::Extent3D(this->_presentContext->getSwapchainExtent(), 1));

        return vk::ImageCreateInfo()
                .setUsage(usage)
                .setFormat(format)
                .setExtent(extent);
    }

    template<>
    vk::ImageViewCreateInfo RenderGraphParser::parse<RenderTarget, vk::ImageViewCreateInfo>(
            const RenderTarget &target) {
        auto type = orElseThrow(target.type, EngineError("Type is required for image render target"));

        auto format = map(target.format, [&](RenderFormat format) {
            return this->parse<RenderFormat, vk::Format>(format);
        }).value_or(this->_presentContext->getSwapchainFormat());

        auto aspect = this->parse<RenderTargetType, vk::ImageAspectFlags>(type);

        auto subresourceRange = vk::ImageSubresourceRange()
                .setBaseMipLevel(0)
                .setLevelCount(1)
                .setBaseArrayLayer(0)
                .setLayerCount(1)
                .setAspectMask(aspect);

        return vk::ImageViewCreateInfo()
                .setViewType(vk::ImageViewType::e2D)
                .setFormat(format)
                .setSubresourceRange(subresourceRange);
    }

    template<>
    vk::AttachmentLoadOp RenderGraphParser::parse<RenderAttachmentLoadOp, vk::AttachmentLoadOp>(
            const RenderAttachmentLoadOp &loadOp) {
        switch (loadOp) {
            case RenderAttachmentLoadOp::DontCare:
                return vk::AttachmentLoadOp::eDontCare;

            case RenderAttachmentLoadOp::Load:
                return vk::AttachmentLoadOp::eLoad;

            case RenderAttachmentLoadOp::Clear:
                return vk::AttachmentLoadOp::eClear;

            default:
                throw EngineError("Render attachment load op is not supported");
        }
    }

    template<>
    vk::AttachmentStoreOp RenderGraphParser::parse<RenderAttachmentStoreOp, vk::AttachmentStoreOp>(
            const RenderAttachmentStoreOp &storeOp) {
        switch (storeOp) {
            case RenderAttachmentStoreOp::DontCare:
                return vk::AttachmentStoreOp::eDontCare;

            case RenderAttachmentStoreOp::Store:
                return vk::AttachmentStoreOp::eStore;

            default:
                throw EngineError("Render attachment store op is not supported");
        }
    }

    template<>
    vk::ImageLayout RenderGraphParser::parse<RenderAttachmentLayout, vk::ImageLayout>(
            const RenderAttachmentLayout &layout) {
        switch (layout) {
            case RenderAttachmentLayout::Undefined:
                return vk::ImageLayout::eUndefined;

            case RenderAttachmentLayout::ColorAttachment:
                return vk::ImageLayout::eColorAttachmentOptimal;

            case RenderAttachmentLayout::DepthStencilAttachment:
                return vk::ImageLayout::eDepthStencilAttachmentOptimal;

            case RenderAttachmentLayout::Present:
                return vk::ImageLayout::ePresentSrcKHR;

            default:
                throw EngineError("Render attachment layout is not supported");
        }
    }

    template<>
    vk::ClearValue RenderGraphParser::parse<RenderAttachmentClearValue, vk::ClearValue>(
            const RenderAttachmentClearValue &clearValue) {
        return vk::ClearValue()
                .setColor(clearValue.color)
                .setDepthStencil({clearValue.depth, clearValue.stencil});
    }

    template<>
    vk::AttachmentDescription RenderGraphParser::parse<RenderAttachment, vk::AttachmentDescription>(
            const RenderAttachment &attachment) {
        auto format = map(attachment.format, [&](RenderFormat format) {
            return this->parse<RenderFormat, vk::Format>(format);
        }).value_or(this->_presentContext->getSwapchainFormat());

        return vk::AttachmentDescription()
                .setFormat(format)
                .setLoadOp(this->parse<RenderAttachmentLoadOp, vk::AttachmentLoadOp>(attachment.loadOp))
                .setStoreOp(this->parse<RenderAttachmentStoreOp, vk::AttachmentStoreOp>(attachment.storeOp))
                .setInitialLayout(this->parse<RenderAttachmentLayout, vk::ImageLayout>(attachment.initialLayout))
                .setFinalLayout(this->parse<RenderAttachmentLayout, vk::ImageLayout>(attachment.finalLayout));
    }

    template<>
    vk::SubpassDescription RenderGraphParser::parse<RenderPass, vk::SubpassDescription>(
            const RenderPass &pass) {
        auto inputAttachments = new vk::AttachmentReference[pass.inputAttachments.size()];
        for (std::uint32_t idx = 0; idx < pass.inputAttachments.size(); idx++) {
            inputAttachments[idx] = vk::AttachmentReference(pass.inputAttachments[idx],
                                                            vk::ImageLayout::eShaderReadOnlyOptimal);
        }

        auto colorAttachments = new vk::AttachmentReference[pass.colorAttachments.size()];
        for (std::uint32_t idx = 0; idx < pass.colorAttachments.size(); idx++) {
            colorAttachments[idx] = vk::AttachmentReference(pass.colorAttachments[idx],
                                                            vk::ImageLayout::eColorAttachmentOptimal);
        }

        auto depthAttachment = map(pass.depthStencilAttachment,
                                   [&](const std::uint32_t &attachmentIdx) {
                                       return new vk::AttachmentReference(attachmentIdx,
                                                                          vk::ImageLayout::eDepthStencilAttachmentOptimal);
                                   }).value_or(nullptr);

        this->_finalizers.emplace_back([inputAttachments, colorAttachments, depthAttachment]() {
            delete[] inputAttachments;
            delete[] colorAttachments;
            delete depthAttachment;
        });

        return vk::SubpassDescription()
                .setInputAttachmentCount(pass.inputAttachments.size())
                .setPInputAttachments(inputAttachments)
                .setColorAttachmentCount(pass.colorAttachments.size())
                .setPColorAttachments(colorAttachments)
                .setPDepthStencilAttachment(depthAttachment);
    }

    template<>
    vk::RenderPassCreateInfo RenderGraphParser::parse<RenderSubgraph, vk::RenderPassCreateInfo>(
            const RenderSubgraph &subgraph) {
        constexpr auto makeDependency = [](const std::uint32_t &srcIdx, const std::uint32_t &dstIdx) {
            return vk::SubpassDependency()
                    .setSrcSubpass(srcIdx)
                    .setDstSubpass(dstIdx)
                    .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
                    .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
                    .setSrcAccessMask(vk::AccessFlagBits::eColorAttachmentWrite |
                                      vk::AccessFlagBits::eColorAttachmentRead)
                    .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite |
                                      vk::AccessFlagBits::eColorAttachmentRead);
        };

        auto attachments = new vk::AttachmentDescription[subgraph.attachments.size()];
        for (std::uint32_t idx = 0; idx < subgraph.attachments.size(); idx++) {
            attachments[idx] = this->parse<RenderAttachment, vk::AttachmentDescription>(subgraph.attachments.at(idx));
        }

        auto subpasses = new vk::SubpassDescription[subgraph.passes.size()];
        auto dependencies = new std::vector<vk::SubpassDependency>();
        for (std::uint32_t idx = 0; idx < subgraph.passes.size(); idx++) {
            const auto pass = subgraph.passes[idx];

            subpasses[idx] = this->parse<RenderPass, vk::SubpassDescription>(pass);

            if (!pass.dependsOn.empty()) {
                std::for_each(pass.dependsOn.begin(), pass.dependsOn.end(), [&](const std::uint32_t &srcIdx) {
                    dependencies->push_back(makeDependency(srcIdx, idx));
                });
            } else {
                dependencies->push_back(makeDependency(VK_SUBPASS_EXTERNAL, idx));
            }
        }

        this->_finalizers.emplace_back([attachments, subpasses, dependencies]() {
            delete[] attachments;
            delete[] subpasses;
            delete dependencies;
        });

        return vk::RenderPassCreateInfo()
                .setAttachmentCount(subgraph.attachments.size())
                .setPAttachments(attachments)
                .setSubpassCount(subgraph.passes.size())
                .setPSubpasses(subpasses)
                .setDependencyCount(dependencies->size())
                .setPDependencies(dependencies->data());
    }
}