#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_UTILS_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_UTILS_HPP

#include <optional>

#include <vulkan/vulkan.hpp>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Rendering/Graph/AttachmentInfo.hpp>
#include <Penrose/Rendering/Graph/TargetInfo.hpp>
#include <Penrose/Rendering/ImageFormat.hpp>
#include <Penrose/Rendering/Objects/Buffer.hpp>
#include <Penrose/Rendering/Objects/PipelineInfo.hpp>
#include <Penrose/Rendering/Objects/Sampler.hpp>
#include <Penrose/Rendering/Objects/Shader.hpp>
#include <Penrose/Rendering/RenderFormat.hpp>
#include <Penrose/Types/Rect.hpp>
#include <Penrose/Types/Size.hpp>

#include "src/Builtin/Vulkan/Rendering/Objects/VkBuffer.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkImage.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkPipeline.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkSampler.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkShader.hpp"
#include "src/Builtin/Vulkan/Rendering/VkCommandRecorder.hpp"

namespace Penrose {

    [[nodiscard]] constexpr vk::BufferUsageFlags mapBufferType(const BufferType type) {
        switch (type) {
            case BufferType::Uniform:
                return vk::BufferUsageFlagBits::eUniformBuffer;

            case BufferType::Vertex:
                return vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer;

            case BufferType::Index:
                return vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer;

            default:
                throw EngineError::notImplemented();
        }
    }

    [[nodiscard]] constexpr vk::Format mapRenderFormat(
        const std::optional<RenderFormat> format, const vk::Format defaultFormat
    ) {
        if (!format.has_value()) {
            return defaultFormat;
        }

        switch (*format) {
            case RenderFormat::R8UNorm:
                return vk::Format::eR8Unorm;

            case RenderFormat::RGBA8UNorm:
                return vk::Format::eR8G8B8A8Unorm;

            case RenderFormat::RGBA16Float:
                return vk::Format::eR16G16B16A16Sfloat;

            case RenderFormat::D32Float:
                return vk::Format::eD32Sfloat;

            default:
                throw EngineError::notImplemented();
        }
    }

    [[nodiscard]] constexpr vk::AttachmentLoadOp mapLoadOp(const AttachmentLoadOp loadOp) {
        switch (loadOp) {
            case AttachmentLoadOp::DontCare:
                return vk::AttachmentLoadOp::eDontCare;

            case AttachmentLoadOp::Load:
                return vk::AttachmentLoadOp::eLoad;

            case AttachmentLoadOp::Clear:
                return vk::AttachmentLoadOp::eClear;

            default:
                throw EngineError::notImplemented();
        }
    }

    [[nodiscard]] constexpr vk::AttachmentStoreOp mapStoreOp(const AttachmentStoreOp storeOp) {
        switch (storeOp) {
            case AttachmentStoreOp::DontCare:
                return vk::AttachmentStoreOp::eDontCare;

            case AttachmentStoreOp::Store:
                return vk::AttachmentStoreOp::eStore;

            default:
                throw EngineError::notImplemented();
        }
    }

    [[nodiscard]] constexpr vk::ImageLayout mapLayout(const AttachmentLayout layout) {
        switch (layout) {
            case AttachmentLayout::Undefined:
                return vk::ImageLayout::eUndefined;

            case AttachmentLayout::ColorAttachment:
                return vk::ImageLayout::eColorAttachmentOptimal;

            case AttachmentLayout::DepthStencilAttachment:
                return vk::ImageLayout::eDepthStencilAttachmentOptimal;

            case AttachmentLayout::Present:
                return vk::ImageLayout::ePresentSrcKHR;

            default:
                throw EngineError::notImplemented();
        }
    }

    [[nodiscard]] constexpr vk::Format mapImageFormat(const ImageFormat format) {
        switch (format) {
            case ImageFormat::RGBA:
                return vk::Format::eR8G8B8A8Srgb;

            default:
                throw EngineError::notImplemented();
        }
    }

    [[nodiscard]] constexpr vk::Extent2D mapSize(const std::optional<Size> &size, const vk::Extent2D defaultSize) {
        if (!size.has_value()) {
            return defaultSize;
        }

        vk::Extent2D result;
        std::tie(result.width, result.height) = *size;

        return result;
    }

    [[nodiscard]] constexpr vk::ImageUsageFlags mapTargetUsage(const TargetUsageFlags usage) {
        auto vkUsage = vk::ImageUsageFlags();

        if (usage & TargetUsage::Input) {
            vkUsage |= vk::ImageUsageFlagBits::eInputAttachment;
        }

        if (usage & TargetUsage::Color) {
            vkUsage |= vk::ImageUsageFlagBits::eInputAttachment;
        }

        if (usage & TargetUsage::DepthStencil) {
            vkUsage |= vk::ImageUsageFlagBits::eDepthStencilAttachment;
        }

        return vkUsage;
    }

    [[nodiscard]] constexpr vk::ImageAspectFlags mapTargetUsageAspect(const TargetUsageFlags usage) {
        auto aspect = vk::ImageAspectFlags();

        if (usage & TargetUsage::Color) {
            aspect |= vk::ImageAspectFlagBits::eColor;
        }

        if (usage & TargetUsage::DepthStencil) {
            aspect |= vk::ImageAspectFlagBits::eDepth;
        }

        return aspect;
    }

