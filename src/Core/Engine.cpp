#include <Penrose/Core/Engine.hpp>

#include <Penrose/Assets/AssetDictionary.hpp>
#include <Penrose/Assets/AssetManager.hpp>
#include <Penrose/ECS/ECSManager.hpp>
#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Rendering/RenderContext.hpp>
#include <Penrose/Scene/SceneManager.hpp>

#include <Penrose/Builtin/ECS/CameraComponent.hpp>
#include <Penrose/Builtin/ECS/MeshRendererComponent.hpp>
#include <Penrose/Builtin/ECS/RenderListProviderComponent.hpp>
#include <Penrose/Builtin/ECS/TransformComponent.hpp>
#include <Penrose/Builtin/Rendering/ForwardSceneDrawRenderOperator.hpp>

#include "src/Backends/GlfwBackend.hpp"
#include "src/Backends/ImGuiBackend.hpp"
#include "src/Backends/VulkanBackend.hpp"
#include "src/Rendering/DeviceContext.hpp"
#include "src/Rendering/PresentContext.hpp"
#include "src/Rendering/RenderGraphExecutor.hpp"
#include "src/Rendering/RenderThread.hpp"
#include "src/Rendering/Surface.hpp"

#include "src/Builtin/ECS/RenderListBuilderSystem.hpp"

namespace Penrose {

    Engine::Engine() {
        // core
        this->_resources.add<EventQueue>();
        this->_resources.add<AssetDictionary>();
        auto ecsManager = this->_resources.add<ECSManager>();
        this->_resources.add<SceneManager>();

        // backends
        this->_resources.add<GlfwBackend>();
        this->_resources.add<ImGuiBackend>();
        this->_resources.add<VulkanBackend>();

        // rendering / core
        this->_resources.add<Surface>();
        this->_resources.add<DeviceContext>();
        this->_resources.add<PresentContext>();

        // asset
        this->_resources.add<AssetManager>();

        // rendering
        auto renderContext = this->_resources.add<RenderContext>();
        this->_resources.add<RenderGraphExecutor>();
        this->_resources.add<RenderThread>();

        // components
        ecsManager->registerComponent<CameraComponent>();
        ecsManager->registerComponent<MeshRendererComponent>();
        ecsManager->registerComponent<RenderListProviderComponent>();
        ecsManager->registerComponent<TransformComponent>();

        // rendering operators
        renderContext->registerRenderOperator<ForwardSceneDrawRenderOperator>();

        // ECS systems
        ecsManager->registerSystem<RenderListBuilderSystem>();
    }

    void Engine::run() {
        this->_resources.initAll();

        auto eventQueue = this->_resources.get<EventQueue>();
        auto ecsManager = this->_resources.get<ECSManager>();
        auto surface = this->_resources.get<Surface>();

        auto alive = true;
        auto handlerIdx = eventQueue->addHandler([&alive](const Event &event) {
            if (event.type != EventType::EngineDestroyRequested) {
                return;
            }

            alive = false;
        });

        while (alive) {
            eventQueue->process();
            ecsManager->update();
            surface->poll();
        }

        eventQueue->removeHandler(handlerIdx);

        this->_resources.destroyAll();
    }
}
