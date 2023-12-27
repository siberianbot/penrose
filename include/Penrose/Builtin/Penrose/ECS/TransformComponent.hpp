#ifndef PENROSE_BUILTIN_PENROSE_ECS_TRANSFORM_COMPONENT_HPP
#define PENROSE_BUILTIN_PENROSE_ECS_TRANSFORM_COMPONENT_HPP

#include <glm/vec3.hpp>

#include <Penrose/ECS/Component.hpp>

namespace Penrose {

    /**
     * \brief Transform component
     * \details Transform defines position, rotation and scale of entity in 3D world.
     */
    struct TransformComponent final: Component<TransformComponent> {
        ~TransformComponent() override = default;

        /**
         * \brief Position
         */
        glm::vec3 pos = glm::vec3(0);

        /**
         * \brief Rotation in radians
         */
        glm::vec3 rot = glm::vec3(0);

        /**
         * \brief Scale
         */
        glm::vec3 scale = glm::vec3(1);
    };
}

#endif // PENROSE_BUILTIN_PENROSE_ECS_TRANSFORM_COMPONENT_HPP
