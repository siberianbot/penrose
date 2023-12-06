#include "VkSamplerFactory.hpp"

#include "src/Builtin/Vulkan/Rendering/VkSampler.hpp"
#include "src/Builtin/Vulkan/Rendering/VkUtils.hpp"

namespace Penrose {

    VkSamplerFactory::VkSamplerFactory(const ResourceSet *resources)
            : _logicalDeviceContext(resources->get<VkLogicalDeviceContext>()) {
        //
    }

    Sampler *VkSamplerFactory::makeSampler(const SamplerInfo &samplerInfo) {
        auto addressMode = toVkSamplerAddressMode(samplerInfo.getAddressMode());

        auto createInfo = vk::SamplerCreateInfo()
                .setAddressModeU(addressMode)
                .setAddressModeV(addressMode)
                .setAddressModeW(addressMode)
                .setBorderColor(toVkBorderColor(samplerInfo.getBorderColor()))
                .setMinFilter(toVkFilter(samplerInfo.getMinFilter()))
                .setMagFilter(toVkFilter(samplerInfo.getMagFilter()))
                .setAnisotropyEnable(false)
                .setMaxAnisotropy(1)
                .setCompareEnable(false)
                .setCompareOp(vk::CompareOp::eNever)
                .setMinLod(0)
                .setMaxLod(1)
                .setUnnormalizedCoordinates(false)
                .setMipmapMode(vk::SamplerMipmapMode::eLinear);

        auto sampler = this->_logicalDeviceContext->getHandle().createSampler(createInfo);

        return new VkSampler(this->_logicalDeviceContext.get(), sampler);
    }
}
