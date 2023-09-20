#ifndef PENROSE_BUILTIN_ECS_TRANSFORM_COMPONENT_HPP
#define PENROSE_BUILTIN_ECS_TRANSFORM_COMPONENT_HPP

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ComponentFactory.hpp>
#include <Penrose/Rendering/RenderList.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class TransformComponent : public Component, public DrawableVisitor, public ViewVisitor {
    public:
        ~TransformComponent() override = default;

        [[nodiscard]] glm::vec3 &getPos() { return this->_pos; }

        [[nodiscard]] glm::vec3 &getRot() { return this->_rot; }

        [[nodiscard]] glm::vec3 &getScale() { return this->_scale; }

        void visit(Drawable *item) const override {
            auto pos = glm::translate(glm::mat4(1), this->_pos);
            auto rot = glm::mat4_cast(glm::quat(this->_rot));
            auto scale = glm::scale(glm::mat4(1), this->_scale);

            item->model = pos * rot * scale;
            item->modelRot = rot;
        }

        void visit(View *view) const override {
            auto target = glm::mat3_cast(glm::quat(this->_rot)) * glm::vec3(1, 0, 0);

            view->view = glm::lookAt(this->_pos, this->_pos + target, glm::vec3(0, 1, 0));
        }

        [[nodiscard]] constexpr static std::string name() { return "Transform"; }

    private:
        glm::vec3 _pos = glm::vec3(0);
        glm::vec3 _rot = glm::vec3(0);
        glm::vec3 _scale = glm::vec3(1);
    };

    class TransformComponentFactory : public Resource, public GenericComponentFactory<TransformComponent> {
    public:
        ~TransformComponentFactory() override = default;
    };
}

#endif // PENROSE_BUILTIN_ECS_TRANSFORM_COMPONENT_HPP
