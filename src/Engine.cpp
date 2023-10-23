#include <Penrose/Engine.hpp>

#include <chrono>
#include <ranges>

#include <Penrose/Assets/AssetDictionary.hpp>
#include <Penrose/Assets/AssetLoader.hpp>
#include <Penrose/Assets/AssetManager.hpp>
#include <Penrose/Common/Log.hpp>
#include <Penrose/Common/StdOutLogSink.hpp>
#include <Penrose/ECS/ECSManager.hpp>
#include <Penrose/Events/EngineEvent.hpp>
#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Events/SurfaceEvent.hpp>
#include <Penrose/Input/InputHandler.hpp>
#include <Penrose/Rendering/RenderGraphContext.hpp>
#include <Penrose/Rendering/RenderListBuilder.hpp>
#include <Penrose/Rendering/RenderManager.hpp>
#include <Penrose/Rendering/SurfaceManager.hpp>
#include <Penrose/Scene/SceneManager.hpp>

#include <Penrose/Builtin/Debug/Debug.hpp>
#include <Penrose/Builtin/Glfw.hpp>
#include <Penrose/Builtin/ImGui.hpp>
#include <Penrose/Builtin/Vulkan.hpp>

#include <Penrose/Builtin/Penrose/ECS/MeshRendererComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/OrthographicCameraComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/PerspectiveCameraComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/TransformComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/ViewComponent.hpp>
#include <Penrose/Builtin/Penrose/Rendering/ForwardSceneDrawRenderOperator.hpp>

#include "src/Rendering/DefaultDrawableProvider.hpp"
#include "src/Rendering/DefaultViewProvider.hpp"

namespace Penrose {

    Engine::Engine() {

        this->_resources.add<Log>().done();
        this->_resources.add<StdOutLogSink>()
                .implements<LogSink>()
                .done();
        this->_resources.add<EventQueue>()
                .implements<Initializable>()
                .implements<Updatable>()
                .done();

        this->_resources.add<ECSManager>()
                .implements<Initializable>()
                .implements<Updatable>()
                .done();
        this->_resources.add<SceneManager>()
                .implements<Initializable>()
                .done();

        this->_resources.add<SurfaceManager>()
                .implements<Initializable>()
                .done();
        this->_resources.add<InputHandler>().done();

        this->_resources.add<RenderGraphContext>().done();
        this->_resources.add<RenderListBuilder>()
                .implements<Initializable>()
                .done();
        this->_resources.add<RenderManager>()
                .implements<Initializable>()
                .implements<Runnable>()
                .done();
        this->_resources.add<DefaultDrawableProvider>()
                .implements<DrawableProvider>()
                .done();
        this->_resources.add<DefaultViewProvider>()
                .implements<ViewProvider>()
                .done();

        this->_resources.add<AssetDictionary>().done();
        this->_resources.add<AssetLoader>().done();
        this->_resources.add<AssetManager>()
                .implements<Initializable>()
                .implements<Runnable>()
                .done();

        // backends
        addGlfw(this->_resources);
        addVulkan(this->_resources);
        addImGui(this->_resources);
        addDebug(this->_resources);

        // builtin
        this->_resources.add<MeshRendererComponentFactory>()
                .implements<ComponentFactory>()
                .done();
        this->_resources.add<OrthographicCameraComponentFactory>()
                .implements<ComponentFactory>()
                .done();
        this->_resources.add<PerspectiveCameraComponentFactory>()
                .implements<ComponentFactory>()
                .done();
        this->_resources.add<TransformComponentFactory>()
                .implements<ComponentFactory>()
                .done();
        this->_resources.add<ViewComponentFactory>()
                .implements<ComponentFactory>()
                .done();

        // builtin / rendering operators
        this->_resources.add<ForwardSceneDrawRenderOperator>()
                .implements<Initializable>()
                .implements<RenderOperator>()
                .done();
    }

    void Engine::run() {
        auto allInitializable = this->_resources.get<Initializable>();
        auto allRunnable = this->_resources.get<Runnable>();
        auto allUpdatable = this->_resources.get<Updatable>();

        for (auto &initializable: allInitializable) {
            initializable->init();
        }

        auto eventQueue = this->_resources.get<EventQueue>();

        auto alive = true;
        auto handlerIdx = eventQueue->addHandler(
                EventType::EngineEvent | EventType::SurfaceEvent,
                [&alive](const EventBase *event) {
                    switch (event->getType()) {
                        case EventType::EngineEvent: {
                            auto engineEvent = dynamic_cast<const EngineEvent *>(event);

                            if (engineEvent->getArgs().type ==
                                EngineEventType::DestroyRequested) {
                                alive = false;
                            }

                            break;
                        }

                        case EventType::SurfaceEvent: {
                            auto surfaceEvent = dynamic_cast<const SurfaceEvent *>(event);

                            if (surfaceEvent->getArgs().type ==
                                SurfaceEventType::CloseRequested) {
                                alive = false;
                            }

                            break;
                        }

                        default:
                            throw EngineError("Not supported event");
                    }
                });

        for (auto &runnable: allRunnable) {
            runnable->run();
        }

        std::chrono::high_resolution_clock::time_point start;
        float delta = 0;

        while (alive) {
            start = std::chrono::high_resolution_clock::now();

            for (auto &updatable: allUpdatable) {
                updatable->update(delta);
            }

            delta = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - start).count();
        }

        for (auto &runnable: std::views::reverse(allRunnable)) {
            runnable->stop();
        }

        eventQueue->removeHandler(handlerIdx);

        for (auto &initializable: std::views::reverse(allInitializable)) {
            initializable->destroy();
        }
    }
}
