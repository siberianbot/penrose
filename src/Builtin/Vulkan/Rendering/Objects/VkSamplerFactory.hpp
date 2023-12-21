#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_SAMPLER_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_SAMPLER_FACTORY_HPP

#include <Penrose/Rendering/Objects/SamplerFactory.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Rendering/Objects/VkLogicalDeviceProvider.hpp"

namespace Penrose {

    class VkSamplerFactory final: public Resource<VkSamplerFactory>,
                                  public SamplerFactory {
    public:
        explicit VkSamplerFactory(const ResourceSet *resources);
        ~VkSamplerFactory() override = default;

        [[nodiscard]] Sampler *makeSampler(
            SamplerAddressMode addressMode, SamplerFilteringMode minFilter, SamplerFilteringMode magFilter,
            SamplerBorderColor borderColor
        ) override;

    private:
        ResourceProxy<VkLogicalDeviceProvider> _logicalDeviceProvider;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_SAMPLER_FACTORY_HPP
