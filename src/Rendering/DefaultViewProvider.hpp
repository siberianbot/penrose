#ifndef PENROSE_RENDERING_DEFAULT_VIEW_PROVIDER_HPP
#define PENROSE_RENDERING_DEFAULT_VIEW_PROVIDER_HPP

#include <Penrose/ECS/ECSManager.hpp>
#include <Penrose/Rendering/ViewProvider.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ResourceSet;

    class DefaultViewProvider : public Resource, public ViewProvider {
    public:
        explicit DefaultViewProvider(ResourceSet *resources);
        ~DefaultViewProvider() override = default;

        [[nodiscard]] std::optional<View> tryGetViewFor(const Entity &entity) override;

    private:
        Lazy<ECSManager> _ecsManager;
    };
}

#endif // PENROSE_RENDERING_DEFAULT_VIEW_PROVIDER_HPP
