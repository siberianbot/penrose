#ifndef PENROSE_BUILTIN_ASSETS_VK_IMAGE_ASSET_FACTORY_HPP
#define PENROSE_BUILTIN_ASSETS_VK_IMAGE_ASSET_FACTORY_HPP

#include <Penrose/Assets/ImageAssetFactory.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ResourceSet;
    class BufferFactory;
    class DeviceContext;
    class ImageFactory;

    class VkImageAssetFactory : public Resource, public ImageAssetFactory {
    public:
        explicit VkImageAssetFactory(ResourceSet *resources);
        ~VkImageAssetFactory() override = default;

        [[nodiscard]] ImageAsset *makeImage(ImageFormat format, std::uint32_t width, std::uint32_t height,
                                            void *data) override;

    private:
        BufferFactory *_bufferFactory;
        DeviceContext *_deviceContext;
        ImageFactory *_imageFactory;
    };
}

#endif // PENROSE_BUILTIN_ASSETS_VK_IMAGE_ASSET_FACTORY_HPP
