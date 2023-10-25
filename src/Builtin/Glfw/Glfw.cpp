#include <Penrose/Builtin/Glfw.hpp>

#include "src/Builtin/Glfw/GlfwBackend.hpp"
#include "src/Builtin/Glfw/Rendering/GlfwSurfaceController.hpp"

namespace Penrose {

    ResourceSet &addGlfw(ResourceSet &resources) {

        resources.add<GlfwBackend, ResourceGroup::Backend>()
                .implements<VkInstanceExtensionsProvider>()
                .implements<Initializable>()
                .implements<Updatable>()
                .done();

        resources.add<GlfwSurfaceController, ResourceGroup::Windowing>()
                .implements<SurfaceFactory>()
                .implements<SurfaceHook>()
                .implements<VkSurfaceProvider>()
                .done();

        return resources;
    }
}
