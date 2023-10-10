#include "GlfwSurface.hpp"

#include <Penrose/Common/EngineError.hpp>

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

    bool GlfwSurface::isCursorLocked() const {
        auto currentMode = glfwGetInputMode(this->_handle, GLFW_CURSOR);

        switch (currentMode) {
            case GLFW_CURSOR_NORMAL:
                return false;

            case GLFW_CURSOR_DISABLED:
                return true;

            default:
                throw EngineError("GLFW cursor mode {:#x} not supported", currentMode);
        }
    }

    void GlfwSurface::lockCursor() {
        glfwSetInputMode(this->_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void GlfwSurface::unlockCursor() {
        glfwSetInputMode(this->_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}
