#ifndef PENROSE_RENDERING_SURFACE_MANAGER_HPP
#define PENROSE_RENDERING_SURFACE_MANAGER_HPP

#include <optional>
#include <memory>

#include <Penrose/Rendering/SurfaceFactory.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ResourceSet;

    class SurfaceManager : public Resource, public Initializable {
    public:
        explicit SurfaceManager(ResourceSet *resources);
        ~SurfaceManager() override = default;

        void init() override;
        void destroy() override;

        [[nodiscard]] Surface *getSurface() const { return this->_surface.value().get(); }

    private:
        Lazy<SurfaceFactory> _surfaceFactory;

        std::optional<std::unique_ptr<Surface>> _surface;
    };
}

#endif // PENROSE_RENDERING_SURFACE_MANAGER_HPP
