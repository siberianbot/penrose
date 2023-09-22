#ifndef PENROSE_BUILTIN_PENROSE_ECS_CAMERA_COMPONENT_HPP
#define PENROSE_BUILTIN_PENROSE_ECS_CAMERA_COMPONENT_HPP

#include <glm/trigonometric.hpp>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ComponentFactory.hpp>
#include <Penrose/Rendering/RenderList.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class CameraComponent : public Component, public ViewVisitor {
    public:
        ~CameraComponent() override = default;

        [[nodiscard]] float &getFov() { return this->_fov; }

        [[nodiscard]] float &getNear() { return this->_near; }

        [[nodiscard]] float &getFar() { return this->_far; }

        void visit(View *view) const override {
            view->projection = Perspective{
                    .fov = this->_fov,
                    .near = this->_near,
                    .far = this->_far
            };
        }

        [[nodiscard]] constexpr static std::string name() { return "Camera"; }

    private:
        float _fov = glm::radians(90.0f);
        float _near = 0.0001f;
        float _far = 100.0f;
    };

    class CameraComponentFactory : public Resource, public GenericComponentFactory<CameraComponent> {
    public:
        ~CameraComponentFactory() override = default;
    };
}

#endif // PENROSE_BUILTIN_PENROSE_ECS_CAMERA_COMPONENT_HPP
