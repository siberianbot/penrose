#include <Penrose/Rendering/SurfaceManager.hpp>

#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    SurfaceManager::SurfaceManager(ResourceSet *resources)
            : _surfaceFactory(resources->getLazy<SurfaceFactory>()) {
        //
    }

    void SurfaceManager::init() {
        auto surface = this->_surfaceFactory->makeSurface();

        this->_surface = std::unique_ptr<Surface>(surface);
    }

    void SurfaceManager::destroy() {
        this->_surface = std::nullopt;
    }
}
