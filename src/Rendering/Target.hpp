#ifndef PENROSE_RENDERING_TARGET_HPP
#define PENROSE_RENDERING_TARGET_HPP

#include <cstdint>

#include <vulkan/vulkan.hpp>

namespace Penrose {

    class DeviceContext;
    class DeviceMemoryAllocator;
    class PresentContext;

    class Target {
    public:
        virtual ~Target() = default;

        [[nodiscard]] virtual const vk::ImageView &getView(const std::uint32_t &imageIdx) const = 0;
    };

    class SwapchainTarget : public Target {
    private:
        PresentContext *_presentContext;

    public:
        explicit SwapchainTarget(PresentContext *presentContext);
        ~SwapchainTarget() override = default;

        [[nodiscard]] const vk::ImageView &getView(const std::uint32_t &imageIdx) const override;
    };

    class ImageTarget : public Target {
    private:
        DeviceContext *_deviceContext;
        DeviceMemoryAllocator *_deviceMemoryAllocator;
        vk::Image _image;
        vk::ImageView _imageView;

    public:
        ImageTarget(DeviceContext *deviceContext,
                    DeviceMemoryAllocator *deviceMemoryAllocator,
                    const vk::Image &image,
                    const vk::ImageView &imageView);
        ~ImageTarget() override;

        [[nodiscard]] const vk::ImageView &getView(const std::uint32_t &) const override {
            return this->_imageView;
        }
    };
}

#endif // PENROSE_RENDERING_TARGET_HPP
