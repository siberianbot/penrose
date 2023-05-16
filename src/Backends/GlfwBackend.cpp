#include "GlfwBackend.hpp"

#include <GLFW/glfw3.h>

#include "src/Common/EngineError.hpp"

namespace Penrose {

    void GlfwBackend::init() {
        if (glfwInit() != GLFW_TRUE) {
            throw EngineError("Failed to initialize GLFW backend");
        }
    }

    void GlfwBackend::destroy() {
        glfwTerminate();
    }
}