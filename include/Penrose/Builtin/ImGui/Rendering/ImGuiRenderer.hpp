#ifndef PENROSE_BUILTIN_IMGUI_RENDERING_IMGUI_RENDERER_HPP
#define PENROSE_BUILTIN_IMGUI_RENDERING_IMGUI_RENDERER_HPP

#include <Penrose/Rendering/Renderer.hpp>

namespace Penrose {

    /**
     * \brief ImGui renderer interface
     */
    class ImGuiRenderer: public Renderer {
    public:
        ~ImGuiRenderer() override = default;

        /**
         * \brief Get name of renderer
         * \return Name of renderer
         */
        [[nodiscard]] static std::string name() { return "ImGui"; }

        //! \copydoc Renderer::getName
        [[nodiscard]] std::string getName() const override { return name(); }
    };
}

#endif // PENROSE_BUILTIN_IMGUI_RENDERING_IMGUI_RENDERER_HPP
