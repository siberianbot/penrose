#ifndef PENROSE_BUILTIN_RENDERING_VK_IMAGE_FACTORY_HPP
#define PENROSE_BUILTIN_RENDERING_VK_IMAGE_FACTORY_HPP

#include <Penrose/Rendering/ImageFactory.hpp>

namespace Penrose {

    class ResourceSet;
    class DeviceContext;

    class VkImageFactory : public ImageFactory {
    public:
        explicit VkImageFactory(ResourceSet *resources);
        ~VkImageFactory() override = default;

        [[nodiscard]] Image *makeImage(ImageFormat format, std::uint32_t width, std::uint32_t height) override;

    private:
        DeviceContext *_deviceContext;
    };
}

#endif // PENROSE_BUILTIN_RENDERING_VK_IMAGE_FACTORY_HPP