    [[nodiscard]] constexpr vk::Rect2D mapRect(const IntRect rect) {
        return vk::Rect2D().setOffset(vk::Offset2D(rect.x, rect.y)).setExtent(vk::Extent2D(rect.w, rect.h));
    }

    [[nodiscard]] constexpr vk::ClearValue mapClearValue(const AttachmentClearValueInfo &clearValue) {
        return vk::ClearValue()
            .setColor(clearValue.color)
            .setDepthStencil(vk::ClearDepthStencilValue(clearValue.depth, clearValue.stencil));
    }

    [[nodiscard]] constexpr vk::SamplerAddressMode mapAddressMode(const SamplerAddressMode addressMode) {
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
                throw EngineError::notImplemented();
        }
    }

    [[nodiscard]] constexpr vk::Filter mapFilteringMode(const SamplerFilteringMode filteringMode) {
        switch (filteringMode) {
            case SamplerFilteringMode::Linear:
                return vk::Filter::eLinear;

            case SamplerFilteringMode::Nearest:
                return vk::Filter::eNearest;

            default:
                throw EngineError::notImplemented();
        }
    }

    [[nodiscard]] constexpr vk::BorderColor mapBorderColor(const SamplerBorderColor borderColor) {
        switch (borderColor) {
            case SamplerBorderColor::Transparent:
                return vk::BorderColor::eFloatTransparentBlack;

            case SamplerBorderColor::Black:
                return vk::BorderColor::eFloatOpaqueBlack;

            case SamplerBorderColor::White:
                return vk::BorderColor::eFloatOpaqueWhite;

            default:
                throw EngineError::notImplemented();
        }
    }

    [[nodiscard]] constexpr vk::DescriptorType mapObjectType(const PipelineObjectType objectType) {
        switch (objectType) {
            case PipelineObjectType::Sampler:
                return vk::DescriptorType::eCombinedImageSampler;

            case PipelineObjectType::InputAttachment:
                return vk::DescriptorType::eInputAttachment;

            case PipelineObjectType::UniformBuffer:
                return vk::DescriptorType::eUniformBuffer;

            default:
                throw EngineError::notImplemented();
        }
    }

    [[nodiscard]] constexpr vk::ShaderStageFlagBits mapShaderStage(const PipelineShaderStage stage) {
        switch (stage) {
            case PipelineShaderStage::Vertex:
                return vk::ShaderStageFlagBits::eVertex;

            case PipelineShaderStage::Fragment:
                return vk::ShaderStageFlagBits::eFragment;

            default:
                throw EngineError::notImplemented();
        }
    }

    [[nodiscard]] constexpr vk::VertexInputRate mapInputRate(const PipelineInputRate inputRate) {
        switch (inputRate) {
            case PipelineInputRate::Vertex:
                return vk::VertexInputRate::eVertex;

            case PipelineInputRate::Instance:
                return vk::VertexInputRate::eInstance;

            default:
                throw EngineError::notImplemented();
        }
    }

    [[nodiscard]] constexpr vk::Format mapAttributeFormat(const PipelineInputAttributeFormat format) {
        switch (format) {
            case PipelineInputAttributeFormat::UInt:
                return vk::Format::eR32Uint;

            case PipelineInputAttributeFormat::Vec2:
                return vk::Format::eR32G32Sfloat;

            case PipelineInputAttributeFormat::Vec3:
                return vk::Format::eR32G32B32Sfloat;

            case PipelineInputAttributeFormat::Mat4:
                return vk::Format::eR32G32B32A32Sfloat;

            default:
                throw EngineError::notImplemented();
        }
    }

    [[nodiscard]] constexpr VkBuffer *asVkBuffer(Buffer *buffer) {
        return static_cast<VkBuffer *>(buffer); // NOLINT(*-pro-type-static-cast-downcast)
    }

    [[nodiscard]] constexpr VkCommandRecorder *asVkCommandRecorder(CommandRecorder *recorder) {
        return static_cast<VkCommandRecorder *>(recorder); // NOLINT(*-pro-type-static-cast-downcast)
    }

    [[nodiscard]] constexpr VkImage *asVkImage(Image *image) {
        return static_cast<VkImage *>(image); // NOLINT(*-pro-type-static-cast-downcast)
    }

    [[nodiscard]] constexpr VkPipeline *asVkPipeline(Pipeline *pipeline) {
        return static_cast<VkPipeline *>(pipeline); // NOLINT(*-pro-type-static-cast-downcast)
    }

    [[nodiscard]] constexpr VkSampler *asVkSampler(Sampler *sampler) {
        return static_cast<VkSampler *>(sampler); // NOLINT(*-pro-type-static-cast-downcast)
    }

    [[nodiscard]] constexpr VkShader *asVkShader(Shader *shader) {
        return static_cast<VkShader *>(shader); // NOLINT(*-pro-type-static-cast-downcast)
    }
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_UTILS_HPP
