#ifndef PENROSE_RENDERING_SAMPLER_INFO_HPP
#define PENROSE_RENDERING_SAMPLER_INFO_HPP

namespace Penrose {

    enum class SamplerAddressMode {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder
    };

    enum class SamplerFilter {
        Linear,
        Nearest
    };

    enum class SamplerBorderColor {
        Transparent,
        Black,
        White
    };

    class SamplerInfo {
    public:
        explicit constexpr SamplerInfo(SamplerAddressMode addressMode = SamplerAddressMode::Repeat,
                                       SamplerFilter minFilter = SamplerFilter::Linear,
                                       SamplerFilter magFilter = SamplerFilter::Linear,
                                       SamplerBorderColor borderColor = SamplerBorderColor::Transparent)
                : _addressMode(addressMode),
                  _minFilter(minFilter),
                  _magFilter(magFilter),
                  _borderColor(borderColor) {
            //
        }

        [[nodiscard]] constexpr SamplerInfo &setAddressMode(SamplerAddressMode addressMode) {
            this->_addressMode = addressMode;

            return *this;
        }

        [[nodiscard]] constexpr SamplerInfo &setMinFilter(SamplerFilter minFilter) {
            this->_minFilter = minFilter;

            return *this;
        }

        [[nodiscard]] constexpr SamplerInfo &setMagFilter(SamplerFilter magFilter) {
            this->_magFilter = magFilter;

            return *this;
        }

        [[nodiscard]] constexpr SamplerInfo &setBorderColor(SamplerBorderColor borderColor) {
            this->_borderColor = borderColor;

            return *this;
        }

        [[nodiscard]] constexpr const SamplerAddressMode &getAddressMode() const { return this->_addressMode; }

        [[nodiscard]] constexpr const SamplerFilter &getMinFilter() const { return this->_minFilter; }

        [[nodiscard]] constexpr const SamplerFilter &getMagFilter() const { return this->_magFilter; }

        [[nodiscard]] constexpr const SamplerBorderColor &getBorderColor() const { return this->_borderColor; }

    private:
        SamplerAddressMode _addressMode;
        SamplerFilter _minFilter;
        SamplerFilter _magFilter;
        SamplerBorderColor _borderColor;
    };
}

#endif // PENROSE_RENDERING_SAMPLER_INFO_HPP
