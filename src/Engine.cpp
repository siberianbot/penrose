#include <Penrose/Engine.hpp>

#include <chrono>
#include <ranges>

#include <Penrose/Assets/AssetDictionary.hpp>
#include <Penrose/Assets/AssetLoader.hpp>
#include <Penrose/Assets/AssetManager.hpp>
#include <Penrose/Common/Log.hpp>
#include <Penrose/Common/StdOutLogSink.hpp>
#include <Penrose/ECS/EntityManager.hpp>
#include <Penrose/ECS/SystemManager.hpp>
#include <Penrose/Events/ECSEvents.hpp>
#include <Penrose/Events/EngineEvents.hpp>
#include <Penrose/Events/InputEvents.hpp>
#include <Penrose/Events/SurfaceEvents.hpp>
#include <Penrose/Input/InputHandler.hpp>
#include <Penrose/Performance/Profiler.hpp>
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

        this->_resources.add<InputHandler, ResourceGroup::Engine>().done();
        this->_resources.add<Log, ResourceGroup::Engine>().done();
        this->_resources.add<StdOutLogSink, ResourceGroup::Engine>()
                .implements<LogSink>()
                .done();

        this->_resources.add<Profiler, ResourceGroup::Performance>().done();

        this->_resources.add<SurfaceManager, ResourceGroup::Windowing>()
                .implements<Initializable>()
                .done();

        this->_resources.add<RenderGraphContext, ResourceGroup::Rendering>().done();
        this->_resources.add<RenderListBuilder, ResourceGroup::Rendering>()
                .implements<Initializable>()
                .done();
        this->_resources.add<RenderManager, ResourceGroup::Rendering>()
                .implements<Initializable>()
                .implements<Runnable>()
                .done();
        this->_resources.add<DefaultDrawableProvider, ResourceGroup::Rendering>()
                .implements<DrawableProvider>()
                .done();
        this->_resources.add<DefaultViewProvider, ResourceGroup::Rendering>()
                .implements<ViewProvider>()
                .done();

        this->_resources.add<AssetDictionary, ResourceGroup::Assets>().done();
        this->_resources.add<AssetLoader, ResourceGroup::Assets>().done();
        this->_resources.add<AssetManager, ResourceGroup::Assets>()
                .implements<Initializable>()
                .implements<Runnable>()
                .done();

        this->_resources.add<ECSEventQueue, ResourceGroup::Events>()
                .implements<Initializable>()
                .implements<Updatable>()
                .done();
        this->_resources.add<EngineEventQueue, ResourceGroup::Events>()
                .implements<Initializable>()
                .implements<Updatable>()
                .done();
        this->_resources.add<InputEventQueue, ResourceGroup::Events>()
                .implements<Initializable>()
                .implements<Updatable>()
                .done();
        this->_resources.add<SurfaceEventQueue, ResourceGroup::Events>()
                .implements<Initializable>()
                .implements<Updatable>()
                .done();

        this->_resources.add<EntityManager, ResourceGroup::ECSManager>()
                .implements<Initializable>()
                .done();
        this->_resources.add<SystemManager, ResourceGroup::ECSManager>()
                .implements<Updatable>()
                .done();

        this->_resources.add<SceneManager, ResourceGroup::Scene>()
                .implements<Initializable>()
                .done();

        // backends
        addGlfw(this->_resources);
        addVulkan(this->_resources);
        addImGui(this->_resources);
        addDebug(this->_resources);

        // builtin
        this->_resources.add<MeshRendererComponentFactory, ResourceGroup::ECSComponent>()
                .implements<ComponentFactory>()
                .done();
        this->_resources.add<OrthographicCameraComponentFactory, ResourceGroup::ECSComponent>()
                .implements<ComponentFactory>()
                .done();
        this->_resources.add<PerspectiveCameraComponentFactory, ResourceGroup::ECSComponent>()
                .implements<ComponentFactory>()
                .done();
        this->_resources.add<TransformComponentFactory, ResourceGroup::ECSComponent>()
                .implements<ComponentFactory>()
                .done();
        this->_resources.add<ViewComponentFactory, ResourceGroup::ECSComponent>()
                .implements<ComponentFactory>()
                .done();

        // builtin / rendering operators
        this->_resources.add<ForwardSceneDrawRenderOperator, ResourceGroup::RenderOperator>()
                .implements<Initializable>()
                .implements<RenderOperator>()
                .done();
    }

    void Engine::run() {
        auto allInitializable = this->_resources.get<Initializable>();
        auto allRunnable = this->_resources.get<Runnable>();
        auto allUpdatable = this->_resources.get<Updatable>();
        auto profiler = this->_resources.get<Profiler>();

        for (auto &initializable: allInitializable) {
            initializable->init();
        }

        auto alive = true;

        auto engineEventQueue = this->_resources.get<EngineEventQueue>();
        engineEventQueue->addHandler<EngineDestroyRequestedEvent>(
                [&alive](const EngineDestroyRequestedEvent *) {
                    alive = false;
                });

        this->_resources.get<SurfaceEventQueue>()->addHandler<SurfaceCloseRequestedEvent>(
                [&engineEventQueue](const SurfaceCloseRequestedEvent *) {
                    engineEventQueue->push<EngineDestroyRequestedEvent>();
                });

        for (auto &runnable: allRunnable) {
            runnable->run();
        }

        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        float delta;

        while (alive) {
            auto frameUpdate = profiler->begin("Frame Update");

            delta = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - start).count();
            start = std::chrono::high_resolution_clock::now();

            for (auto &updatable: allUpdatable) {
                updatable->update(delta);
            }
        }

        for (auto &runnable: std::views::reverse(allRunnable)) {
            runnable->stop();
        }

        for (auto &initializable: std::views::reverse(allInitializable)) {
            initializable->destroy();
        }
    }
}
