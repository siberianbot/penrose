#ifndef PENROSE_BUILTIN_RENDERING_VK_SAMPLER_FACTORY_HPP
#define PENROSE_BUILTIN_RENDERING_VK_SAMPLER_FACTORY_HPP

#include <Penrose/Rendering/SamplerFactory.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ResourceSet;
    class DeviceContext;

    class VkSamplerFactory : public Resource, public SamplerFactory {
    public:
        explicit VkSamplerFactory(ResourceSet *resources);
        ~VkSamplerFactory() override = default;

        [[nodiscard]] Sampler *makeSampler(const SamplerInfo &samplerInfo) override;

    private:
        DeviceContext *_deviceContext;
    };
}

#endif // PENROSE_BUILTIN_RENDERING_VK_SAMPLER_FACTORY_HPP
