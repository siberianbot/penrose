#ifndef PENROSE_COMMON_VERTEX_HPP
#define PENROSE_COMMON_VERTEX_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace Penrose {

    struct Vertex {
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec3 color;
        glm::vec2 uv;
    };
}

#endif // PENROSE_COMMON_VERTEX_HPP
