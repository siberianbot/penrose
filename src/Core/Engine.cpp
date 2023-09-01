#include <Penrose/Core/Engine.hpp>

#include <chrono>

#include <Penrose/Assets/AssetDictionary.hpp>
#include <Penrose/Assets/AssetLoader.hpp>
#include <Penrose/Assets/AssetManager.hpp>
#include <Penrose/Common/Vertex.hpp>
#include <Penrose/Core/Log.hpp>
#include <Penrose/ECS/ECSManager.hpp>
#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Rendering/RenderContext.hpp>
#include <Penrose/Scene/SceneManager.hpp>

#include <Penrose/Builtin/ECS/CameraComponent.hpp>
#include <Penrose/Builtin/ECS/MeshRendererComponent.hpp>
#include <Penrose/Builtin/ECS/TransformComponent.hpp>
#include <Penrose/Builtin/ECS/ViewComponent.hpp>
#include <Penrose/Builtin/Rendering/ForwardSceneDrawRenderOperator.hpp>

#include "src/Rendering/DeviceContext.hpp"
#include "src/Rendering/PresentContext.hpp"
#include "src/Rendering/RenderData.hpp"
#include "src/Rendering/RenderGraphExecutorProvider.hpp"
#include "src/Rendering/RenderListBuilder.hpp"
#include "src/Rendering/RenderManager.hpp"
#include "src/Rendering/Surface.hpp"

#include "src/Builtin/Backends/GlfwBackend.hpp"
#include "src/Builtin/Backends/ImGuiBackend.hpp"
#include "src/Builtin/Vulkan/VulkanBackend.hpp"

#include "src/Builtin/Vulkan/Rendering/VkPipelineFactory.hpp"

namespace Penrose {

    Engine::Engine() {
        // core
        this->_resources.add<Log>();

        // backends
        this->_resources.add<GlfwBackend, VkInstanceExtensionsProvider>();
        this->_resources.add<ImGuiBackend>();

        addVulkan(this->_resources);

        // core
        this->_resources.add<EventQueue>();
        this->_resources.add<AssetDictionary>();
        auto ecsManager = this->_resources.add<ECSManager>();
        this->_resources.add<SceneManager>();

        // rendering / core
        this->_resources.add<Surface>(this->_resources.tryGetIteratorOf<DeviceContext>().value());

        // asset
        this->_resources.add<AssetLoader>();
        this->_resources.add<AssetManager>();

        // rendering
        this->_resources.add<RenderContext>();
        this->_resources.add<RenderGraphExecutorProvider>();
        this->_resources.add<RenderListBuilder>();
        this->_resources.add<RenderManager>();

        // builtin / ECS
        ecsManager->registerComponent<CameraComponent>();
        ecsManager->registerComponent<MeshRendererComponent>();
        ecsManager->registerComponent<TransformComponent>();
        ecsManager->registerComponent<ViewComponent>();

        // builtin / rendering operators
        this->_resources.add<ForwardSceneDrawRenderOperator, RenderOperator>();

        // TODO:
        auto defaultPipelineInfo = PipelineInfo()
                .setLayout(
                        PipelineLayout()
                                .addConstant(
                                        PipelineLayoutConstant(PipelineShaderStageType::Vertex, 0, sizeof(RenderData))
                                )
                                .addBinding(
                                        PipelineLayoutBinding(PipelineShaderStageType::Fragment,
                                                              PipelineLayoutBindingType::Sampler,
                                                              1)
                                )
                )
                .addStage(
                        PipelineShaderStage(PipelineShaderStageType::Vertex,
                                            "shaders/default-forward-rendering.vert.asset")
                )
                .addStage(
                        PipelineShaderStage(PipelineShaderStageType::Fragment,
                                            "shaders/default-forward-rendering.frag.asset")
                )
                .addBinding(
                        PipelineBinding(PipelineBindingInputRate::Vertex, sizeof(Vertex))
                                .addAttribute(PipelineBindingAttribute(PipelineBindingAttributeFormat::Vec3,
                                                                       offsetof(Vertex, pos)))
                                .addAttribute(PipelineBindingAttribute(PipelineBindingAttributeFormat::Vec3,
                                                                       offsetof(Vertex, normal)))
                                .addAttribute(PipelineBindingAttribute(PipelineBindingAttributeFormat::Vec3,
                                                                       offsetof(Vertex, color)))
                                .addAttribute(PipelineBindingAttribute(PipelineBindingAttributeFormat::Vec2,
                                                                       offsetof(Vertex, uv)))
                );

        this->_resources.get<PipelineFactory>()->addPipeline("Default", defaultPipelineInfo);
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

        this->_resources.runAll();

        std::chrono::high_resolution_clock::time_point start;
        float delta = 0;

        while (alive) {
            start = std::chrono::high_resolution_clock::now();

            eventQueue->process();
            ecsManager->updateSystems(delta);
            surface->poll();

            delta = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - start).count();
        }

        this->_resources.stopAll();

        eventQueue->removeHandler(handlerIdx);

        this->_resources.destroyAll();
    }
}
