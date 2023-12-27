#ifndef PENROSE_BUILTIN_PENROSE_ECS_MESH_COMPONENT_HPP
#define PENROSE_BUILTIN_PENROSE_ECS_MESH_COMPONENT_HPP

#include <optional>
#include <string>

#include <glm/vec3.hpp>

#include <Penrose/ECS/Component.hpp>

namespace Penrose {

    /**
     * \brief Mesh component
     * \details Defines mesh and textures for entity to rendered.
     */
    struct MeshComponent final: Component<MeshComponent> {
        ~MeshComponent() override = default;

        /**
         * \brief Name of mesh asset
         */
        std::optional<std::string> mesh = std::nullopt;

        /**
         * \brief Name of image asset to be used as albedo texture
         */
        std::optional<std::string> albedo = std::nullopt;

        /**
         * \brief Mesh color
         */
        glm::vec3 color = glm::vec3(1);
    };
}

#endif // PENROSE_BUILTIN_PENROSE_ECS_MESH_COMPONENT_HPP
