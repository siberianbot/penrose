#ifndef PENROSE_BUILTIN_BACKENDS_IMGUI_BACKEND_HPP
#define PENROSE_BUILTIN_BACKENDS_IMGUI_BACKEND_HPP

#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ImGuiBackend : public Resource, public Initializable {
    public:
        ~ImGuiBackend() override = default;

        void init() override;
        void destroy() override;
    };
}

#endif // PENROSE_BUILTIN_BACKENDS_IMGUI_BACKEND_HPP
