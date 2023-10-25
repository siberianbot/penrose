#ifndef PENROSE_RENDERING_DEFAULT_DRAWABLE_PROVIDER_HPP
#define PENROSE_RENDERING_DEFAULT_DRAWABLE_PROVIDER_HPP

#include <Penrose/ECS/EntityManager.hpp>
#include <Penrose/Rendering/DrawableProvider.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    class ResourceSet;

    class DefaultDrawableProvider : public Resource<DefaultDrawableProvider, ResourceGroup::Rendering>,
                                    public DrawableProvider {
    public:
        explicit DefaultDrawableProvider(ResourceSet *resources);
        ~DefaultDrawableProvider() override = default;

        [[nodiscard]] std::vector<Drawable> getDrawablesFor(const Entity &entity) override;

    private:
        ResourceProxy<EntityManager> _entityManager;
    };
}

#endif // PENROSE_RENDERING_DEFAULT_DRAWABLE_PROVIDER_HPP
