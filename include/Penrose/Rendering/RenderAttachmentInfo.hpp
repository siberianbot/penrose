#ifndef PENROSE_RENDERING_RENDER_ATTACHMENT_INFO_HPP
#define PENROSE_RENDERING_RENDER_ATTACHMENT_INFO_HPP

#include <array>
#include <cstdint>
#include <string>

#include <Penrose/Rendering/RenderFormat.hpp>

namespace Penrose {

    enum class RenderAttachmentLayout {
        Undefined,
        ColorAttachment,
        DepthStencilAttachment,
        Present
    };

    enum class RenderAttachmentLoadOp {
        DontCare,
        Load,
        Clear
    };

    enum class RenderAttachmentStoreOp {
        DontCare,
        Store
    };

    class RenderAttachmentClearValueInfo {
    public:
        explicit constexpr RenderAttachmentClearValueInfo(std::array<float, 4> &&color = {},
                                                          float depth = 0,
                                                          std::uint32_t stencil = 0)
                : _color(color),
                  _depth(depth),
                  _stencil(stencil) {
            //
        }

        [[nodiscard]] constexpr RenderAttachmentClearValueInfo &setColor(const std::array<float, 4> &color) {
            this->_color = color;

            return *this;
        }

        [[nodiscard]] constexpr RenderAttachmentClearValueInfo &setColor(std::array<float, 4> &&color) {
            this->_color = color;

            return *this;
        }

        [[nodiscard]] constexpr RenderAttachmentClearValueInfo &setDepth(float depth) {
            this->_depth = depth;

            return *this;
        }

        [[nodiscard]] constexpr RenderAttachmentClearValueInfo &setStencil(std::uint32_t stencil) {
            this->_stencil = stencil;

            return *this;
        }

        [[nodiscard]] constexpr const std::array<float, 4> &getColor() const { return this->_color; }

        [[nodiscard]] constexpr const float &getDepth() const { return this->_depth; }

        [[nodiscard]] constexpr const std::uint32_t &getStencil() const { return this->_stencil; }

        [[nodiscard]] constexpr bool operator==(const RenderAttachmentClearValueInfo &rhs) const = default;

    private:
        std::array<float, 4> _color;
        float _depth;
        std::uint32_t _stencil;
    };

    class RenderAttachmentInfo {
    public:
        explicit constexpr RenderAttachmentInfo(
                std::string &&target,
                RenderFormat format = RenderFormat::None,
                RenderAttachmentClearValueInfo &&clearValue = RenderAttachmentClearValueInfo{},
                RenderAttachmentLoadOp loadOp = RenderAttachmentLoadOp::DontCare,
                RenderAttachmentStoreOp storeOp = RenderAttachmentStoreOp::DontCare,
                RenderAttachmentLayout initialLayout = RenderAttachmentLayout::Undefined,
                RenderAttachmentLayout finalLayout = RenderAttachmentLayout::Undefined)
                : _target(target),
                  _format(format),
                  _clearValue(clearValue),
                  _loadOp(loadOp),
                  _storeOp(storeOp),
                  _initialLayout(initialLayout),
                  _finalLayout(finalLayout) {
            //
        }

        [[nodiscard]] constexpr RenderAttachmentInfo &setTarget(const std::string &target) {
            this->_target = target;

            return *this;
        }

        [[nodiscard]] constexpr RenderAttachmentInfo &setTarget(std::string &&target) {
            this->_target = target;

            return *this;
        }

        [[nodiscard]] constexpr RenderAttachmentInfo &setFormat(RenderFormat format) {
            this->_format = format;

            return *this;
        }

        [[nodiscard]] constexpr RenderAttachmentInfo &setClearValue(const RenderAttachmentClearValueInfo &clearValue) {
            this->_clearValue = clearValue;

            return *this;
        }

        [[nodiscard]] constexpr RenderAttachmentInfo &setClearValue(RenderAttachmentClearValueInfo &&clearValue) {
            this->_clearValue = clearValue;

            return *this;
        }

        [[nodiscard]] constexpr RenderAttachmentInfo &setLoadOp(RenderAttachmentLoadOp loadOp) {
            this->_loadOp = loadOp;

            return *this;
        }

        [[nodiscard]] constexpr RenderAttachmentInfo &setStoreOp(RenderAttachmentStoreOp storeOp) {
            this->_storeOp = storeOp;

            return *this;
        }

        [[nodiscard]] constexpr RenderAttachmentInfo &setInitialLayout(RenderAttachmentLayout initialLayout) {
            this->_initialLayout = initialLayout;

            return *this;
        }

        [[nodiscard]] constexpr RenderAttachmentInfo &setFinalLayout(RenderAttachmentLayout finalLayout) {
            this->_finalLayout = finalLayout;

            return *this;
        }

        [[nodiscard]] constexpr const std::string &getTarget() const { return this->_target; }

        [[nodiscard]] constexpr const RenderFormat &getFormat() const { return this->_format; }

        [[nodiscard]] constexpr const RenderAttachmentClearValueInfo &getClearValue() const {
            return this->_clearValue;
        }

        [[nodiscard]] constexpr const RenderAttachmentLoadOp &getLoadOp() const { return this->_loadOp; }

        [[nodiscard]] constexpr const RenderAttachmentStoreOp &getStoreOp() const { return this->_storeOp; }

        [[nodiscard]] constexpr const RenderAttachmentLayout &getInitialLayout() const { return this->_initialLayout; }

        [[nodiscard]] constexpr const RenderAttachmentLayout &getFinalLayout() const { return this->_finalLayout; }

        [[nodiscard]] bool operator==(const RenderAttachmentInfo &rhs) const = default;

    private:
        std::string _target;
        RenderFormat _format;
        RenderAttachmentClearValueInfo _clearValue;
        RenderAttachmentLoadOp _loadOp;
        RenderAttachmentStoreOp _storeOp;
        RenderAttachmentLayout _initialLayout;
        RenderAttachmentLayout _finalLayout;
    };
}

#endif // PENROSE_RENDERING_RENDER_ATTACHMENT_INFO_HPP
