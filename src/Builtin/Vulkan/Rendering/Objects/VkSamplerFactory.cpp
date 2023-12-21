#include "src/Builtin/Vulkan/Rendering/Objects/VkSamplerFactory.hpp"

#include "src/Builtin/Vulkan/Rendering/Objects/VkSampler.hpp"
#include "src/Builtin/Vulkan/Rendering/VkUtils.hpp"

namespace Penrose {

    VkSamplerFactory::VkSamplerFactory(const ResourceSet *resources)
        : _logicalDeviceProvider(resources->get<VkLogicalDeviceProvider>()) {
        //
    }

    Sampler *VkSamplerFactory::makeSampler(
        const SamplerAddressMode addressMode, const SamplerFilteringMode minFilter,
        const SamplerFilteringMode magFilter, const SamplerBorderColor borderColor
    ) {
        const auto vkAddressMode = mapAddressMode(addressMode);
        const auto createInfo = vk::SamplerCreateInfo()
                                    .setAddressModeU(vkAddressMode)
                                    .setAddressModeV(vkAddressMode)
                                    .setAddressModeW(vkAddressMode)
                                    .setMinFilter(mapFilteringMode(minFilter))
                                    .setMagFilter(mapFilteringMode(magFilter))
                                    .setBorderColor(mapBorderColor(borderColor))
                                    .setAnisotropyEnable(false)
                                    .setMaxAnisotropy(1)
                                    .setCompareEnable(false)
                                    .setCompareOp(vk::CompareOp::eNever)
                                    .setMinLod(0)
                                    .setMaxLod(1)
                                    .setUnnormalizedCoordinates(false)
                                    .setMipmapMode(vk::SamplerMipmapMode::eLinear);

        auto sampler = this->_logicalDeviceProvider->getLogicalDevice().handle->createSamplerUnique(createInfo);

        return new VkSampler(addressMode, minFilter, magFilter, borderColor, std::move(sampler));
    }
}
