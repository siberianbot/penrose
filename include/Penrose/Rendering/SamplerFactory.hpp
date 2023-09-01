#ifndef PENROSE_RENDERING_SAMPLER_FACTORY_HPP
#define PENROSE_RENDERING_SAMPLER_FACTORY_HPP

#include <Penrose/Rendering/SamplerInfo.hpp>

namespace Penrose {

    class Sampler;

    class SamplerFactory {
    public:
        virtual ~SamplerFactory() = default;

        [[nodiscard]] virtual Sampler *makeSampler(const SamplerInfo &samplerInfo) = 0;
    };
}

#endif // PENROSE_RENDERING_SAMPLER_FACTORY_HPP
