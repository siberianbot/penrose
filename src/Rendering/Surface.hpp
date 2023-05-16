#ifndef PENROSE_RENDERING_SURFACE_HPP
#define PENROSE_RENDERING_SURFACE_HPP

#include "src/Core/Resource.hpp"

struct GLFWwindow;

namespace Penrose {

    class Surface : public Resource {
    private:
        GLFWwindow *_handle = nullptr;

    public:
        ~Surface() override = default;

        void init() override;
        void destroy() override;
    };
}

#endif // PENROSE_RENDERING_SURFACE_HPP
