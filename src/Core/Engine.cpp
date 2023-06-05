#include "Engine.hpp"

#include "src/Backends/GlfwBackend.hpp"
#include "src/Backends/ImGuiBackend.hpp"
#include "src/Backends/VulkanBackend.hpp"
#include "src/Events/EventQueue.hpp"
#include "src/ECS/EntityComponentManager.hpp"
#include "src/Rendering/DeviceContext.hpp"
#include "src/Rendering/DeviceMemoryAllocator.hpp"
#include "src/Rendering/PresentContext.hpp"
#include "src/Rendering/RenderContext.hpp"
#include "src/Rendering/RenderGraphExecutor.hpp"
#include "src/Rendering/RenderThread.hpp"
#include "src/Rendering/Surface.hpp"
#include "src/Rendering/Operators/ImGuiDrawRenderOperator.hpp"

namespace Penrose {

    Engine::Engine() {
        // core
        this->_resources.add<EventQueue>();
        this->_resources.add<EntityComponentManager>();

        // backends
        this->_resources.add<GlfwBackend>();
        this->_resources.add<ImGuiBackend>();
        this->_resources.add<VulkanBackend>();

        // rendering
        this->_resources.add<Surface>();
        this->_resources.add<DeviceContext>();
        this->_resources.add<DeviceMemoryAllocator>();
        this->_resources.add<PresentContext>();
        this->_resources.add<RenderContext>();
        this->_resources.add<RenderGraphExecutor>();
        this->_resources.add<RenderThread>();

        auto renderContext = this->_resources.get<RenderContext>();
        renderContext->addRenderOperatorProducer<ImGuiDrawRenderOperatorProducer>(IMGUI_DRAW_RENDER_OPERATOR_NAME);
    }

    void Engine::run() {
        this->_resources.initAll();

        auto eventQueue = this->_resources.get<EventQueue>();
        auto surface = this->_resources.get<Surface>();

        auto alive = true;

        eventQueue->addHandler([&alive](const Event &event) {
            if (event.type != EventType::EngineDestroyRequested) {
                return;
            }

            alive = false;
        });

        while (alive) {
            eventQueue->process();
            surface->poll();
        }

        this->_resources.destroyAll();
    }
}
