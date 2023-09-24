#include <Penrose/Core/Engine.hpp>

#include <chrono>

#include <Penrose/Assets/AssetDictionary.hpp>
#include <Penrose/Assets/AssetLoader.hpp>
#include <Penrose/Assets/AssetManager.hpp>
#include <Penrose/Common/Vertex.hpp>
#include <Penrose/Core/Log.hpp>
#include <Penrose/ECS/ECSManager.hpp>
#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Rendering/RenderGraphContext.hpp>
#include <Penrose/Rendering/RenderManager.hpp>
#include <Penrose/Rendering/SurfaceManager.hpp>
#include <Penrose/Scene/SceneManager.hpp>

#include <Penrose/Builtin/Debug/Debug.hpp>
#include <Penrose/Builtin/Glfw.hpp>
#include <Penrose/Builtin/ImGui.hpp>
#include <Penrose/Builtin/Vulkan.hpp>

#include <Penrose/Builtin/Penrose/ECS/CameraComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/MeshRendererComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/TransformComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/ViewComponent.hpp>
#include <Penrose/Builtin/Penrose/Rendering/ForwardSceneDrawRenderOperator.hpp>

#include "src/Rendering/RenderData.hpp"
#include "src/Rendering/RenderListBuilder.hpp"

#include "src/Builtin/Vulkan/Rendering/VkPipelineFactory.hpp"

namespace Penrose {

    Engine::Engine() {
        // assets
        this->_resources.add<AssetDictionary>();
        this->_resources.add<AssetLoader>();
        this->_resources.add<EventQueue>();
        this->_resources.add<Log>();
        this->_resources.add<RenderGraphContext>();
        this->_resources.add<SurfaceManager>();

        // backends
        addVulkan(this->_resources);
        addGlfw(this->_resources);
        addImGui(this->_resources);
        addDebug(this->_resources);

        this->_resources.add<AssetManager>();
        this->_resources.add<ECSManager>();
        this->_resources.add<RenderManager>();
        this->_resources.add<SceneManager>();

        // rendering
        this->_resources.add<RenderListBuilder>();

        // builtin / ECS
        this->_resources.add<CameraComponentFactory, ComponentFactory>();
        this->_resources.add<MeshRendererComponentFactory, ComponentFactory>();
        this->_resources.add<TransformComponentFactory, ComponentFactory>();
        this->_resources.add<ViewComponentFactory, ComponentFactory>();

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

            // TODO: use Updatable
            eventQueue->process();

            this->_resources.updateAll(delta);

            delta = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - start).count();
        }

        this->_resources.stopAll();

        eventQueue->removeHandler(handlerIdx);

        this->_resources.destroyAll();
    }
}
