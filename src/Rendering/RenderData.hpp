#ifndef PENROSE_RENDERING_RENDER_DATA_HPP
#define PENROSE_RENDERING_RENDER_DATA_HPP

#include <glm/mat4x4.hpp>

namespace Penrose {

    struct RenderData {
        alignas(16) glm::mat4 matrix;
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 modelRot;
    };
}

#endif // PENROSE_RENDERING_RENDER_DATA_HPP
