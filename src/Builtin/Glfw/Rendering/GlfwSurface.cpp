#include "GlfwSurface.hpp"

namespace Penrose {

    GlfwSurface::GlfwSurface(GLFWwindow *handle)
            : _handle(handle) {
        //
    }

    GlfwSurface::~GlfwSurface() {
        glfwDestroyWindow(this->_handle);
    }

    Size GlfwSurface::getSize() const {
        int width, height;
        glfwGetWindowSize(this->_handle, &width, &height);

        return {width, height};
    }
}
