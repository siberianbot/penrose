#ifndef PENROSE_ECS_COMPONENTS_CAMERA_COMPONENT_HPP
#define PENROSE_ECS_COMPONENTS_CAMERA_COMPONENT_HPP

#include <glm/trigonometric.hpp>

#include "src/ECS/Components/Component.hpp"

namespace Penrose {

    class CameraComponent : public Component {
    private:
        float _fov = glm::radians(90.0f);
        float _near = 0.0001f;
        float _far = 100.0f;

    public:
        ~CameraComponent() override = default;

        [[nodiscard]] float &fov() { return this->_fov; }

        [[nodiscard]] float &near() { return this->_near; }

        [[nodiscard]] float &far() { return this->_far; }

        [[nodiscard]] static ComponentName name() { return "CameraComponent"; }
    };
}

#endif // PENROSE_ECS_COMPONENTS_CAMERA_COMPONENT_HPP
