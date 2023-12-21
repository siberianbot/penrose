#ifndef PENROSE_RENDERING_OBJECTS_SAMPLER_HPP
#define PENROSE_RENDERING_OBJECTS_SAMPLER_HPP

namespace Penrose {

    /**
     * \brief Sampler address mode
     */
    enum class SamplerAddressMode {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder
    };

    /**
     * \brief Sampler minification/magnification filtering mode
     */
    enum class SamplerFilteringMode {
        Linear,
        Nearest
    };

    /**
     * \brief Sampler border color
     */
    enum class SamplerBorderColor {
        Transparent,
        Black,
        White
    };

    /**
     * \brief Sampler object
     * \details Sampler objects used for representing image in shader pipeline.
     */
    class PENROSE_API Sampler {
    public:
        virtual ~Sampler() = default;

        /**
         * \brief Get address mode of sampler
         * \return Address mode of sampler
         */
        [[nodiscard]] virtual SamplerAddressMode getAddressMode() const = 0;

        /**
         * \brief Get minification filter of sampler
         * \return Minification filter of sampler
         */
        [[nodiscard]] virtual SamplerFilteringMode getMinFilter() const = 0;

        /**
         * \brief Get magnification filter of sampler
         * \return Magnification filter of sampler
         */
        [[nodiscard]] virtual SamplerFilteringMode getMagFilter() const = 0;

        /**
         * \brief Get border color of sampler
         * \return Border color of sampler
         */
        [[nodiscard]] virtual SamplerBorderColor getBorderColor() const = 0;
    };
}

#endif // PENROSE_RENDERING_OBJECTS_SAMPLER_HPP
