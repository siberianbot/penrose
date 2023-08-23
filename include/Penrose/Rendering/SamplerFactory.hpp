#ifndef PENROSE_RENDERING_SAMPLER_FACTORY_HPP
#define PENROSE_RENDERING_SAMPLER_FACTORY_HPP

#include <Penrose/Rendering/SamplerInfo.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class Sampler;

    class SamplerFactory : public Resource {
    public:
        ~SamplerFactory() override = default;

        [[nodiscard]] virtual Sampler *makeSampler(const SamplerInfo &samplerInfo) = 0;
    };
}

#endif // PENROSE_RENDERING_SAMPLER_FACTORY_HPP
