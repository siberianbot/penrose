#include "RenderGraphParser.hpp"

#include <algorithm>
#include <memory>

#include <vulkan/vulkan.hpp>

#include "src/Rendering/DeviceContext.hpp"
#include "src/Rendering/DeviceMemoryAllocator.hpp"
#include "src/Rendering/Pass.hpp"
#include "src/Rendering/PresentContext.hpp"
#include "src/Rendering/Target.hpp"
#include "src/Utils/OptionalUtils.hpp"

namespace Penrose {

    RenderGraphParser::Context::Context(DeviceContext *deviceContext,
                                        DeviceMemoryAllocator *deviceMemoryAllocator,
                                        PresentContext *presentContext,
                                        const RenderGraph *graph)
            : deviceContext(deviceContext),
              deviceMemoryAllocator(deviceMemoryAllocator),
              presentContext(presentContext),
              graph(graph) {
        //
    }

    RenderGraphParser::Context::~Context() {
        for (const auto &finalizer: this->finalizers) {
            finalizer();
        }
    }

    RenderGraphParser::RenderGraphParser(DeviceContext *deviceContext,
                                         DeviceMemoryAllocator *deviceMemoryAllocator,
                                         PresentContext *presentContext,
                                         const RenderGraph *graph)
            : _context({deviceContext, deviceMemoryAllocator, presentContext, graph}) {
        //
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
    vk::Format RenderGraphParser::parse<RenderTargetFormat, vk::Format>(
            const RenderTargetFormat &format) {
        switch (format) {
            case RenderTargetFormat::R8UNorm:
                return vk::Format::eR8Unorm;

            case RenderTargetFormat::RGBA8UNorm:
                return vk::Format::eR8G8B8A8Unorm;

            case RenderTargetFormat::RGBA16Float:
                return vk::Format::eR16G16B16A16Sfloat;

            case RenderTargetFormat::D32Float:
                return vk::Format::eD32Sfloat;

            default:
                throw EngineError("Render target format is not supported");
        }
    }

    template<>
    std::unique_ptr<SwapchainTarget> RenderGraphParser::parse<RenderTarget, std::unique_ptr<SwapchainTarget>>(
            const RenderTarget &target) {
        if (target.source != RenderTargetSource::Swapchain) {
            throw EngineError("Attempt to process render target with non-swapchain source");
        }

        return std::make_unique<SwapchainTarget>(this->_context.presentContext);
    }

    template<>
    std::unique_ptr<ImageTarget> RenderGraphParser::parse<RenderTarget, std::unique_ptr<ImageTarget>>(
            const RenderTarget &target) {
        if (target.source != RenderTargetSource::Image) {
            throw EngineError("Attempt to process render target with non-image source");
        }

        if (!target.type.has_value()) {
            throw EngineError("Type is required for image render target");
        }

        auto type = target.type.value();
        auto usage = this->parse<RenderTargetType, vk::ImageUsageFlags>(type);
        auto aspect = this->parse<RenderTargetType, vk::ImageAspectFlags>(type);
        auto format = map(target.format, [&](RenderTargetFormat format) {
            return this->parse<RenderTargetFormat, vk::Format>(format);
        }).value_or(this->_context.presentContext->getSwapchainFormat());
        auto extent = map(target.size, [](Size size) {
            auto [w, h] = size;
            return vk::Extent3D(w, h, 1);
        }).value_or(vk::Extent3D(this->_context.presentContext->getSwapchainExtent(), 1));

        auto imageCreateInfo = vk::ImageCreateInfo()
                .setUsage(usage)
                .setFormat(format)
                .setExtent(extent);

        auto image = this->_context.deviceContext->getLogicalDevice().createImage(imageCreateInfo);

        this->_context.deviceMemoryAllocator->allocateDeviceLocalFor(image);

        auto imageViewCreateInfo = vk::ImageViewCreateInfo()
                .setImage(image)
                .setViewType(vk::ImageViewType::e2D)
                .setFormat(format)
                .setSubresourceRange(vk::ImageSubresourceRange()
                                             .setBaseMipLevel(0)
                                             .setLevelCount(1)
                                             .setBaseArrayLayer(0)
                                             .setLayerCount(1)
                                             .setAspectMask(aspect));

        auto imageView = this->_context.deviceContext->getLogicalDevice().createImageView(imageViewCreateInfo);

        return std::make_unique<ImageTarget>(this->_context.deviceContext,
                                             this->_context.deviceMemoryAllocator,
                                             image, imageView);
    }

    template<>
    std::unique_ptr<Target> RenderGraphParser::parse<RenderTarget, std::unique_ptr<Target>>(
            const RenderTarget &target) {
        switch (target.source) {
            case RenderTargetSource::Swapchain:
                return this->parse<RenderTarget, std::unique_ptr<SwapchainTarget>>(target);

            case RenderTargetSource::Image:
                return this->parse<RenderTarget, std::unique_ptr<ImageTarget>>(target);

            default:
                throw EngineError("Unknown source of render target");
        }
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
    vk::AttachmentDescription RenderGraphParser::parse<RenderAttachment, vk::AttachmentDescription>(
            const RenderAttachment &attachment) {
        auto target = this->_context.graph->targets.at(attachment.targetIdx);
        auto format = map(target.format, [&](RenderTargetFormat format) {
            return this->parse<RenderTargetFormat, vk::Format>(format);
        }).value_or(this->_context.presentContext->getSwapchainFormat());

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
        constexpr auto transformFunc = [](vk::ImageLayout layout) {
            return [layout](const std::uint32_t &attachmentIdx) {
                return vk::AttachmentReference(attachmentIdx, layout);
            };
        };

        auto inputAttachments = std::vector<vk::AttachmentReference>(pass.inputAttachments.size());
        std::transform(pass.inputAttachments.begin(), pass.inputAttachments.end(), inputAttachments.begin(),
                       transformFunc(vk::ImageLayout::eShaderReadOnlyOptimal));

        auto colorAttachments = std::vector<vk::AttachmentReference>(pass.colorAttachments.size());
        std::transform(pass.colorAttachments.begin(), pass.colorAttachments.end(), colorAttachments.begin(),
                       transformFunc(vk::ImageLayout::eColorAttachmentOptimal));

        auto depthAttachment = map(pass.depthStencilAttachment,
                                   [&](const std::uint32_t &attachmentIdx) {
                                       auto reference = new vk::AttachmentReference(
                                               attachmentIdx,
                                               vk::ImageLayout::eDepthStencilAttachmentOptimal);

                                       this->_context.finalizers.emplace_back([&reference]() {
                                           delete reference;
                                       });

                                       return reference;
                                   }).value_or(nullptr);

        return vk::SubpassDescription()
                .setInputAttachments(inputAttachments)
                .setColorAttachments(colorAttachments)
                .setPDepthStencilAttachment(depthAttachment);
    }

    template<>
    std::unique_ptr<Pass> RenderGraphParser::parse<RenderSubgraph, std::unique_ptr<Pass>>(
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

        auto attachments = std::vector<vk::AttachmentDescription>(subgraph.attachments.size());
        std::transform(subgraph.attachments.begin(), subgraph.attachments.end(), attachments.begin(),
                       [&](const RenderAttachment &attachment) {
                           return this->parse<RenderAttachment, vk::AttachmentDescription>(attachment);
                       });

        auto subpasses = std::vector<vk::SubpassDescription>(subgraph.passes.size());
        std::vector<vk::SubpassDependency> dependencies;

        for (std::uint32_t idx = 0; idx < subgraph.passes.size(); idx++) {
            const auto pass = subgraph.passes[idx];

            subpasses[idx] = this->parse<RenderPass, vk::SubpassDescription>(pass);

            std::for_each(pass.dependsOn.begin(), pass.dependsOn.end(), [&](const std::uint32_t &srcIdx) {
                dependencies.push_back(makeDependency(srcIdx, idx));
            });
        }

        auto renderPassCreateInfo = vk::RenderPassCreateInfo()
                .setAttachments(attachments)
                .setSubpasses(subpasses)
                .setDependencies(dependencies);

        auto renderPass = this->_context.deviceContext->getLogicalDevice().createRenderPass(renderPassCreateInfo);
        auto semaphore = this->_context.deviceContext->getLogicalDevice().createSemaphore(vk::SemaphoreCreateInfo());

        return std::make_unique<Pass>(this->_context.deviceContext, renderPass, semaphore);
    }
}