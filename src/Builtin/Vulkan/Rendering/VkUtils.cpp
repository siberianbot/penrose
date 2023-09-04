#include "VkUtils.hpp"

#include <Penrose/Common/EngineError.hpp>

namespace Penrose {

    vk::DescriptorType toVkDescriptorType(const PipelineLayoutBindingType &type) {
        switch (type) {
            case PipelineLayoutBindingType::Sampler:
                return vk::DescriptorType::eCombinedImageSampler;

            case PipelineLayoutBindingType::InputAttachment:
                return vk::DescriptorType::eInputAttachment;

            case PipelineLayoutBindingType::UniformBuffer:
                return vk::DescriptorType::eUniformBuffer;

            default:
                throw EngineError("Binding type is not supported");
        }
    }

    vk::ShaderStageFlags toVkShaderStageFlags(const PipelineShaderStageType &type) {
        switch (type) {
            case PipelineShaderStageType::Vertex:
                return vk::ShaderStageFlagBits::eVertex;

            case PipelineShaderStageType::Fragment:
                return vk::ShaderStageFlagBits::eFragment;

            default:
                throw EngineError("Shader stage type is not supported");
        }
    }

    vk::ShaderStageFlagBits toVkShaderStageFlagBits(const PipelineShaderStageType &type) {
        switch (type) {
            case PipelineShaderStageType::Vertex:
                return vk::ShaderStageFlagBits::eVertex;

            case PipelineShaderStageType::Fragment:
                return vk::ShaderStageFlagBits::eFragment;

            default:
                throw EngineError("Shader stage type is not supported");
        }
    }

    vk::VertexInputRate toVkVertexInputRate(const PipelineBindingInputRate &inputRate) {
        switch (inputRate) {
            case PipelineBindingInputRate::Vertex:
                return vk::VertexInputRate::eVertex;

            case PipelineBindingInputRate::Instance:
                return vk::VertexInputRate::eInstance;

            default:
                throw EngineError("Input rate is not supported");
        }
    }

    vk::BufferUsageFlags toVkBufferUsageFlags(const BufferType &type) {
        switch (type) {
            case BufferType::Staging:
                return vk::BufferUsageFlagBits::eTransferSrc;

            case BufferType::Uniform:
                return vk::BufferUsageFlagBits::eUniformBuffer;

            case BufferType::Vertex:
                return vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer;

            case BufferType::Index:
                return vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer;

            default:
                throw EngineError("Buffer type is not supported");
        }
    }

    vk::ImageUsageFlags toVkImageUsageFlags(const RenderTargetType &type) {
        auto usage = vk::ImageUsageFlags();

        if (type & RenderTargetType::Input) {
            usage |= vk::ImageUsageFlagBits::eInputAttachment;
        }

        if (type & RenderTargetType::Color) {
            usage |= vk::ImageUsageFlagBits::eInputAttachment;
        }

        if (type & RenderTargetType::DepthStencil) {
            usage |= vk::ImageUsageFlagBits::eDepthStencilAttachment;
        }

        return usage;
    }

    vk::ImageAspectFlags toVkImageAspectFlags(const RenderTargetType &type) {
        auto aspect = vk::ImageAspectFlags();

        if (type & RenderTargetType::Color) {
            aspect |= vk::ImageAspectFlagBits::eColor;
        }

        if (type & RenderTargetType::DepthStencil) {
            aspect |= vk::ImageAspectFlagBits::eDepth;
        }

        return aspect;
    }

    vk::Format toVkFormat(const PipelineBindingAttributeFormat &format) {
        switch (format) {
            case PipelineBindingAttributeFormat::Vec2:
                return vk::Format::eR32G32Sfloat;

            case PipelineBindingAttributeFormat::Vec3:
                return vk::Format::eR32G32B32Sfloat;

            default:
                throw EngineError("Binding attribute format is not supported");
        }
    }

    vk::Format toVkFormat(const ImageFormat &format) {
        switch (format) {
            case ImageFormat::RGBA:
                return vk::Format::eR8G8B8A8Srgb;

            default:
                throw EngineError("Image format is not supported");
        }
    }

    std::optional<vk::Format> toVkFormat(const RenderFormat &format) {
        switch (format) {
            case RenderFormat::None:
                return std::nullopt;

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

    vk::AttachmentLoadOp toVkAttachmentLoadOp(const RenderAttachmentLoadOp &loadOp) {
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

    vk::AttachmentStoreOp toVkAttachmentStoreOp(const RenderAttachmentStoreOp &storeOp) {
        switch (storeOp) {
            case RenderAttachmentStoreOp::DontCare:
                return vk::AttachmentStoreOp::eDontCare;

            case RenderAttachmentStoreOp::Store:
                return vk::AttachmentStoreOp::eStore;

            default:
                throw EngineError("Render attachment store op is not supported");
        }
    }

    vk::ImageLayout toVkImageLayout(const RenderAttachmentLayout &layout) {
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

    vk::SamplerAddressMode toVkSamplerAddressMode(const SamplerAddressMode &addressMode) {
        switch (addressMode) {
            case SamplerAddressMode::Repeat:
                return vk::SamplerAddressMode::eRepeat;

            case SamplerAddressMode::MirroredRepeat:
                return vk::SamplerAddressMode::eMirroredRepeat;

            case SamplerAddressMode::ClampToEdge:
                return vk::SamplerAddressMode::eClampToEdge;

            case SamplerAddressMode::ClampToBorder:
                return vk::SamplerAddressMode::eClampToBorder;

            default:
                throw EngineError("Sampler address mode is not supported");
        }
    }

    vk::BorderColor toVkBorderColor(const SamplerBorderColor &borderColor) {
        switch (borderColor) {
            case SamplerBorderColor::Transparent:
                return vk::BorderColor::eFloatTransparentBlack;

            case SamplerBorderColor::Black:
                return vk::BorderColor::eFloatOpaqueBlack;

            case SamplerBorderColor::White:
                return vk::BorderColor::eFloatOpaqueWhite;

            default:
                throw EngineError("Sampler border color is not supported");
        }
    }

    vk::Filter toVkFilter(const SamplerFilter &filter) {
        switch (filter) {
            case SamplerFilter::Linear:
                return vk::Filter::eLinear;

            case SamplerFilter::Nearest:
                return vk::Filter::eNearest;

            default:
                throw EngineError("Sampler filter is not supported");
        }
    }
}
