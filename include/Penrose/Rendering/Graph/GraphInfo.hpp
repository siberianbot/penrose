#ifndef PENROSE_RENDERING_GRAPH_GRAPH_INFO_HPP
#define PENROSE_RENDERING_GRAPH_GRAPH_INFO_HPP

#include <optional>
#include <string>
#include <vector>

#include <Penrose/Rendering/Graph/AttachmentInfo.hpp>
#include <Penrose/Rendering/Graph/PassInfo.hpp>
#include <Penrose/Types/Rect.hpp>

namespace Penrose {

    /**
     * \brief Render graph info
     * \details Render graph describes set of attached targets and passes. Optional render area can be specified.
     * Otherwise, surface area is used.
     */
    struct PENROSE_API GraphInfo {

        /**
         * \brief Render graph name
         */
        std::string name;

        /**
         * \brief Attached targets
         */
        std::vector<AttachmentInfo> attachments;

        /**
         * \brief Graph passes
         */
        std::vector<PassInfo> passes;

        /**
         * \brief Render area (or surface area is used)
         */
        std::optional<IntRect> area;

        friend auto operator<=>(const GraphInfo &, const GraphInfo &) = default;
    };
}

#endif // PENROSE_RENDERING_GRAPH_GRAPH_INFO_HPP
