#ifndef PENROSE_BACKENDS_GLFW_BACKEND_HPP
#define PENROSE_BACKENDS_GLFW_BACKEND_HPP

#include <vector>

#include <Penrose/Common/Initializable.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class GlfwBackend : public Resource, public Initializable {
    public:
        ~GlfwBackend() override = default;

        void init() override;
        void destroy() override;

        [[nodiscard]] std::vector<const char *> getRequiredInstanceExtensions() const;
    };
}

#endif // PENROSE_BACKENDS_GLFW_BACKEND_HPP
