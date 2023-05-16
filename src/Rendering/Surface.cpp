#include "Surface.hpp"

#include <GLFW/glfw3.h>

#include "src/Common/EngineError.hpp"

namespace Penrose {

    void Surface::init() {
        auto handle = glfwCreateWindow(1280, 720, "Penrose", nullptr, nullptr);

        if (handle == nullptr) {
            throw EngineError("Failed to create surface");
        }

        this->_handle = handle;
    }

    void Surface::destroy() {
        glfwDestroyWindow(this->_handle);
    }
}
