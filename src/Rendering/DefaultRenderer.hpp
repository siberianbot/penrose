#ifndef PENROSE_RENDERING_DEFAULT_RENDERER_HPP
#define PENROSE_RENDERING_DEFAULT_RENDERER_HPP

#include <Penrose/Rendering/Renderer.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class DefaultRenderer final: public Resource<DefaultRenderer>,
                                 public Renderer {
    public:
        ~DefaultRenderer() override = default;

        [[nodiscard]] std::string getName() const override { return "Default"; }

        void init() override;
        void destroy() override;

        // TODO: void setRenderGraph(GraphInfo &&graphInfo) override;
    };
}

#endif // PENROSE_RENDERING_DEFAULT_RENDERER_HPP
