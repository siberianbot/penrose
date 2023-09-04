#include "VkSampler.hpp"

#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"

namespace Penrose {

    VkSampler::VkSampler(VkLogicalDeviceContext *logicalDeviceContext, vk::Sampler sampler)
            : _logicalDeviceContext(logicalDeviceContext),
              _sampler(sampler) {
        //
    }

    VkSampler::~VkSampler() {
        this->_logicalDeviceContext->getHandle().destroy(this->_sampler);
    }
}
