#ifndef PENROSE_ECS_COMPONENTS_TRANSFORM_COMPONENT_HPP
#define PENROSE_ECS_COMPONENTS_TRANSFORM_COMPONENT_HPP

#include <glm/vec3.hpp>

#include "src/ECS/Components/Component.hpp"

namespace Penrose {

    class TransformComponent : public Component {
    private:
        glm::vec3 _pos;
        glm::vec3 _rot;
        glm::vec3 _scale;

    public:
        ~TransformComponent() override = default;

        [[nodiscard]] glm::vec3 &getPos() { return this->_pos; }

        [[nodiscard]] glm::vec3 &getRot() { return this->_rot; }

        [[nodiscard]] glm::vec3 &getScale() { return this->_scale; }

        static ComponentName name() { return "Transform"; }
    };
}

#endif // PENROSE_ECS_COMPONENTS_TRANSFORM_COMPONENT_HPP