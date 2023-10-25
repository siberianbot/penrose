#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_SAMPLER_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_SAMPLER_FACTORY_HPP

#include <Penrose/Rendering/SamplerFactory.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"

namespace Penrose {

    class VkSamplerFactory : public Resource<VkSamplerFactory, ResourceGroup::Rendering>,
                             public SamplerFactory {
    public:
        explicit VkSamplerFactory(ResourceSet *resources);
        ~VkSamplerFactory() override = default;

        [[nodiscard]] Sampler *makeSampler(const SamplerInfo &samplerInfo) override;

    private:
        ResourceProxy<VkLogicalDeviceContext> _logicalDeviceContext;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_SAMPLER_FACTORY_HPP
