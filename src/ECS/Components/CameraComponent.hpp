#ifndef PENROSE_ECS_COMPONENTS_CAMERA_COMPONENT_HPP
#define PENROSE_ECS_COMPONENTS_CAMERA_COMPONENT_HPP

#include <glm/trigonometric.hpp>

#include "src/ECS/ECSBase.hpp"
#include "src/ECS/Components/Component.hpp"

namespace Penrose {

    class CameraComponent : public Component {
    private:
        float _fov = glm::radians(90.0f);
        float _near = 0.0001f;
        float _far = 100.0f;

    public:
        ~CameraComponent() override = default;

        [[nodiscard]] float &getFov() { return this->_fov; }

        [[nodiscard]] float &getNear() { return this->_near; }

        [[nodiscard]] float &getFar() { return this->_far; }

        [[nodiscard]] static constexpr ComponentName name() { return "Camera"; }
    };
}

#endif // PENROSE_ECS_COMPONENTS_CAMERA_COMPONENT_HPP
