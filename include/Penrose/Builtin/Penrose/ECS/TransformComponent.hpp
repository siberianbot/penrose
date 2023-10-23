#ifndef PENROSE_BUILTIN_PENROSE_ECS_TRANSFORM_COMPONENT_HPP
#define PENROSE_BUILTIN_PENROSE_ECS_TRANSFORM_COMPONENT_HPP

#include <glm/vec3.hpp>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ComponentFactory.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class TransformComponent : public Component {
    public:
        ~TransformComponent() override = default;

        [[nodiscard]] glm::vec3 &getPos() { return this->_pos; }

        [[nodiscard]] glm::vec3 &getRot() { return this->_rot; }

        [[nodiscard]] glm::vec3 &getScale() { return this->_scale; }

        [[nodiscard]] std::string getName() const override { return name(); }

        [[nodiscard]] constexpr static std::string name() { return "Transform"; }

    private:
        glm::vec3 _pos = glm::vec3(0);
        glm::vec3 _rot = glm::vec3(0);
        glm::vec3 _scale = glm::vec3(1);
    };

    class TransformComponentFactory : public Resource<TransformComponentFactory>,
                                      public GenericComponentFactory<TransformComponent> {
    public:
        ~TransformComponentFactory() override = default;
    };
}

#endif // PENROSE_BUILTIN_PENROSE_ECS_TRANSFORM_COMPONENT_HPP
