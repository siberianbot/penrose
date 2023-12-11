#ifndef PENROSE_RENDERING_RENDER_TARGET_INFO_HPP
#define PENROSE_RENDERING_RENDER_TARGET_INFO_HPP

#include <cstdint>
#include <optional>
#include <utility>

#include <Penrose/Types/Size.hpp>
#include <Penrose/Rendering/RenderFormat.hpp>

namespace Penrose {

    enum class RenderTargetSource {
        None,
        Image,
        Swapchain
    };

    enum class RenderTargetType : std::uint8_t {
        None = 0,
        Input = 1 << 0,
        Color = 1 << 1,
        DepthStencil = 1 << 2
    };

    [[nodiscard]] constexpr std::uint8_t operator&(const RenderTargetType &lhs, const RenderTargetType &rhs) {
        return static_cast<std::uint8_t>(lhs) & static_cast<std::uint8_t>(rhs);
    }

    class RenderTargetInfo {
    public:
        explicit constexpr RenderTargetInfo(RenderTargetSource source = RenderTargetSource::None,
                                            RenderTargetType type = RenderTargetType::None,
                                            RenderFormat format = RenderFormat::None,
                                            std::optional<Size> &&size = std::nullopt)
                : _source(source),
                  _type(type),
                  _format(format),
                  _size(size) {
            //
        }

        [[nodiscard]] constexpr RenderTargetInfo &setSource(RenderTargetSource source) {
            this->_source = source;

            return *this;
        }

        [[nodiscard]] constexpr RenderTargetInfo &setType(RenderTargetType type) {
            this->_type = type;

            return *this;
        }

        [[nodiscard]] constexpr RenderTargetInfo &setFormat(RenderFormat format) {
            this->_format = format;

            return *this;
        }

        [[nodiscard]] constexpr RenderTargetInfo &setSize(std::optional<Size> &&size) {
            this->_size = size;

            return *this;
        }

        [[nodiscard]] constexpr const RenderTargetSource &getSource() const { return this->_source; }

        [[nodiscard]] constexpr const RenderTargetType &getType() const { return this->_type; }

        [[nodiscard]] constexpr const RenderFormat &getFormat() const { return this->_format; }

        [[nodiscard]] constexpr const std::optional<Size> &getSize() const { return this->_size; }

        [[nodiscard]] constexpr bool operator==(const RenderTargetInfo &rhs) const = default;

    private:
        RenderTargetSource _source;
        RenderTargetType _type;
        RenderFormat _format;
        std::optional<Size> _size;
    };
}

#endif // PENROSE_RENDERING_RENDER_TARGET_INFO_HPP
