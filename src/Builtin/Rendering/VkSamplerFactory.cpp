#include "VkSamplerFactory.hpp"

#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Rendering/DeviceContext.hpp"

#include "src/Builtin/Rendering/VkSampler.hpp"
#include "src/Builtin/Rendering/VkUtils.hpp"

namespace Penrose {

    VkSamplerFactory::VkSamplerFactory(ResourceSet *resources)
            : _deviceContext(resources->get<DeviceContext>()) {
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

        auto sampler = this->_deviceContext->getLogicalDevice().createSampler(createInfo);

        return new VkSampler(this->_deviceContext, sampler);
    }
}
