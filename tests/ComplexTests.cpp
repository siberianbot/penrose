#include <catch2/catch.hpp>

#include <cmath>
#include <ranges>

#include <Penrose/Assets/AssetDictionary.hpp>
#include <Penrose/Assets/AssetManager.hpp>
#include <Penrose/Core/Engine.hpp>
#include <Penrose/ECS/ECSManager.hpp>
#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Rendering/RenderContext.hpp>
#include <Penrose/Rendering/RenderOperatorFactory.hpp>
#include <Penrose/Scene/SceneManager.hpp>

#include <Penrose/Builtin/ECS/CameraComponent.hpp>
#include <Penrose/Builtin/ECS/MeshRendererComponent.hpp>
#include <Penrose/Builtin/ECS/TransformComponent.hpp>
#include <Penrose/Builtin/ECS/ViewComponent.hpp>
#include <Penrose/Builtin/Rendering/ForwardSceneDrawRenderOperator.hpp>

using namespace Penrose;

TEST_CASE("Starts engine with some prebuilt scene", "[complex]") {

    class TestLogicTargetComponent : public Component {
    public:
        ~TestLogicTargetComponent() override = default;

        [[nodiscard]] constexpr static std::string_view name() { return "TestLogicTarget"; }
    };

    class TestLogicSystem : public System {
    public:
        explicit TestLogicSystem(ResourceSet *resources)
                : _ecsManager(resources->get<ECSManager>()),
                  _eventQueue(resources->get<EventQueue>()) {
            //
        }

        ~TestLogicSystem() override = default;

        void init() override {
            this->_passed = 0;
            this->_targetEntity = this->_ecsManager->queryComponents<TestLogicTargetComponent>().at(0);
        }

        void update(float delta) override {
            this->_passed += delta;

            auto transform = this->_ecsManager->getComponent<TransformComponent>(this->_targetEntity);
            transform->getRot().y = glm::radians(this->_passed * 360.0f);

            if (this->_passed > 1.0f) {
                this->_eventQueue->push(makeEvent(EventType::EngineDestroyRequested));
            }
        }

        [[nodiscard]] constexpr static std::string_view name() { return "TestLogic"; }

    private:
        ECSManager *_ecsManager;
        EventQueue *_eventQueue;

        float _passed = 0;
        Entity _targetEntity = -1;
    };

    Engine engine;

    auto assetDictionary = engine.resources().get<AssetDictionary>();
    assetDictionary->addDir("tests/data");

    auto assetManager = engine.resources().get<AssetManager>();
    assetManager->queueMeshLoading("models/cube.obj");
    assetManager->queueImageLoading("textures/texture-1024.png");
    assetManager->queueShaderLoading("shaders/default-forward-rendering.frag.spv");
    assetManager->queueShaderLoading("shaders/default-forward-rendering.vert.spv");

    auto graph = RenderGraph()
            .setTarget("swapchain", RenderTarget::makeSwapchain())
            .setTarget("depth", RenderTarget::makeImage(RenderTargetType::DepthStencil,
                                                        RenderFormat::D32Float,
                                                        std::nullopt))
            .setSubgraph("default", RenderSubgraph()
                    .addAttachment(RenderAttachment("swapchain")
                                           .setClearValue(RenderAttachmentClearValue({0, 0, 0, 1}))
                                           .setLoadOp(RenderAttachmentLoadOp::Clear)
                                           .setStoreOp(RenderAttachmentStoreOp::Store)
                                           .setInitialLayout(RenderAttachmentLayout::Undefined)
                                           .setFinalLayout(RenderAttachmentLayout::Present))
                    .addAttachment(RenderAttachment("depth")
                                           .setFormat(RenderFormat::D32Float)
                                           .setClearValue(RenderAttachmentClearValue().setDepth(1))
                                           .setLoadOp(RenderAttachmentLoadOp::Clear)
                                           .setStoreOp(RenderAttachmentStoreOp::Store)
                                           .setInitialLayout(RenderAttachmentLayout::Undefined)
                                           .setFinalLayout(RenderAttachmentLayout::DepthStencilAttachment))
                    .addPass(RenderPass()
                                     .addColorAttachmentIdx(0)
                                     .setDepthStencilAttachment(1)
                                     .setOperator(RenderPassOperator(
                                             std::string(ForwardSceneDrawRenderOperator::NAME))))
            );

    auto renderContext = engine.resources().get<RenderContext>();
    renderContext->setRenderGraph(graph);

    auto ecsManager = engine.resources().get<ECSManager>();
    ecsManager->registerComponent<TestLogicTargetComponent>();
    ecsManager->registerSystem<TestLogicSystem>();

    auto sceneManager = engine.resources().get<SceneManager>();
    auto root = sceneManager->addRoot("Default");

    for (int x: std::ranges::views::iota(-2, 3)) {
        auto entity = ecsManager->createEntity();

        auto meshRenderer = ecsManager->addComponent<MeshRendererComponent>(entity);
        meshRenderer->setMeshAsset("models/cube.obj");
        meshRenderer->setAlbedoTextureAsset("textures/texture-1024.png");

        auto transform = ecsManager->addComponent<TransformComponent>(entity);
        transform->getPos() = glm::vec3(2.5 * x, 0, 4);

        sceneManager->insertEntityNode(root, entity);
    }

    {
        auto entity = ecsManager->createEntity();

        ecsManager->addComponent<TestLogicTargetComponent>(entity);
        ecsManager->addComponent<ViewComponent>(entity);

        auto camera = ecsManager->addComponent<CameraComponent>(entity);
        camera->getFov() = glm::radians(120.0f);

        auto transform = ecsManager->addComponent<TransformComponent>(entity);
        transform->getPos() = glm::vec3(0, 0, -4);
        transform->getRot() = glm::vec3(0, glm::radians(-90.0f), 0);

        sceneManager->insertEntityNode(root, entity);
    }

    engine.run();
}

