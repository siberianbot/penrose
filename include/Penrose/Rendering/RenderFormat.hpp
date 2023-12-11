#ifndef PENROSE_RENDERING_RENDER_FORMAT_HPP
#define PENROSE_RENDERING_RENDER_FORMAT_HPP

namespace Penrose {

    /**
     * \brief Available rendering formats
     */
    enum class RenderFormat {
        None, // TODO: remove
        R8UNorm,
        RGBA8UNorm,
        RGBA16Float,
        D32Float
    };
}

#endif // PENROSE_RENDERING_RENDER_FORMAT_HPP
