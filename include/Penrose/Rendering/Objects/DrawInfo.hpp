#ifndef PENROSE_RENDERING_OBJECTS_DRAW_INFO_HPP
#define PENROSE_RENDERING_OBJECTS_DRAW_INFO_HPP

#include <optional>
#include <vector>

#include <Penrose/Rendering/Objects/Buffer.hpp>

namespace Penrose {

    /**
     * \brief Buffer usage definition
     */
    struct PENROSE_API BufferUsage {

        /**
         * \brief Pointer to buffer instance
         */
        Buffer *buffer;

        /**
         * \brief Buffer offset
         */
        std::uint64_t offset;

        friend auto operator<=>(const BufferUsage &, const BufferUsage &) = default;
    };

    /**
     * \brief Drawing operation definition
     */
    struct PENROSE_API DrawInfo {

        /**
         * \brief Count of vertices or indices to draw
         */
        std::uint32_t count;

        /**
         * \brief Count of instances to draw
         */
        std::uint32_t instanceCount;

        /**
         * \brief Input buffers
         */
        std::vector<BufferUsage> buffers;

        /**
         * \brief Optional index data buffer
         */
        std::optional<BufferUsage> index;

        friend auto operator<=>(const DrawInfo &, const DrawInfo &) = default;
    };
}

#endif // PENROSE_RENDERING_OBJECTS_DRAW_INFO_HPP
