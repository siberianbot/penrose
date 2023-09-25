#ifndef PENROSE_RENDERING_DEFAULT_DRAWABLE_PROVIDER_HPP
#define PENROSE_RENDERING_DEFAULT_DRAWABLE_PROVIDER_HPP

#include <Penrose/ECS/ECSManager.hpp>
#include <Penrose/Rendering/DrawableProvider.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ResourceSet;

    class DefaultDrawableProvider : public Resource, public DrawableProvider {
    public:
        explicit DefaultDrawableProvider(ResourceSet *resources);
        ~DefaultDrawableProvider() override = default;

        [[nodiscard]] std::vector<Drawable> getDrawablesFor(const Entity &entity) override;

    private:
        Lazy<ECSManager> _ecsManager;
    };
}

#endif // PENROSE_RENDERING_DEFAULT_DRAWABLE_PROVIDER_HPP
