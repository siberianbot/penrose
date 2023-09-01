#ifndef PENROSE_BUILTIN_VULKAN_ASSETS_VK_IMAGE_ASSET_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_ASSETS_VK_IMAGE_ASSET_FACTORY_HPP

#include <Penrose/Assets/ImageAssetFactory.hpp>
#include <Penrose/Rendering/BufferFactory.hpp>
#include <Penrose/Rendering/ImageFactory.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/Rendering/DeviceContext.hpp"

namespace Penrose {

    class ResourceSet;

    class VkImageAssetFactory : public Resource, public ImageAssetFactory {
    public:
        explicit VkImageAssetFactory(ResourceSet *resources);
        ~VkImageAssetFactory() override = default;

        [[nodiscard]] ImageAsset *makeImage(ImageFormat format, std::uint32_t width, std::uint32_t height,
                                            void *data) override;

    private:
        Lazy<BufferFactory> _bufferFactory;
        Lazy<DeviceContext> _deviceContext;
        Lazy<ImageFactory> _imageFactory;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_ASSETS_VK_IMAGE_ASSET_FACTORY_HPP
