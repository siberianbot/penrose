#include <Penrose/Builtin/Glfw.hpp>

#include <Penrose/Rendering/SurfaceManager.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Glfw/GlfwBackend.hpp"
#include "src/Builtin/Glfw/Rendering/GlfwSurfaceController.hpp"

namespace Penrose {

    ResourceSet &addGlfw(ResourceSet &resources) {

        resources.add<GlfwBackend>()
                .implements<VkInstanceExtensionsProvider>()
                .implements<Initializable>()
                .implements<Updatable>()
                .before<SurfaceManager>()
                .done();

        resources.add<GlfwSurfaceController>()
                .implements<SurfaceFactory>()
                .implements<SurfaceHook>()
                .implements<VkSurfaceProvider>()
                .before<SurfaceManager>()
                .done();

        return resources;
    }
}
