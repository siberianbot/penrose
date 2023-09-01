#include "GlfwSurface.hpp"

#include "src/Builtin/Glfw/Rendering/GlfwSurfaceController.hpp"

namespace Penrose {

    GlfwSurface::GlfwSurface(GlfwSurfaceController *surfaceController,
                             GLFWwindow *handle)
            : _surfaceController(surfaceController),
              _handle(handle) {
        //
    }

    GlfwSurface::~GlfwSurface() {
        this->_surfaceController->handleSurfaceDestroy(this);

        glfwDestroyWindow(this->_handle);
    }

    Size GlfwSurface::getSize() const {
        int width, height;
        glfwGetWindowSize(this->_handle, &width, &height);

        return {width, height};
    }
}
