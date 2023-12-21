#ifndef PENROSE_RENDERING_GRAPH_ATTACHMENT_INFO_HPP
#define PENROSE_RENDERING_GRAPH_ATTACHMENT_INFO_HPP

#include <array>
#include <cstdint>

#include <Penrose/Rendering/RenderFormat.hpp>

namespace Penrose {

    /**
     * \brief Render attachment layout
     */
    enum class AttachmentLayout {
        Undefined,
        ColorAttachment,
        DepthStencilAttachment,
        Present
    };

    /**
     * \brief Render attachment load operator
     */
    enum class AttachmentLoadOp {
        DontCare,
        Load,
        Clear
    };

    /**
     * \brief Render attachment store operator
     */
    enum class AttachmentStoreOp {
        DontCare,
        Store
    };

    /**
     * \brief Render attachment clear value
     */
    struct PENROSE_API AttachmentClearValueInfo {

        /**
         * \brief Color clear value
         */
        std::array<float, 4> color;

        /**
         * \brief Depth clear value
         */
        float depth;

        /**
         * \brief Stencil clear value
         */
        std::uint32_t stencil;

        friend auto operator<=>(const AttachmentClearValueInfo &, const AttachmentClearValueInfo &) = default;
    };

    /**
     * \brief Attachment info for render graph
     * \details Attachments are used in render graph to define which targets should be treated as inputs or outputs.
     */
    struct PENROSE_API AttachmentInfo {

        /**
         * \brief Rendering format of attachment (or swapchain format when there is none)
         */
        std::optional<RenderFormat> format = std::nullopt;

        /**
         * \brief Clear value
         */
        AttachmentClearValueInfo clearValue;

        /**
         * \brief Load operator
         */
        AttachmentLoadOp loadOp;

        /**
         * \brief Store operator
         */
        AttachmentStoreOp storeOp;

        /**
         * \brief Initial layout
         */
        AttachmentLayout initialLayout;

        /**
         * \brief Final layout
         */
        AttachmentLayout finalLayout;

        friend auto operator<=>(const AttachmentInfo &, const AttachmentInfo &) = default;
    };
}

#endif // PENROSE_RENDERING_GRAPH_ATTACHMENT_INFO_HPP
