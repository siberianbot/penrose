#ifndef PENROSE_TYPES_PROJECTION_HPP
#define PENROSE_TYPES_PROJECTION_HPP

#include <variant>

namespace Penrose {

    /**
     * \brief Perspective projection parameters
     */
    struct PerspectiveProjection {

        /**
         * \brief Field of view in radians
         */
        float fov;

        /**
         * \brief Near plane distance
         */
        float near;

        /**
         * \brief Far plane distance
         */
        float far;
    };

    /**
     * \brief Orthographic projection parameters
     */
    struct OrthographicProjection {

        /**
         * \brief Top in-screen coordinate
         */
        float top;

        /**
         * \brief Bottom in-screen coordinate
         */
        float bottom;

        /**
         * \brief Left in-screen coordinate
         */
        float left;

        /**
         * \brief Right in-screen coordinate
         */
        float right;

        /**
         * \brief Near plane distance
         */
        float near;

        /**
         * \brief Far plane distance
         */
        float far;
    };

    /**
     * \brief Type for projection parameters
     */
    using Projection = std::variant<PerspectiveProjection, OrthographicProjection>;
}

#endif // PENROSE_TYPES_PROJECTION_HPP
