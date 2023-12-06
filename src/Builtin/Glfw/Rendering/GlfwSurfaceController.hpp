#ifndef PENROSE_BUILTIN_GLFW_RENDERING_GLFW_SURFACE_CONTROLLER_HPP
#define PENROSE_BUILTIN_GLFW_RENDERING_GLFW_SURFACE_CONTROLLER_HPP

#include <map>

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include <Penrose/Events/SurfaceEvents.hpp>
#include <Penrose/Input/InputHandler.hpp>
#include <Penrose/Rendering/SurfaceFactory.hpp>
#include <Penrose/Rendering/SurfaceHook.hpp>
#include <Penrose/Rendering/SurfaceManager.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/VkSurfaceProvider.hpp"
#include "src/Builtin/Vulkan/VulkanBackend.hpp"

namespace Penrose {

    class GlfwSurface;

    class GlfwSurfaceController : public Resource<GlfwSurfaceController>,
                                  public SurfaceFactory,
                                  public SurfaceHook,
                                  public VkSurfaceProvider {
    public:
        explicit GlfwSurfaceController(const ResourceSet *resources);
        ~GlfwSurfaceController() override = default;

        [[nodiscard]] Surface *makeSurface() override;

        [[nodiscard]] vk::SurfaceKHR getVkSurfaceFor(Surface *surface) override;

        void onSurfaceCreate(Surface *) override { /* nothing to do */ };

        void onSurfaceDestroy(Surface *surface) override;

        void onSurfaceInvalidated(Surface *) override { /* nothing to do */ }

    private:
        ResourceProxy<SurfaceEventQueue> _eventQueue;
        ResourceProxy<InputHandler> _inputHandler;
        ResourceProxy<SurfaceManager> _surfaceManager;
        ResourceProxy<VulkanBackend> _vulkanBackend;

        std::map<GLFWwindow *, vk::SurfaceKHR> _vkSurfaces;

        static void windowCloseCallback(GLFWwindow *handle);
        static void framebufferSizeCallback(GLFWwindow *handle, int width, int height);
        static void keyCallback(GLFWwindow *handle, int key, int scancode, int action, int mods);
        static void mouseButtonCallback(GLFWwindow *handle, int button, int action, int mods);
        static void cursorPosCallback(GLFWwindow *handle, double x, double y);
        static void scrollCallback(GLFWwindow *handle, double dx, double dy);
    };
}

#endif // PENROSE_BUILTIN_GLFW_RENDERING_GLFW_SURFACE_CONTROLLER_HPP
