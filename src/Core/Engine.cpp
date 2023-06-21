#include "Engine.hpp"

#include "src/Assets/AssetDictionary.hpp"
#include "src/Assets/AssetManager.hpp"
#include "src/Backends/GlfwBackend.hpp"
#include "src/Backends/ImGuiBackend.hpp"
#include "src/Backends/VulkanBackend.hpp"
#include "src/ECS/ECSManager.hpp"
#include "src/ECS/Components/MeshRendererComponent.hpp"
#include "src/ECS/Components/TransformComponent.hpp"
#include "src/Events/EventQueue.hpp"
#include "src/Rendering/DeviceContext.hpp"
#include "src/Rendering/PresentContext.hpp"
#include "src/Rendering/RenderContext.hpp"
#include "src/Rendering/RenderGraphExecutor.hpp"
#include "src/Rendering/RenderListProvider.hpp"
#include "src/Rendering/RenderThread.hpp"
#include "src/Rendering/Surface.hpp"
#include "src/Rendering/Operators/ForwardSceneDrawRenderOperator.hpp"
#include "src/Scene/SceneManager.hpp"

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
        this->_resources.add<RenderListProvider>();

        // asset
        this->_resources.add<AssetManager>();

        // rendering
        auto renderContext = this->_resources.add<RenderContext>();
        this->_resources.add<RenderGraphExecutor>();
        this->_resources.add<RenderThread>();

        // components
        ecsManager->registerComponent<MeshRendererComponent>();
        ecsManager->registerComponent<TransformComponent>();

        // rendering operators
        renderContext->registerRenderOperator<Penrose::ForwardSceneDrawRenderOperator>();
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
