#include "Surface.hpp"

#include <GLFW/glfw3.h>

#include "src/Common/EngineError.hpp"
#include "src/Core/ResourceSet.hpp"
#include "src/Events/EventQueue.hpp"

namespace Penrose {

    void Surface::closeCallback(GLFWwindow *handle) {
        auto that = reinterpret_cast<Surface *>(glfwGetWindowUserPointer(handle));

        that->_eventQueue->push(Event{
                .type = EventType::EngineDestroyRequested
        });
    }

    Surface::Surface(ResourceSet *resources)
            : _eventQueue(resources->get<EventQueue>()->lock()) {
        //
    }

    void Surface::init() {
        auto handle = glfwCreateWindow(1280, 720, "Penrose", nullptr, nullptr);

        if (handle == nullptr) {
            throw EngineError("Failed to create surface");
        }

        glfwSetWindowUserPointer(handle, this);
        glfwSetWindowCloseCallback(handle, Surface::closeCallback);

        this->_handle = handle;
    }

    void Surface::destroy() {
        glfwDestroyWindow(this->_handle);
    }

    void Surface::poll() { // NOLINT(readability-convert-member-functions-to-static)
        glfwPollEvents();
    }
}