TEST_CASE("Scene with single cube and orbital camera", "[complex]") {

    class TestLogicTargetComponent : public Component {
    public:
        ~TestLogicTargetComponent() override = default;

        [[nodiscard]] constexpr static std::string_view name() { return "TestLogicTarget"; }
    };

    class TestLogicSystem : public System {
    public:
        explicit TestLogicSystem(ResourceSet *resources)
                : _ecsManager(resources->get<ECSManager>()),
                  _eventQueue(resources->get<EventQueue>()) {
            //
        }

        ~TestLogicSystem() override = default;

        void init() override {
            this->_passed = 0;
            this->_targetEntity = this->_ecsManager->queryComponents<TestLogicTargetComponent>().at(0);
        }

        void update(float delta) override {
            this->_passed += delta;

            const int r = 4;
            auto angle = glm::radians(this->_passed * 90.0f);

            auto transform = this->_ecsManager->getComponent<TransformComponent>(this->_targetEntity);
            transform->getPos() = glm::vec3(r * cos(angle), 0, r * sin(angle));
            transform->getRot().y = M_PI - angle;

            if (this->_passed > 4.0f) {
                this->_eventQueue->push(makeEvent(EventType::EngineDestroyRequested));
            }
        }

        [[nodiscard]] constexpr static std::string_view name() { return "TestLogic"; }

    private:
        ECSManager *_ecsManager;
        EventQueue *_eventQueue;

        float _passed = 0;
        Entity _targetEntity = -1;
    };

    Engine engine;

    auto assetDictionary = engine.resources().get<AssetDictionary>();
    assetDictionary->addDir("tests/data");

    auto assetManager = engine.resources().get<AssetManager>();
    assetManager->queueMeshLoading("models/cube.obj");
    assetManager->queueImageLoading("textures/texture-1024.png");
    assetManager->queueShaderLoading("shaders/default-forward-rendering.frag.spv");
    assetManager->queueShaderLoading("shaders/default-forward-rendering.vert.spv");

    auto graph = RenderGraph()
            .setTarget("swapchain", RenderTarget::makeSwapchain())
            .setTarget("depth", RenderTarget::makeImage(RenderTargetType::DepthStencil,
                                                        RenderFormat::D32Float,
                                                        std::nullopt))
            .setSubgraph("default", RenderSubgraph()
                    .addAttachment(RenderAttachment("swapchain")
                                           .setClearValue(RenderAttachmentClearValue({0, 0, 0, 1}))
                                           .setLoadOp(RenderAttachmentLoadOp::Clear)
                                           .setStoreOp(RenderAttachmentStoreOp::Store)
                                           .setInitialLayout(RenderAttachmentLayout::Undefined)
                                           .setFinalLayout(RenderAttachmentLayout::Present))
                    .addAttachment(RenderAttachment("depth")
                                           .setFormat(RenderFormat::D32Float)
                                           .setClearValue(RenderAttachmentClearValue().setDepth(1))
                                           .setLoadOp(RenderAttachmentLoadOp::Clear)
                                           .setStoreOp(RenderAttachmentStoreOp::Store)
                                           .setInitialLayout(RenderAttachmentLayout::Undefined)
                                           .setFinalLayout(RenderAttachmentLayout::DepthStencilAttachment))
                    .addPass(RenderPass()
                                     .addColorAttachmentIdx(0)
                                     .setDepthStencilAttachment(1)
                                     .setOperator(RenderPassOperator(
                                             std::string(ForwardSceneDrawRenderOperator::NAME))))
            );

    auto renderContext = engine.resources().get<RenderContext>();
    renderContext->setRenderGraph(graph);

    auto ecsManager = engine.resources().get<ECSManager>();
    ecsManager->registerComponent<TestLogicTargetComponent>();
    ecsManager->registerSystem<TestLogicSystem>();

    auto sceneManager = engine.resources().get<SceneManager>();
    auto root = sceneManager->addRoot("Default");

    {
        auto entity = ecsManager->createEntity();

        auto meshRenderer = ecsManager->addComponent<MeshRendererComponent>(entity);
        meshRenderer->setMeshAsset("models/cube.obj");
        meshRenderer->setAlbedoTextureAsset("textures/texture-1024.png");

        auto transform = ecsManager->addComponent<TransformComponent>(entity);
        transform->getPos() = glm::vec3(0);

        sceneManager->insertEntityNode(root, entity);
    }

    {
        auto entity = ecsManager->createEntity();

        ecsManager->addComponent<TestLogicTargetComponent>(entity);
        ecsManager->addComponent<ViewComponent>(entity);

        auto camera = ecsManager->addComponent<CameraComponent>(entity);
        camera->getFov() = glm::radians(90.0f);

        auto transform = ecsManager->addComponent<TransformComponent>(entity);
        transform->getPos() = glm::vec3(0);
        transform->getRot() = glm::vec3(0);

        sceneManager->insertEntityNode(root, entity);
    }

    engine.run();
}
