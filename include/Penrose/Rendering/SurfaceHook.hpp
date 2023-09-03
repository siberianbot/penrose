#ifndef PENROSE_RENDERING_SURFACE_HOOK_HPP
#define PENROSE_RENDERING_SURFACE_HOOK_HPP

namespace Penrose {

    class Surface;

    class SurfaceHook {
    public:
        virtual ~SurfaceHook() = default;

        virtual void onSurfaceCreate(Surface *surface) = 0;
        virtual void onSurfaceDestroy(Surface *surface) = 0;
    };
}

#endif // PENROSE_RENDERING_SURFACE_HOOK_HPP
