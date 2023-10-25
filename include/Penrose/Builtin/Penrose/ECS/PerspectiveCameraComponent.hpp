#ifndef PENROSE_BUILTIN_PENROSE_ECS_PERSPECTIVE_CAMERA_COMPONENT_HPP
#define PENROSE_BUILTIN_PENROSE_ECS_PERSPECTIVE_CAMERA_COMPONENT_HPP

#include <glm/trigonometric.hpp>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ComponentFactory.hpp>

namespace Penrose {

    class PerspectiveCameraComponent : public Component<PerspectiveCameraComponent> {
    public:
        ~PerspectiveCameraComponent() override = default;

        [[nodiscard]] float &getFov() { return this->_fov; }

        [[nodiscard]] float &getNear() { return this->_near; }

        [[nodiscard]] float &getFar() { return this->_far; }

    private:
        float _fov = glm::radians(90.0f);
        float _near = 0.0001f;
        float _far = 100.0f;
    };

    using PerspectiveCameraComponentFactory = GenericComponentFactory<PerspectiveCameraComponent>;
}

#endif // PENROSE_BUILTIN_PENROSE_ECS_PERSPECTIVE_CAMERA_COMPONENT_HPP
