#ifndef PENROSE_RENDERING_OBJECTS_SAMPLER_FACTORY_HPP
#define PENROSE_RENDERING_OBJECTS_SAMPLER_FACTORY_HPP

#include <Penrose/Rendering/Objects/Sampler.hpp>

namespace Penrose {

    /**
     * \brief Sampler object factory interface
     */
    class PENROSE_API SamplerFactory {
    public:
        virtual ~SamplerFactory() = default;

        /**
         * \brief Create instance of sampler object
         * \param addressMode Address mode
         * \param minFilter Minification filtering mode
         * \param magFilter Magnification filtering mode
         * \param borderColor Border color
         * \return Instance of sampler object
         */
        [[nodiscard]] virtual Sampler *makeSampler(
            SamplerAddressMode addressMode, SamplerFilteringMode minFilter, SamplerFilteringMode magFilter,
            SamplerBorderColor borderColor
        ) = 0;
    };
}

#endif // PENROSE_RENDERING_OBJECTS_SAMPLER_FACTORY_HPP
