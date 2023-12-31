#include <Penrose/Rendering/SurfaceManager.hpp>

namespace Penrose {

    SurfaceManager::SurfaceManager(const ResourceSet *resources)
            : _surfaceFactory(resources->get<SurfaceFactory>()),
              _surfaceHooks(resources->get<SurfaceHook>()) {
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
