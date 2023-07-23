#include "ImGuiBackend.hpp"

#include <imgui.h>

namespace Penrose {

    void ImGuiBackend::init() {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
    }

    void ImGuiBackend::destroy() {
        ImGui::DestroyContext();
    }
}
