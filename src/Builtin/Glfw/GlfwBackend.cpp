#include "GlfwBackend.hpp"

#include <cstdint>

#include <GLFW/glfw3.h>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Glfw/Rendering/GlfwSurfaceController.hpp"

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

    ResourceSet &addGlfw(ResourceSet &resources) {

        resources.add<GlfwBackend, VkInstanceExtensionsProvider>(resources.getBeginIterator());
        resources.add<GlfwSurfaceController, SurfaceFactory, VkSurfaceProvider>();

        return resources;
    }
}