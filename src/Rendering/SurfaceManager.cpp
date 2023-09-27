#include <Penrose/Rendering/SurfaceManager.hpp>

#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    SurfaceManager::SurfaceManager(ResourceSet *resources)
            : _surfaceFactory(resources->getLazy<SurfaceFactory>()),
              _surfaceHooks(resources->getAllLazy<SurfaceHook>()) {
        //
    }

    void SurfaceManager::init() {
        auto surface = this->_surfaceFactory->makeSurface();

        for (const auto &hook: this->_surfaceHooks) {
            hook->onSurfaceCreate(surface);
        }

        this->_surface = std::shared_ptr<Surface>(surface);
    }

    void SurfaceManager::destroy() {
        if (!this->_surface.has_value()) {
            return;
        }

        for (const auto &hook: this->_surfaceHooks) {
            hook->onSurfaceDestroy(this->_surface->get());
        }

        this->_surface = std::nullopt;
    }

    void SurfaceManager::invalidate() {
        if (!this->_surface.has_value()) {
            return;
        }

        for (const auto &hook: this->_surfaceHooks) {
            hook->onSurfaceInvalidated(this->_surface->get());
        }
    }
}
