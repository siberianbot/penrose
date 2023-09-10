#include "GlfwSurfaceController.hpp"

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Glfw/Rendering/GlfwSurface.hpp"

namespace Penrose {

    GlfwSurfaceController::GlfwSurfaceController(ResourceSet *resources)
            : _eventQueue(resources->getLazy<EventQueue>()),
              _surfaceManager(resources->getLazy<SurfaceManager>()),
              _vulkanBackend(resources->getLazy<VulkanBackend>()) {
        //
    }

    Surface *GlfwSurfaceController::makeSurface() {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        auto handle = glfwCreateWindow(1280, 720, "Penrose", nullptr, nullptr);

        if (handle == nullptr) {
            throw EngineError("Failed to create surface instance");
        }

        glfwSetWindowUserPointer(handle, this);
        glfwSetWindowCloseCallback(handle, GlfwSurfaceController::windowCloseCallback);
        glfwSetFramebufferSizeCallback(handle, GlfwSurfaceController::framebufferSizeCallback);

        return new GlfwSurface(handle);
    }

    void GlfwSurfaceController::onSurfaceDestroy(Surface *surface) {
        auto glfwSurface = dynamic_cast<GlfwSurface *>(surface);
        auto vkSurfaceIt = this->_vkSurfaces.find(glfwSurface->getHandle());

        if (vkSurfaceIt == this->_vkSurfaces.end()) {
            return;
        }

        this->_vulkanBackend->getInstance().destroy(vkSurfaceIt->second);
        this->_vkSurfaces.erase(glfwSurface->getHandle());
    }

    vk::SurfaceKHR GlfwSurfaceController::getVkSurfaceFor(Surface *surface) {
        auto glfwSurface = dynamic_cast<GlfwSurface *>(surface);

        auto vkSurfaceIt = this->_vkSurfaces.find(glfwSurface->getHandle());

        if (vkSurfaceIt != this->_vkSurfaces.end()) {
            return vkSurfaceIt->second;
        }

        VkSurfaceKHR vkSurface;
        auto result = glfwCreateWindowSurface(this->_vulkanBackend->getInstance(),
                                              glfwSurface->getHandle(),
                                              nullptr,
                                              &vkSurface);

        if (result != VkResult::VK_SUCCESS) {
            throw EngineError("Failed to create VK surface for surface instance");
        }

        this->_vkSurfaces.emplace(glfwSurface->getHandle(), vkSurface);

        return vkSurface;
    }

    void GlfwSurfaceController::windowCloseCallback(GLFWwindow *handle) {
        auto that = reinterpret_cast<GlfwSurfaceController *>(glfwGetWindowUserPointer(handle));

        that->_eventQueue->push(makeEvent(EventType::EngineDestroyRequested));
    }

    void GlfwSurfaceController::framebufferSizeCallback(GLFWwindow *handle, int, int) {
        auto that = reinterpret_cast<GlfwSurfaceController *>(glfwGetWindowUserPointer(handle));

        that->_surfaceManager->invalidate();
    }
}
