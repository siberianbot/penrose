#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_SAMPLER_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_SAMPLER_FACTORY_HPP

#include <Penrose/Rendering/SamplerFactory.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/Rendering/DeviceContext.hpp"

namespace Penrose {

    class ResourceSet;

    class VkSamplerFactory : public Resource, public SamplerFactory {
    public:
        explicit VkSamplerFactory(ResourceSet *resources);
        ~VkSamplerFactory() override = default;

        [[nodiscard]] Sampler *makeSampler(const SamplerInfo &samplerInfo) override;

    private:
        Lazy<DeviceContext> _deviceContext;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_SAMPLER_FACTORY_HPP
