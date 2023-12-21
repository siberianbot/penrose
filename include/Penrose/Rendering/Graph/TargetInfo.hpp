#ifndef PENROSE_RENDERING_GRAPH_TARGET_INFO_HPP
#define PENROSE_RENDERING_GRAPH_TARGET_INFO_HPP

#include <cstdint>
#include <optional>
#include <string>

#include <Penrose/Rendering/RenderFormat.hpp>
#include <Penrose/Types/Size.hpp>

namespace Penrose {

    /**
     * \brief Render target types
     */
    enum class TargetType {
        Image,
        Swapchain
    };

    /**
     * \brief Render target usage flags
     */
    using TargetUsageFlags = std::uint8_t;

    /**
     * \brief Render target usages
     */
    enum class TargetUsage : TargetUsageFlags {
        None = 0,
        Input = 1 << 0,
        Color = 1 << 1,
        DepthStencil = 1 << 2
    };

    [[nodiscard]] constexpr TargetUsageFlags operator&(const TargetUsage &lhs, const TargetUsage &rhs) {
        return static_cast<TargetUsageFlags>(lhs) & static_cast<TargetUsageFlags>(rhs);
    }

    [[nodiscard]] constexpr TargetUsageFlags operator&(const TargetUsageFlags &lhs, const TargetUsage &rhs) {
        return lhs & static_cast<TargetUsageFlags>(rhs);
    }

    [[nodiscard]] constexpr TargetUsageFlags operator|(const TargetUsage &lhs, const TargetUsage &rhs) {
        return static_cast<TargetUsageFlags>(lhs) | static_cast<TargetUsageFlags>(rhs);
    }

    [[nodiscard]] constexpr TargetUsageFlags operator|(const TargetUsageFlags &lhs, const TargetUsage &rhs) {
        return lhs | static_cast<TargetUsageFlags>(rhs);
    }

    /**
     * \brief Target info for render graph
     * \details Targets are used in render graph as rendering inputs or outputs (i.e. g-buffer, swapchain, etc).
     */
    struct PENROSE_API TargetInfo {

        /**
         * \brief Target type
         */
        TargetType type = TargetType::Swapchain;

        /**
         * \brief Name of target, ignored for swapchain
         */
        std::string name;

        /**
         * \brief Target usage (one or many)
         */
        TargetUsageFlags usage = static_cast<TargetUsageFlags>(TargetUsage::None);

        /**
         * \brief Rendering format of target (or swapchain format when there is none)
         */
        std::optional<RenderFormat> format = std::nullopt;

        /**
         * \brief Size of target (or surface size when there is none)
         */
        std::optional<Size> size = std::nullopt;

        friend auto operator<=>(const TargetInfo &, const TargetInfo &) = default;
    };
}

#endif // PENROSE_RENDERING_GRAPH_TARGET_INFO_HPP
