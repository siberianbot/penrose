#include "RenderManagerImpl.hpp"

#include <Penrose/Common/EngineError.hpp>

namespace Penrose {

    inline static constexpr std::string_view TAG = "RenderManagerImpl";

    RenderManagerImpl::RenderManagerImpl(const ResourceSet *resources)
        : _resources(resources),
          _log(resources->get<Log>()),
          _surfaceEventQueue(resources->get<SurfaceEventQueue>()),
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

        this->_renderContext = std::unique_ptr<RenderContext>((*this->_renderSystem)->makeRenderContext());

        this->_jobQueue.enqueue<SurfaceResizeJob>(this->_log.get(), this->_renderContext->get());
        this->_jobQueue.enqueue<FrameRenderJob>(this->_log.get(), this);
        this->_jobQueue.start();

        this->_surfaceEventQueue->addHandler<SurfaceResizedEvent>([this](const SurfaceResizedEvent *) {
            this->_jobQueue.enqueue<SurfaceResizeJob>(this->_log.get(), this->_renderContext->get());
        });

        this->_initialized = true;
    }

    void RenderManagerImpl::destroy() {
        if (!this->_initialized) {
            return;
        }

        this->_log->writeInfo(TAG, "Deinitializing render manager");

        this->_jobQueue.stop();
        this->_jobQueue.clear();

        this->_renderContext = std::nullopt;

        for (const auto &renderer: this->_renderers | std::views::values) {
            try {
                renderer->destroy();
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

        const auto renderer = this->_resources->resolveOne<Renderer>(std::forward<decltype(type)>(type));

        if (this->_renderers.contains(renderer->getName())) {
            throw EngineError("Renderer {} already added into rendering manager", renderer->getName());
        }

        this->_renderers.emplace(renderer->getName(), renderer);
    }

    RenderManagerImpl::FrameRenderJob::FrameRenderJob(Log *log, RenderManagerImpl *renderManager)
        : _log(log),
          _renderManager(renderManager) {
        //
    }

    void RenderManagerImpl::FrameRenderJob::exec() {
        try {
            this->_renderManager->render();
        } catch (const std::exception &error) {
            this->_log->writeError("FrameRenderJob", "Caught error: {}", error.what());
        }
    }

    RenderManagerImpl::SurfaceResizeJob::SurfaceResizeJob(Log *log, RenderContext *renderContext)
        : _log(log),
          _renderContext(renderContext) {
        //
    }

    void RenderManagerImpl::SurfaceResizeJob::exec() {
        try {
            this->_renderContext->invalidate();
        } catch (const std::exception &error) {
            this->_log->writeError("SurfaceResizeJob", "Caught error: {}", error.what());
        }
    }

    void RenderManagerImpl::render() {
        const auto renderContext = this->_renderContext->get();

        if (!renderContext->beginRender()) {
            return;
        }

        for (const auto &[name, params]: this->_executionInfo.renderers) {
            const auto it = this->_renderers.find(name);

            if (it == this->_renderers.end()) {
                this->_log->writeError(TAG, "Render execution info contains invocation of unknown renderer {}", name);

                continue;
            }

            {
                const auto rendererContext = std::unique_ptr<RendererContext>(renderContext->makeRendererContext());

                it->second->execute(rendererContext.get(), params);
            }
        }

        renderContext->submitRender();
    }
}
