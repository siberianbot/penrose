#ifndef PENROSE_BUILTIN_GLFW_GLFW_BACKEND_HPP
#define PENROSE_BUILTIN_GLFW_GLFW_BACKEND_HPP

#include <vector>

#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/Updatable.hpp>

#include "src/Builtin/Vulkan/VkInstanceExtensionsProvider.hpp"

namespace Penrose {

    class ResourceSet;

    class GlfwBackend : public Resource, public Initializable, public Updatable, public VkInstanceExtensionsProvider {
    public:
        ~GlfwBackend() override = default;

        void init() override;
        void destroy() override;

        void update(float) override;

        [[nodiscard]] std::vector<const char *> getRequiredInstanceExtensions() const override;
    };

    ResourceSet &addGlfw(ResourceSet &resources);
}

#endif // PENROSE_BUILTIN_GLFW_GLFW_BACKEND_HPP
