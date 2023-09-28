#include <Penrose/Core/Engine.hpp>

#include <chrono>

#include <Penrose/Assets/AssetDictionary.hpp>
#include <Penrose/Assets/AssetLoader.hpp>
#include <Penrose/Assets/AssetManager.hpp>
#include <Penrose/Core/Log.hpp>
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
        // before backends
        this->_resources.add<AssetDictionary>();
        this->_resources.add<AssetLoader>();
        this->_resources.add<EventQueue>();
        this->_resources.add<InputHandler>();
        this->_resources.add<Log>();
        this->_resources.add<RenderGraphContext>();
        this->_resources.add<SurfaceManager>();

        // backends
        addVulkan(this->_resources);
        addGlfw(this->_resources);
        addImGui(this->_resources);
        addDebug(this->_resources);

        // after backends
        this->_resources.add<AssetManager>();
        this->_resources.add<ECSManager>();
        this->_resources.add<RenderListBuilder>();
        this->_resources.add<RenderManager>();
        this->_resources.add<SceneManager>();
        this->_resources.add<DefaultDrawableProvider, DrawableProvider>();
        this->_resources.add<DefaultViewProvider, ViewProvider>();

        // builtin / ECS
        this->_resources.add<MeshRendererComponentFactory, ComponentFactory>();
        this->_resources.add<OrthographicCameraComponentFactory, ComponentFactory>();
        this->_resources.add<PerspectiveCameraComponentFactory, ComponentFactory>();
        this->_resources.add<TransformComponentFactory, ComponentFactory>();
        this->_resources.add<ViewComponentFactory, ComponentFactory>();

        // builtin / rendering operators
        this->_resources.add<ForwardSceneDrawRenderOperator, RenderOperator>();
    }

    void Engine::run() {
        this->_resources.initAll();

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

        this->_resources.runAll();

        std::chrono::high_resolution_clock::time_point start;
        float delta = 0;

        while (alive) {
            start = std::chrono::high_resolution_clock::now();

            this->_resources.updateAll(delta);

            delta = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - start).count();
        }

        this->_resources.stopAll();

        eventQueue->removeHandler(handlerIdx);

        this->_resources.destroyAll();
    }
}
