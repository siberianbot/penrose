#ifndef PENROSE_BUILTIN_GLFW_RENDERING_GLFW_SURFACE_HPP
#define PENROSE_BUILTIN_GLFW_RENDERING_GLFW_SURFACE_HPP

#include <GLFW/glfw3.h>

#include <Penrose/Rendering/Surface.hpp>

namespace Penrose {

    class GlfwSurface : public Surface {
    public:
        explicit GlfwSurface(GLFWwindow *handle);
        ~GlfwSurface() override;

        [[nodiscard]] Size getSize() const override;

        void setSize(const Size &size) override;

        [[nodiscard]] bool isCursorLocked() const override;

        void lockCursor() override;
        void unlockCursor() override;

        [[nodiscard]] GLFWwindow *getHandle() const { return this->_handle; }

    private:
        GLFWwindow *_handle;
    };
}

#endif // PENROSE_BUILTIN_GLFW_RENDERING_GLFW_SURFACE_HPP
