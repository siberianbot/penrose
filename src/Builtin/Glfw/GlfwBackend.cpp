#include "GlfwBackend.hpp"

#include <cstdint>

#include <GLFW/glfw3.h>

#include <Penrose/Common/EngineError.hpp>

namespace Penrose {

    void GlfwBackend::init() {
        if (glfwInit() != GLFW_TRUE) {
            throw EngineError("Failed to initialize GLFW backend");
        }
    }

    void GlfwBackend::destroy() {
        glfwTerminate();
    }

    void GlfwBackend::update(float) {
        glfwPollEvents();
    }

    std::vector<const char *> GlfwBackend::getRequiredInstanceExtensions() const {
        std::uint32_t count = 0;
        auto extensions = glfwGetRequiredInstanceExtensions(&count);

        return {extensions, extensions + count};
    }
}