#ifndef PENROSE_BUILTIN_PENROSE_ECS_VIEW_COMPONENT_HPP
#define PENROSE_BUILTIN_PENROSE_ECS_VIEW_COMPONENT_HPP

#include <string>

#include <glm/trigonometric.hpp>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/Types/Projection.hpp>

namespace Penrose {

    /**
     * \brief View component
     * \details Entities with view components are treated as camera.
     */
    struct PENROSE_API ViewComponent final: Component<ViewComponent> {
        ~ViewComponent() override = default;

        /**
         * \brief Name of view
         */
        std::string name = "Default";

        /**
         * \brief Projection parameters
         */
        Projection projection = PerspectiveProjection {
            .fov = glm::radians(90.0f),
            .near = 0.001f,
            .far = 100.000f,
        };
    };
}

#endif // PENROSE_BUILTIN_PENROSE_ECS_VIEW_COMPONENT_HPP
