#ifndef PENROSE_RENDERING_RENDER_SYSTEM_HPP
#define PENROSE_RENDERING_RENDER_SYSTEM_HPP

namespace Penrose {

    class RenderSystem {
    public:
        virtual ~RenderSystem() = default;

        virtual void renderFrame() = 0;
    };
}

#endif // PENROSE_RENDERING_RENDER_SYSTEM_HPP
