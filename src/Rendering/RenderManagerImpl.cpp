#include "RenderManagerImpl.hpp"

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Utils/TypeUtils.hpp>

namespace Penrose {

    inline static constexpr std::string_view TAG = "RenderManagerImpl";

    RenderManagerImpl::RenderManagerImpl(const ResourceSet *resources)
        : _resources(resources),
          _log(resources->get<Log>()),
          _initialized(false) {
        //
    }

    void RenderManagerImpl::init() {
        if (this->_initialized) {
            return;
        }

        this->_log->writeInfo(TAG, "Initializing render manager");

        if (!this->_renderSystem.has_value()) {
            this->_renderSystem = this->_resources->resolveOne<RenderSystem>();
        }

        try {
            (*this->_renderSystem)->init();
        } catch (...) {
            std::throw_with_nested(
                EngineError("Failed to initialize rendering system {}", (*this->_renderSystem)->getName())
            );
        }

        for (const auto &renderer: this->_renderers | std::views::values) {
            try {
                renderer->init();
            } catch (...) {
                std::throw_with_nested(EngineError("Failed to initialize renderer {}", renderer->getName()));
            }
        }

        this->_jobQueue.enqueue<FrameRenderJob>(*this->_renderSystem);
        this->_jobQueue.start();

        this->_initialized = true;
    }

    void RenderManagerImpl::destroy() {
        if (!this->_initialized) {
            return;
        }

        this->_log->writeInfo(TAG, "Deinitializing render manager");

        this->_jobQueue.stop();
        this->_jobQueue.clear();

        for (const auto &renderer: this->_renderers | std::views::values) {
            try {
                renderer->init();
            } catch (const std::exception &error) {
                this->_log->writeError(
                    TAG, "Failed to deinitialize renderer {}: {}", renderer->getName(), error.what()
                );
            }
        }

        try {
            (*this->_renderSystem)->destroy();
        } catch (const std::exception &error) {
            this->_log->writeError(
                TAG, "Failed to deinitialize rendering system {}: {}", (*this->_renderSystem)->getName(), error.what()
            );
        }

        this->_initialized = false;
    }

    void RenderManagerImpl::setRenderSystem(std::type_index &&type) {
        if (this->_initialized) {
            throw EngineError("Rendering manager is initialized");
        }

        this->_renderSystem = this->_resources->resolveOne<RenderSystem>(std::forward<decltype(type)>(type));
    }

    void RenderManagerImpl::addRenderer(std::type_index &&type) {
        if (this->_initialized) {
            throw EngineError("Rendering manager is initialized");
        }

        if (this->_renderers.contains(type)) {
            throw EngineError("Renderer of type {} already added into rendering manager", getTypeName(type));
        }

        const auto renderer = this->_resources->resolveOne<Renderer>(std::type_index(type));

        this->_renderers.emplace(std::forward<decltype(type)>(type), renderer);
    }

    RenderManagerImpl::FrameRenderJob::FrameRenderJob(RenderSystem *renderSystem)
        : _renderSystem(renderSystem) {
        //
    }

    void RenderManagerImpl::FrameRenderJob::exec() {
        this->_renderSystem->render();
    }

    RenderManagerImpl::SurfaceResizeJob::SurfaceResizeJob(RenderSystem *renderSystem)
        : _renderSystem(renderSystem) {
        //
    }

    void RenderManagerImpl::SurfaceResizeJob::exec() {
        this->_renderSystem->resize();
    }
}
