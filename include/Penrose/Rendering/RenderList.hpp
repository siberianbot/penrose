#ifndef PENROSE_RENDERING_RENDER_LIST_HPP
#define PENROSE_RENDERING_RENDER_LIST_HPP

#include <list>

#include <Penrose/Rendering/Drawable.hpp>
#include <Penrose/Rendering/View.hpp>

namespace Penrose {

    struct RenderList {
        View view;
        std::list<Drawable> drawables;
    };
}

#endif // PENROSE_RENDERING_RENDER_LIST_HPP
