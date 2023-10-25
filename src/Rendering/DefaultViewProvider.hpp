#ifndef PENROSE_RENDERING_DEFAULT_VIEW_PROVIDER_HPP
#define PENROSE_RENDERING_DEFAULT_VIEW_PROVIDER_HPP

#include <Penrose/ECS/EntityManager.hpp>
#include <Penrose/Rendering/ViewProvider.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    class ResourceSet;

    class DefaultViewProvider : public Resource<DefaultViewProvider>,
                                public ViewProvider {
    public:
        explicit DefaultViewProvider(ResourceSet *resources);
        ~DefaultViewProvider() override = default;

        [[nodiscard]] std::optional<View> tryGetViewFor(const Entity &entity) override;

    private:
        ResourceProxy<EntityManager> _entityManager;
    };
}

#endif // PENROSE_RENDERING_DEFAULT_VIEW_PROVIDER_HPP
