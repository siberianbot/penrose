#ifndef PENROSE_BACKENDS_GLFW_BACKEND_HPP
#define PENROSE_BACKENDS_GLFW_BACKEND_HPP

#include "src/Core/Resource.hpp"

namespace Penrose {

    class GlfwBackend : public Resource {
    public:
        ~GlfwBackend() override = default;

        void init() override;
        void destroy() override;
    };
}

#endif // PENROSE_BACKENDS_GLFW_BACKEND_HPP
