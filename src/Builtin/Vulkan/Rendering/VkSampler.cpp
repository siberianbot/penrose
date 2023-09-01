#include "VkSampler.hpp"

#include "src/Rendering/DeviceContext.hpp"

namespace Penrose {

    VkSampler::VkSampler(DeviceContext *deviceContext, vk::Sampler sampler)
            : _deviceContext(deviceContext),
              _sampler(sampler) {
        //
    }

    VkSampler::~VkSampler() {
        this->_deviceContext->getLogicalDevice().destroy(this->_sampler);
    }
}
