#include "Surface.hpp"

#include <GLFW/glfw3.h>

#include "src/Backends/VulkanBackend.hpp"
#include "src/Common/EngineError.hpp"
#include "src/Core/ResourceSet.hpp"
#include "src/Events/EventQueue.hpp"

namespace Penrose {

    void Surface::windowCloseCallback(GLFWwindow *handle) {
        auto that = reinterpret_cast<Surface *>(glfwGetWindowUserPointer(handle));

        that->_eventQueue->push(makeEvent(EventType::EngineDestroyRequested));
    }

    void Surface::framebufferSizeCallback(GLFWwindow *handle, int width, int height) {
        auto that = reinterpret_cast<Surface *>(glfwGetWindowUserPointer(handle));

        that->_eventQueue->push(makeEvent(EventType::SurfaceResized, SurfaceSize(width, height)));
    }

    Surface::Surface(ResourceSet *resources)
            : _eventQueue(resources->get<EventQueue>()->lock()),
              _vulkanBackend(resources->get<VulkanBackend>()->lock()) {
        //
    }

    void Surface::init() {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        auto handle = glfwCreateWindow(1280, 720, "Penrose", nullptr, nullptr);

        if (handle == nullptr) {
            throw EngineError("Failed to create surface instance");
        }

        glfwSetWindowUserPointer(handle, this);
        glfwSetWindowCloseCallback(handle, Surface::windowCloseCallback);
        glfwSetFramebufferSizeCallback(handle, Surface::framebufferSizeCallback);

        this->_handle = handle;

        VkSurfaceKHR surface;
        auto result = glfwCreateWindowSurface(this->_vulkanBackend->getInstance(),
                                              this->_handle,
                                              nullptr,
                                              &surface);

        if (result != VkResult::VK_SUCCESS) {
            throw EngineError("Failed to create VK surface for surface instance");
        }

        this->_surface = surface;
    }

    void Surface::destroy() {
        if (this->_surface.has_value()) {
            this->_vulkanBackend->getInstance().destroy(this->_surface.value());
            this->_surface = std::nullopt;
        }

        if (this->_handle != nullptr) {
            glfwDestroyWindow(this->_handle);
            this->_handle = nullptr;
        }
    }

    void Surface::poll() { // NOLINT(readability-convert-member-functions-to-static)
        glfwPollEvents();
    }

    SurfaceSize Surface::getSize() const {
        int width, height;
        glfwGetWindowSize(this->_handle, &width, &height);

        return {width, height};
    }
}
