#ifndef PENROSE_RENDERING_RENDER_LIST_HPP
#define PENROSE_RENDERING_RENDER_LIST_HPP

#include <map>
#include <set>
#include <string>
#include <variant>

#include <glm/mat4x4.hpp>

#include <Penrose/ECS/Entity.hpp>

namespace Penrose {

    struct Perspective {
        float fov;
    };

    struct Orthogonal {
        // TODO
    };

    struct RenderListItem {
        std::string mesh;
        std::string albedo;
        glm::mat4 model;
        glm::mat4 modelRot;
    };

    struct RenderList {
        float near;
        float far;
        std::optional<std::variant<Perspective, Orthogonal>> projection;
        glm::mat4 view;
        std::set<Entity> entities;
        std::map<Entity, RenderListItem> items;
    };
}

#endif // PENROSE_RENDERING_RENDER_LIST_HPP
