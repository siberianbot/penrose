#include "ImGuiBackend.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>

#include "src/Builtin/Glfw/Rendering/GlfwSurface.hpp"

namespace Penrose {

    void ImGuiBackend::init() {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
    }

    void ImGuiBackend::destroy() {
        ImGui::DestroyContext();
    }

    void ImGuiBackend::onSurfaceCreate(Surface *surface) {
        auto glfwSurface = dynamic_cast<GlfwSurface *>(surface);

        ImGui_ImplGlfw_InitForVulkan(glfwSurface->getHandle(), true);
    }

    bool ImGuiBackend::onKeyStateUpdate(InputKey key, InputState) {
        if (isKeyboardKey(key)) {
            return !ImGui::GetIO().WantCaptureKeyboard;
        }

        if (isMouseKey(key)) {
            return !ImGui::GetIO().WantCaptureMouse;
        }

        return true;
    }
}
