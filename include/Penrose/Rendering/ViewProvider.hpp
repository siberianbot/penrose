#ifndef PENROSE_RENDERING_VIEW_PROVIDER_HPP
#define PENROSE_RENDERING_VIEW_PROVIDER_HPP

#include <optional>

#include <Penrose/ECS/Entity.hpp>
#include <Penrose/Rendering/View.hpp>

namespace Penrose {

    class ViewProvider {
    public:
        virtual ~ViewProvider() = default;

        [[nodiscard]] virtual std::optional<View> tryGetViewFor(const Entity &entity) = 0;
    };
}

#endif // PENROSE_RENDERING_VIEW_PROVIDER_HPP
