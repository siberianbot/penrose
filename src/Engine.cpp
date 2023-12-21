#include <Penrose/Engine.hpp>

#include <chrono>
#include <ranges>

#include <Penrose/ECS/EntityManager.hpp>
#include <Penrose/ECS/SystemManager.hpp>
#include <Penrose/Events/ECSEvents.hpp>
#include <Penrose/Events/EngineEvents.hpp>
#include <Penrose/Events/InputEvents.hpp>
#include <Penrose/Events/SurfaceEvents.hpp>
#include <Penrose/Input/InputHandler.hpp>
#include <Penrose/Performance/Profiler.hpp>
#include <Penrose/Rendering/RenderListBuilder.hpp>
#include <Penrose/Rendering/RenderManager.hpp>
#include <Penrose/Rendering/SurfaceManager.hpp>
#include <Penrose/Scene/SceneManager.hpp>
#include <Penrose/UI/LayoutFactory.hpp>
#include <Penrose/UI/UIManager.hpp>

#include <Penrose/Builtin/Glfw.hpp>
#include <Penrose/Builtin/ImGui/ImGui.hpp>
#include <Penrose/Builtin/Vulkan/Vulkan.hpp>

#include <Penrose/Builtin/Penrose/ECS/MeshRendererComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/OrthographicCameraComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/PerspectiveCameraComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/TransformComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/ViewComponent.hpp>

#include "src/Assets/AssetIndex.hpp"
#include "src/Assets/AssetLoadingJobQueue.hpp"
#include "src/Assets/AssetLoadingProxy.hpp"
#include "src/Assets/AssetManagerImpl.hpp"
#include "src/Assets/Loaders/ImageLoader.hpp"
#include "src/Assets/Loaders/MeshLoader.hpp"
#include "src/Assets/Loaders/ShaderLoader.hpp"
#include "src/Assets/Loaders/UILayoutLoader.hpp"

#include "src/Common/ConsoleLogSink.hpp"
#include "src/Common/LogImpl.hpp"

#include "src/Rendering/DefaultDrawableProvider.hpp"
#include "src/Rendering/DefaultRenderer.hpp"
#include "src/Rendering/DefaultViewProvider.hpp"
#include "src/Rendering/RenderManagerImpl.hpp"

namespace Penrose {

