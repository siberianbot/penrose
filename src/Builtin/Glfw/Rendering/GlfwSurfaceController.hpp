#ifndef PENROSE_BUILTIN_GLFW_RENDERING_GLFW_SURFACE_CONTROLLER_HPP
#define PENROSE_BUILTIN_GLFW_RENDERING_GLFW_SURFACE_CONTROLLER_HPP

#include <map>

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Rendering/SurfaceFactory.hpp>
#include <Penrose/Rendering/SurfaceHook.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/Builtin/Vulkan/VkSurfaceProvider.hpp"
#include "src/Builtin/Vulkan/VulkanBackend.hpp"

namespace Penrose {

    class ResourceSet;
    class GlfwSurface;

    class GlfwSurfaceController : public Resource,
                                  public SurfaceFactory,
                                  public SurfaceHook,
                                  public VkSurfaceProvider {
    public:
        explicit GlfwSurfaceController(ResourceSet *resources);
        ~GlfwSurfaceController() override = default;

        [[nodiscard]] Surface *makeSurface() override;

        [[nodiscard]] vk::SurfaceKHR getVkSurfaceFor(Surface *surface) override;

        void onSurfaceCreate(Surface *) override { /* nothing to do */ };

        void onSurfaceDestroy(Surface *surface) override;

    private:
        Lazy<EventQueue> _eventQueue;
        Lazy<VulkanBackend> _vulkanBackend;

        std::map<GLFWwindow *, vk::SurfaceKHR> _vkSurfaces;

        static void windowCloseCallback(GLFWwindow *handle);
        static void framebufferSizeCallback(GLFWwindow *handle, int width, int height);
    };
}

#endif // PENROSE_BUILTIN_GLFW_RENDERING_GLFW_SURFACE_CONTROLLER_HPP
