#ifndef PENROSE_BUILTIN_PENROSE_ECS_ORTHOGRAPHIC_CAMERA_COMPONENT_HPP
#define PENROSE_BUILTIN_PENROSE_ECS_ORTHOGRAPHIC_CAMERA_COMPONENT_HPP

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ComponentFactory.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class OrthographicCameraComponent : public Component {
    public:
        ~OrthographicCameraComponent() override = default;

        [[nodiscard]] float &getTop() { return this->_top; }

        [[nodiscard]] float &getBottom() { return this->_bottom; }

        [[nodiscard]] float &getLeft() { return this->_left; }

        [[nodiscard]] float &getRight() { return this->_right; }

        [[nodiscard]] float &getNear() { return this->_near; }

        [[nodiscard]] float &getFar() { return this->_far; }

        [[nodiscard]] std::string getName() const override { return name(); }

        [[nodiscard]] constexpr static std::string name() { return "OrthographicCamera"; }

    private:
        float _top = 1.0f;
        float _bottom = -1.0f;
        float _left = -1.0f;
        float _right = 1.0f;
        float _near = 0.0001f;
        float _far = 100.0f;
    };

    class OrthographicCameraComponentFactory : public Resource,
                                               public GenericComponentFactory<OrthographicCameraComponent> {
    public:
        ~OrthographicCameraComponentFactory() override = default;
    };
}

#endif // PENROSE_BUILTIN_PENROSE_ECS_ORTHOGRAPHIC_CAMERA_COMPONENT_HPP
