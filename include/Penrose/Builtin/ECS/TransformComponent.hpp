#ifndef PENROSE_BUILTIN_ECS_TRANSFORM_COMPONENT_HPP
#define PENROSE_BUILTIN_ECS_TRANSFORM_COMPONENT_HPP

#include <glm/vec3.hpp>

#include <Penrose/ECS/Component.hpp>

namespace Penrose {

    class TransformComponent : public Component {
    public:
        ~TransformComponent() override = default;

        [[nodiscard]] glm::vec3 &getPos() { return this->_pos; }

        [[nodiscard]] glm::vec3 &getRot() { return this->_rot; }

        [[nodiscard]] glm::vec3 &getScale() { return this->_scale; }

        [[nodiscard]] static constexpr std::string_view name() { return "Transform"; }

    private:
        glm::vec3 _pos = glm::vec3(0);
        glm::vec3 _rot = glm::vec3(0);
        glm::vec3 _scale = glm::vec3(1);
    };
}

#endif // PENROSE_BUILTIN_ECS_TRANSFORM_COMPONENT_HPP