    Engine::Engine() {

        this->_resources.add<InputHandler>().group(ResourceGroup::Engine).done();

        Log *log = this->_resources.add<LogImpl>().group(ResourceGroup::Engine).implements<Log>().done();
        this->_resources.add<ConsoleLogSink>().group(ResourceGroup::Engine).implements<LogSink>().done();

        log->addSink<ConsoleLogSink>();

        this->_resources.add<Profiler>().group(ResourceGroup::Performance).done();

        this->_resources.add<SurfaceManager>().group(ResourceGroup::Windowing).implements<Initializable>().done();

        RenderManager *renderManager = this->_resources.add<RenderManagerImpl>()
                                           .group(ResourceGroup::Rendering)
                                           .implements<Initializable>()
                                           .implements<RenderManager>()
                                           .done();

        this->_resources.add<DefaultRenderer>().group(ResourceGroup::Rendering).implements<Renderer>().done();

        renderManager->addRenderer<DefaultRenderer>();
        renderManager->setExecutionInfo(RenderExecutionInfo {
            .renderers =
                {
                            RendererExecutionInfo("Default", {}),
                            }
        });

        this->_resources.add<RenderListBuilder>().group(ResourceGroup::Rendering).implements<Initializable>().done();
        this->_resources.add<DefaultDrawableProvider>()
            .group(ResourceGroup::Rendering)
            .implements<DrawableProvider>()
            .done();
        this->_resources.add<DefaultViewProvider>().group(ResourceGroup::Rendering).implements<ViewProvider>().done();

        this->_resources.add<MeshLoader>().group(ResourceGroup::Assets).implements<TypedAssetLoader>().done();
        this->_resources.add<ImageLoader>().group(ResourceGroup::Assets).implements<TypedAssetLoader>().done();
        this->_resources.add<ShaderLoader>().group(ResourceGroup::Assets).implements<TypedAssetLoader>().done();
        this->_resources.add<UILayoutLoader>().group(ResourceGroup::Assets).implements<TypedAssetLoader>().done();
        this->_resources.add<AssetIndex>().group(ResourceGroup::Assets).done();
        this->_resources.add<AssetLoadingProxy>().group(ResourceGroup::Assets).done();
        this->_resources.add<AssetLoadingJobQueue>().group(ResourceGroup::Assets).implements<Initializable>().done();
        this->_resources.add<AssetManagerImpl>()
            .group(ResourceGroup::Assets)
            .implements<Initializable>()
            .implements<AssetManager>()
            .done();

        this->_resources.add<LayoutFactory>().group(ResourceGroup::UI).done();
        this->_resources.add<UIManager>().group(ResourceGroup::UI).done();

        this->_resources.add<ECSEventQueue>()
            .group(ResourceGroup::Events)
            .implements<Initializable>()
            .implements<Updatable>()
            .done();
        this->_resources.add<EngineEventQueue>()
            .group(ResourceGroup::Events)
            .implements<Initializable>()
            .implements<Updatable>()
            .done();
        this->_resources.add<InputEventQueue>()
            .group(ResourceGroup::Events)
            .implements<Initializable>()
            .implements<Updatable>()
            .done();
        this->_resources.add<SurfaceEventQueue>()
            .group(ResourceGroup::Events)
            .implements<Initializable>()
            .implements<Updatable>()
            .done();

        this->_resources.add<EntityManager>().group(ResourceGroup::ECSManager).implements<Initializable>().done();
        this->_resources.add<SystemManager>().group(ResourceGroup::ECSManager).implements<Updatable>().done();

        this->_resources.add<SceneManager>().group(ResourceGroup::Scene).implements<Initializable>().done();

        // backends
        addGlfw(this->_resources);
        addVulkan(this->_resources);
        addImGui(this->_resources);

        // builtin
        this->_resources.add<MeshRendererComponentFactory>()
            .group(ResourceGroup::ECSComponent)
            .implements<ComponentFactory>()
            .done();
        this->_resources.add<OrthographicCameraComponentFactory>()
            .group(ResourceGroup::ECSComponent)
            .implements<ComponentFactory>()
            .done();
        this->_resources.add<PerspectiveCameraComponentFactory>()
            .group(ResourceGroup::ECSComponent)
            .implements<ComponentFactory>()
            .done();
        this->_resources.add<TransformComponentFactory>()
            .group(ResourceGroup::ECSComponent)
            .implements<ComponentFactory>()
            .done();
        this->_resources.add<ViewComponentFactory>()
            .group(ResourceGroup::ECSComponent)
            .implements<ComponentFactory>()
            .done();
    }

    void Engine::run() {
        auto allInitializable = this->_resources.get<Initializable>();
        auto allUpdatable = this->_resources.get<Updatable>();
        auto profiler = this->_resources.get<Profiler>();

        for (auto &initializable: allInitializable) {
            initializable->init();
        }

        auto alive = true;

        auto engineEventQueue = this->_resources.get<EngineEventQueue>();
        engineEventQueue->addHandler<EngineDestroyRequestedEvent>([&alive](const EngineDestroyRequestedEvent *) {
            alive = false;
        });

        this->_resources.get<SurfaceEventQueue>()->addHandler<SurfaceCloseRequestedEvent>(
            [&engineEventQueue](const SurfaceCloseRequestedEvent *) {
                engineEventQueue->push<EngineDestroyRequestedEvent>();
            }
        );

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

        for (auto &initializable: std::views::reverse(allInitializable)) {
            initializable->destroy();
        }
    }
}
