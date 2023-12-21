#ifndef PENROSE_RENDERING_GRAPH_PASS_INFO_HPP
#define PENROSE_RENDERING_GRAPH_PASS_INFO_HPP

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace Penrose {

    /**
     * \brief Pass info for render graph
     * \details Render graph is consisted of passes which defines which targets are used as inputs or outputs. Pass
     * can be dependent on different passes. Pass can specify optional function name which invokes required renderer
     * logic.
     */
    struct PENROSE_API PassInfo {

        /**
         * \brief Pass dependencies
         */
        std::vector<std::uint32_t> dependsOn;

        /**
         * \brief Indexes of input targets
         */
        std::vector<std::uint32_t> inputTargets;

        /**
         * \brief Indexes of color targets
         */
        std::vector<std::uint32_t> colorTargets;

        /**
         * \brief Index of depth/stencil target
         */
        std::optional<std::uint32_t> depthStencilTarget;

        /**
         * \brief Name of renderer function
         */
        std::optional<std::string> function;

        friend auto operator<=>(const PassInfo &, const PassInfo &) = default;
    };
}

#endif // PENROSE_RENDERING_GRAPH_PASS_INFO_HPP
