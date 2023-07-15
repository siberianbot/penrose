#ifndef PENROSE_BUILTIN_ECS_CAMERA_COMPONENT_HPP
#define PENROSE_BUILTIN_ECS_CAMERA_COMPONENT_HPP

#include <glm/trigonometric.hpp>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/Rendering/RenderList.hpp>

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

        [[nodiscard]] static constexpr std::string_view name() { return "Camera"; }

    private:
        float _fov = glm::radians(90.0f);
        float _near = 0.0001f;
        float _far = 100.0f;
    };
}

#endif // PENROSE_BUILTIN_ECS_CAMERA_COMPONENT_HPP
