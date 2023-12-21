#include "RenderManagerImpl.hpp"

#include <Penrose/Common/EngineError.hpp>

namespace Penrose {

    inline static constexpr std::string_view TAG = "RenderManagerImpl";

    inline constexpr int INVALIDATE_JOB_ORDER = -1;
    inline constexpr int RENDER_JOB_ORDER = 0;

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

        this->invalidate();

        this->_jobQueue.enqueue(
            [this] {
                try {
                    this->render();
                } catch (const std::exception &error) {
                    this->_log->writeError(TAG, "Rendering error: {}", error.what());
                }
            },
            JobQueue::Params {
                .order = RENDER_JOB_ORDER,
                .remove = false,
                .override = false,
            }
        );

        this->_jobQueue.start();

        this->_surfaceEventQueue->addHandler<SurfaceResizedEvent>([this](const SurfaceResizedEvent *) {
            this->invalidate();
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

    void RenderManagerImpl::invalidate() {
        this->_jobQueue.enqueue(
            [this] {
                try {
                    this->_renderContext->get()->invalidate();
                } catch (const std::exception &error) {
                    this->_log->writeError("SurfaceResizeJob", "Caught error: {}", error.what());
                }
            },
            JobQueue::Params {
                .order = INVALIDATE_JOB_ORDER,
                .remove = true,
                .override = true,
            }
        );
    }
}
