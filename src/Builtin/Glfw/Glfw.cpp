#include <Penrose/Builtin/Glfw.hpp>

#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Glfw/GlfwBackend.hpp"
#include "src/Builtin/Glfw/Rendering/GlfwSurfaceController.hpp"

namespace Penrose {

    ResourceSet &addGlfw(ResourceSet &resources) {

        resources.add<GlfwBackend, VkInstanceExtensionsProvider>(resources.getBeginIterator());
        resources.add<GlfwSurfaceController, SurfaceFactory, SurfaceHook, VkSurfaceProvider>();

        return resources;
    }
}
