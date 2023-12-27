#ifndef PENROSE_RENDERING_VIEW_HPP
#define PENROSE_RENDERING_VIEW_HPP

#include <variant>

#include <glm/mat4x4.hpp>

#include <Penrose/Types/Projection.hpp>

namespace Penrose {

    struct View {
        std::variant<PerspectiveProjection, OrthographicProjection> projection;
        glm::mat4 view;
    };
}

#endif // PENROSE_RENDERING_VIEW_HPP
