#ifndef PENROSE_RENDERING_RENDER_LIST_HPP
#define PENROSE_RENDERING_RENDER_LIST_HPP

#include <map>
#include <set>
#include <variant>

#include <glm/mat4x4.hpp>

#include "src/Assets/AssetId.hpp"
#include "src/ECS/ECSBase.hpp"

namespace Penrose {

    struct Perspective {
        float fov;
    };

    struct Orthogonal {
        // TODO
    };

    struct RenderListItem {
        AssetId mesh;
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
