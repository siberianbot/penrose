#ifndef PENROSE_BUILTIN_RENDERING_VK_SAMPLER_HPP
#define PENROSE_BUILTIN_RENDERING_VK_SAMPLER_HPP

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/Sampler.hpp>

namespace Penrose {

    class DeviceContext;

    class VkSampler : public Sampler {
    public:
        VkSampler(DeviceContext *deviceContext, vk::Sampler sampler);
        ~VkSampler() override;

        [[nodiscard]] const vk::Sampler &getSampler() const { return this->_sampler; }

    private:
        DeviceContext *_deviceContext;

        vk::Sampler _sampler;
    };
}

#endif // PENROSE_BUILTIN_RENDERING_VK_SAMPLER_HPP
