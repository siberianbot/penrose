#ifndef PENROSE_BUILTIN_PENROSE_ECS_PERSPECTIVE_CAMERA_COMPONENT_HPP
#define PENROSE_BUILTIN_PENROSE_ECS_PERSPECTIVE_CAMERA_COMPONENT_HPP

#include <glm/trigonometric.hpp>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ComponentFactory.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class PerspectiveCameraComponent : public Component {
    public:
        ~PerspectiveCameraComponent() override = default;

        [[nodiscard]] float &getFov() { return this->_fov; }

        [[nodiscard]] float &getNear() { return this->_near; }

        [[nodiscard]] float &getFar() { return this->_far; }

        [[nodiscard]] std::string getName() const override { return name(); }

        [[nodiscard]] constexpr static std::string name() { return "PerspectiveCamera"; }

    private:
        float _fov = glm::radians(90.0f);
        float _near = 0.0001f;
        float _far = 100.0f;
    };

    class PerspectiveCameraComponentFactory : public Resource<PerspectiveCameraComponentFactory>,
                                              public GenericComponentFactory<PerspectiveCameraComponent> {
    public:
        ~PerspectiveCameraComponentFactory() override = default;
    };
}

#endif // PENROSE_BUILTIN_PENROSE_ECS_PERSPECTIVE_CAMERA_COMPONENT_HPP
