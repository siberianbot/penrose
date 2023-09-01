#ifndef PENROSE_BUILTIN_GLFW_RENDERING_GLFW_SURFACE_HPP
#define PENROSE_BUILTIN_GLFW_RENDERING_GLFW_SURFACE_HPP

#include <GLFW/glfw3.h>

#include <Penrose/Rendering/Surface.hpp>

namespace Penrose {

    class GlfwSurfaceController;

    class GlfwSurface : public Surface {
    public:
        explicit GlfwSurface(GlfwSurfaceController *surfaceController,
                             GLFWwindow *handle);
        ~GlfwSurface() override;

        [[nodiscard]] Size getSize() const override;

        [[nodiscard]] GLFWwindow *getHandle() const { return this->_handle; }

    private:
        GlfwSurfaceController *_surfaceController;
        GLFWwindow *_handle;
    };
}

#endif // PENROSE_BUILTIN_GLFW_RENDERING_GLFW_SURFACE_HPP
