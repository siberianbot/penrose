#include "GlfwSurfaceController.hpp"

#include <Penrose/Common/EngineError.hpp>

#include "src/Builtin/Glfw/Rendering/GlfwSurface.hpp"
#include "src/Builtin/Glfw/Utils/InputUtils.hpp"

namespace Penrose {

    GlfwSurfaceController::GlfwSurfaceController(const ResourceSet *resources)
        : _eventQueue(resources->get<SurfaceEventQueue>()),
          _inputHandler(resources->get<InputHandler>()),
          _surfaceManager(resources->get<SurfaceManager>()),
          _vulkanBackend(resources->get<VulkanBackend>()) {
        //
    }

    Surface *GlfwSurfaceController::makeSurface() {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        auto handle = glfwCreateWindow(1280, 720, "Penrose", nullptr, nullptr);

        if (handle == nullptr) {
            throw EngineError("Failed to create surface instance");
        }

        glfwSetInputMode(handle, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

        glfwSetWindowUserPointer(handle, this);
        glfwSetWindowCloseCallback(handle, GlfwSurfaceController::windowCloseCallback);
        glfwSetFramebufferSizeCallback(handle, GlfwSurfaceController::framebufferSizeCallback);
        glfwSetKeyCallback(handle, GlfwSurfaceController::keyCallback);
        glfwSetMouseButtonCallback(handle, GlfwSurfaceController::mouseButtonCallback);
        glfwSetCursorPosCallback(handle, GlfwSurfaceController::cursorPosCallback);
        glfwSetScrollCallback(handle, GlfwSurfaceController::scrollCallback);

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
        auto result = glfwCreateWindowSurface(
            this->_vulkanBackend->getInstance(), glfwSurface->getHandle(), nullptr, &vkSurface
        );

        if (result != VkResult::VK_SUCCESS) {
            throw EngineError("Failed to create VK surface for surface instance");
        }

        this->_vkSurfaces.emplace(glfwSurface->getHandle(), vkSurface);

        return vkSurface;
    }

    void GlfwSurfaceController::windowCloseCallback(GLFWwindow *handle) {
        auto that = reinterpret_cast<GlfwSurfaceController *>(glfwGetWindowUserPointer(handle));

        that->_eventQueue->push(SurfaceClosedEvent {
            .surface = that->_surfaceManager->getSurface(),
        });
    }

    void GlfwSurfaceController::framebufferSizeCallback(GLFWwindow *handle, int w, int h) {
        auto that = reinterpret_cast<GlfwSurfaceController *>(glfwGetWindowUserPointer(handle));

        that->_surfaceManager->invalidate(); // TODO: remove

        that->_eventQueue->push(SurfaceResizedEvent {
            .surface = that->_surfaceManager->getSurface(),
            .size = {w, h},
        });
    }

    void GlfwSurfaceController::keyCallback(GLFWwindow *handle, int key, int, int action, int) {
        auto that = reinterpret_cast<GlfwSurfaceController *>(glfwGetWindowUserPointer(handle));

        auto inputKey = fromGlfwKeyboardKey(key);
        auto inputState = fromGlfwAction(action);

        that->_inputHandler->pushKeyStateUpdate(inputKey, inputState);
    }

    void GlfwSurfaceController::mouseButtonCallback(GLFWwindow *handle, int button, int action, int) {
        auto that = reinterpret_cast<GlfwSurfaceController *>(glfwGetWindowUserPointer(handle));

        auto inputKey = fromGlfwMouseButton(button);
        auto inputState = fromGlfwAction(action);

        that->_inputHandler->pushKeyStateUpdate(inputKey, inputState);
    }

    void GlfwSurfaceController::cursorPosCallback(GLFWwindow *handle, double x, double y) {
        auto that = reinterpret_cast<GlfwSurfaceController *>(glfwGetWindowUserPointer(handle));

        int w, h;
        glfwGetWindowSize(handle, &w, &h);

        auto ndcX = static_cast<float>(x) / (static_cast<float>(w) / 2) - 1;
        auto ndcY = 1 - static_cast<float>(y) / (static_cast<float>(h) / 2);

        that->_inputHandler->pushMouseMove(ndcX, ndcY);

        if (glfwGetInputMode(handle, GLFW_CURSOR) != GLFW_CURSOR_DISABLED) {
            return;
        }

        glfwSetCursorPos(handle, std::clamp(x, 0., static_cast<double>(w)), std::clamp(y, 0., static_cast<double>(h)));
    }

    void GlfwSurfaceController::scrollCallback(GLFWwindow *handle, double dx, double dy) {
        auto that = reinterpret_cast<GlfwSurfaceController *>(glfwGetWindowUserPointer(handle));

        that->_inputHandler->pushScroll(static_cast<float>(dx), static_cast<float>(dy));
    }
}
