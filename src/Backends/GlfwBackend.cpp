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

    std::vector<const char *>
    GlfwBackend::getRequiredInstanceExtensions() const { // NOLINT(readability-convert-member-functions-to-static)
        std::uint32_t count = 0;
        const char **extensions = glfwGetRequiredInstanceExtensions(&count);

        return {extensions, extensions + count};
    }
}