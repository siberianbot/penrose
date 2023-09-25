#ifndef PENROSE_RENDERING_VIEW_HPP
#define PENROSE_RENDERING_VIEW_HPP

#include <variant>

#include <glm/mat4x4.hpp>

namespace Penrose {

    struct PerspectiveProjection {
        float fov;
        float near;
        float far;
    };

    struct OrthographicProjection {
        float top;
        float bottom;
        float left;
        float right;
        float near;
        float far;
    };

    struct View {
        std::variant<PerspectiveProjection, OrthographicProjection> projection;
        glm::mat4 view;
    };
}

#endif // PENROSE_RENDERING_VIEW_HPP
