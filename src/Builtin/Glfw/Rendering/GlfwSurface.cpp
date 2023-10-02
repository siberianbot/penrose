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

    void GlfwSurface::setSize(const Size &size) {
        auto [width, height] = size;

        glfwSetWindowSize(this->_handle, static_cast<int>(width), static_cast<int>(height));
    }

    void GlfwSurface::lockCursor() {
        glfwSetInputMode(this->_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void GlfwSurface::unlockCursor() {
        glfwSetInputMode(this->_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}
