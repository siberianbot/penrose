#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_SAMPLER_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_SAMPLER_HPP

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/Sampler.hpp>

namespace Penrose {

    class VkLogicalDeviceContext;

    class VkSampler : public Sampler {
    public:
        VkSampler(VkLogicalDeviceContext *logicalDeviceContext, vk::Sampler sampler);
        ~VkSampler() override;

        [[nodiscard]] const vk::Sampler &getSampler() const { return this->_sampler; }

    private:
        VkLogicalDeviceContext *_logicalDeviceContext;

        vk::Sampler _sampler;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_SAMPLER_HPP
