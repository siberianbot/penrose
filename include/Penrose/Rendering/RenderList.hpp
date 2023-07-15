#ifndef PENROSE_RENDERING_RENDER_LIST_HPP
#define PENROSE_RENDERING_RENDER_LIST_HPP

#include <map>
#include <optional>
#include <set>
#include <string>
#include <variant>

#include <glm/mat4x4.hpp>

#include <Penrose/ECS/Entity.hpp>

namespace Penrose {

    struct Perspective {
        float fov;
        float near;
        float far;
    };

    struct Orthogonal {
        // TODO
    };

    struct View {
        std::optional<std::variant<Perspective, Orthogonal>> projection;
        glm::mat4 view;
    };

    class ViewVisitor {
    public:
        virtual void visit(View *) const = 0;
    };

    struct Drawable {
        std::optional<std::string> meshAsset;
        std::optional<std::string> albedoTextureAsset;
        glm::mat4 model;
        glm::mat4 modelRot;
    };

    class DrawableVisitor {
    public:
        virtual void visit(Drawable *) const = 0;
    };

    struct RenderList {
        View view;
        std::map<Entity, Drawable> drawables;
    };
}

#endif // PENROSE_RENDERING_RENDER_LIST_HPP
