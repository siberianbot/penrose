#ifndef PENROSE_RENDERING_SURFACE_MANAGER_HPP
#define PENROSE_RENDERING_SURFACE_MANAGER_HPP

#include <optional>
#include <memory>

#include <Penrose/Api.hpp>
#include <Penrose/Rendering/SurfaceFactory.hpp>
#include <Penrose/Rendering/SurfaceHook.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    class ResourceSet;

    class PENROSE_API SurfaceManager : public Resource<SurfaceManager, ResourceGroup::Windowing>,
                                       public Initializable {
    public:
        explicit SurfaceManager(ResourceSet *resources);
        ~SurfaceManager() override = default;

        void init() override;
        void destroy() override;

        void invalidate();

        [[nodiscard]] std::shared_ptr<Surface> getSurface() const { return this->_surface.value(); }

    private:
        ResourceProxy<SurfaceFactory> _surfaceFactory;
        ResourceProxy<SurfaceHook> _surfaceHooks;

        std::optional<std::shared_ptr<Surface>> _surface;
    };
}

#endif // PENROSE_RENDERING_SURFACE_MANAGER_HPP
