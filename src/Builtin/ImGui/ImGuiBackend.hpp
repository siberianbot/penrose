#ifndef PENROSE_BUILTIN_IMGUI_IMGUI_BACKEND_HPP
#define PENROSE_BUILTIN_IMGUI_IMGUI_BACKEND_HPP

#include <Penrose/Input/InputHook.hpp>
#include <Penrose/Rendering/SurfaceHook.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ResourceSet;

    class ImGuiBackend : public Resource<ImGuiBackend>,
                         public Initializable,
                         public SurfaceHook,
                         public InputHook {
    public:
        ~ImGuiBackend() override = default;

        void init() override;
        void destroy() override;

        void onSurfaceCreate(Surface *surface) override;

        void onSurfaceDestroy(Surface *) override { /* nothing to do */ }

        void onSurfaceInvalidated(Surface *) override { /* nothing to do */ }

        [[nodiscard]] bool onKeyStateUpdate(InputKey key, InputState) override;
    };
}

#endif // PENROSE_BUILTIN_IMGUI_IMGUI_BACKEND_HPP
