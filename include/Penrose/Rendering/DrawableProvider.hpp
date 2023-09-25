#ifndef PENROSE_RENDERING_DRAWABLE_PROVIDER_HPP
#define PENROSE_RENDERING_DRAWABLE_PROVIDER_HPP

#include <vector>

#include <Penrose/ECS/Entity.hpp>
#include <Penrose/Rendering/Drawable.hpp>

namespace Penrose {

    class DrawableProvider {
    public:
        virtual ~DrawableProvider() = default;

        [[nodiscard]] virtual std::vector<Drawable> getDrawablesFor(const Entity &entity) = 0;
    };
}

#endif // PENROSE_RENDERING_DRAWABLE_PROVIDER_HPP
