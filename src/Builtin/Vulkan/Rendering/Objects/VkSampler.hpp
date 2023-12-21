#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_SAMPLER_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_SAMPLER_HPP

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/Objects/Sampler.hpp>

namespace Penrose {

    class VkSampler final: public Sampler {
    public:
        VkSampler(
            const SamplerAddressMode addressMode, const SamplerFilteringMode minFilter,
            const SamplerFilteringMode magFilter, const SamplerBorderColor borderColor, vk::UniqueSampler &&sampler
        )
            : _addressMode(addressMode),
              _minFilter(minFilter),
              _magFilter(magFilter),
              _borderColor(borderColor),
              _sampler(std::forward<decltype(sampler)>(sampler)) {
            //
        }

        ~VkSampler() override = default;

        [[nodiscard]] SamplerAddressMode getAddressMode() const override { return this->_addressMode; }

        [[nodiscard]] SamplerFilteringMode getMinFilter() const override { return this->_minFilter; }

        [[nodiscard]] SamplerFilteringMode getMagFilter() const override { return this->_magFilter; }

        [[nodiscard]] SamplerBorderColor getBorderColor() const override { return this->_borderColor; }

        [[nodiscard]] const vk::Sampler &getHandle() const { return this->_sampler.get(); }

    private:
        SamplerAddressMode _addressMode;
        SamplerFilteringMode _minFilter;
        SamplerFilteringMode _magFilter;
        SamplerBorderColor _borderColor;

        vk::UniqueSampler _sampler;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_SAMPLER_HPP
