#ifndef PENROSE_BUILTIN_GLFW_GLFW_BACKEND_HPP
#define PENROSE_BUILTIN_GLFW_GLFW_BACKEND_HPP

#include <vector>

#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/Updatable.hpp>

#include <Penrose/Builtin/Vulkan/VkInstanceExtensionsProvider.hpp>

namespace Penrose {

    class ResourceSet;

    class GlfwBackend : public Resource<GlfwBackend>,
                        public Initializable,
                        public Updatable,
                        public VkInstanceExtensionsProvider {
    public:
        ~GlfwBackend() override = default;

        void init() override;
        void destroy() override;

        void update(float) override;

        [[nodiscard]] std::vector<std::string_view> getRequiredInstanceExtensions() const override;
    };
}

#endif // PENROSE_BUILTIN_GLFW_GLFW_BACKEND_HPP
