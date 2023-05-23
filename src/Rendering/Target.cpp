#include "Target.hpp"

#include "src/Rendering/DeviceContext.hpp"
#include "src/Rendering/DeviceMemoryAllocator.hpp"
#include "src/Rendering/PresentContext.hpp"

namespace Penrose {

    SwapchainTarget::SwapchainTarget(PresentContext *presentContext)
            : _presentContext(presentContext) {
        //
    }

    const vk::ImageView &SwapchainTarget::getView(const uint32_t &imageIdx) const {
        return this->_presentContext->getSwapchainImageViews().at(imageIdx);
    }

    ImageTarget::ImageTarget(DeviceContext *deviceContext,
                             DeviceMemoryAllocator *deviceMemoryAllocator,
                             const vk::Image &image,
                             const vk::ImageView &imageView)
            : _deviceContext(deviceContext),
              _deviceMemoryAllocator(deviceMemoryAllocator),
              _image(image),
              _imageView(imageView) {
        //
    }

    ImageTarget::~ImageTarget() {
        this->_deviceContext->getLogicalDevice().destroy(this->_imageView);

        this->_deviceMemoryAllocator->freeFor(this->_image);
        this->_deviceContext->getLogicalDevice().destroy(this->_image);
    }
}
