#include <catch2/catch.hpp>

#include <Penrose/Assets/AssetDictionary.hpp>
#include <Penrose/Assets/AssetManager.hpp>
#include <Penrose/Core/Engine.hpp>
#include <Penrose/ECS/ECSManager.hpp>
#include <Penrose/ECS/System.hpp>
#include <Penrose/Rendering/RenderGraphContext.hpp>
#include <Penrose/Rendering/RenderGraphInfo.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Scene/SceneManager.hpp>

#include <Penrose/Builtin/Penrose/ECS/CameraComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/MeshRendererComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/TransformComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/ViewComponent.hpp>
#include <Penrose/Builtin/Penrose/Rendering/ForwardSceneDrawRenderOperator.hpp>

#include "src/ECS/TestCountdownSystem.hpp"
#include "src/ECS/TestLogicTargetComponent.hpp"

using namespace Penrose;

TEST_CASE("ComplexScenes_OrbitalCamera", "[engine-int-test]") {

    class TestLogicSystem : public Resource, public Initializable, public System {
    public:
        explicit TestLogicSystem(ResourceSet *resources)
                : _ecsManager(resources->getLazy<ECSManager>()) {
            //
        }

        ~TestLogicSystem() override = default;

        void init() override {
            this->_passed = 0;

            auto query = ECSQuery().component<TestLogicTargetComponent>();
            this->_targetEntity = this->_ecsManager->queryEntities(std::forward<decltype(query)>(query)).at(0);
        }

        void update(float delta) override {
            this->_passed += delta;

            const int r = 4;
            auto angle = glm::radians(this->_passed * 90.0f);

            auto transform = this->_ecsManager->getComponent<TransformComponent>(this->_targetEntity);
            transform->getPos() = glm::vec3(r * cos(angle), 0, r * sin(angle));
            transform->getRot().y = M_PI - angle;
        }

        void destroy() override { /* nothing to do */ }

        [[nodiscard]] std::string getName() const override { return "TestLogic"; }

    private:
        Lazy<ECSManager> _ecsManager;

        float _passed = 0;
        Entity _targetEntity = -1;
    };

    Engine engine;

    engine.resources().add<TestCountdownSystem, System>();

    engine.resources().add<TestLogicTargetComponentFactory, ComponentFactory>();
    engine.resources().add<TestLogicSystem, System>();

    auto assetDictionary = engine.resources().get<AssetDictionary>();
    assetDictionary->addDir("tests/data");

    auto assetManager = engine.resources().get<AssetManager>();
    assetManager->enqueue("models/cube.asset");
    assetManager->enqueue("textures/texture-1024.asset");
    assetManager->enqueue("shaders/default-forward-rendering.frag.asset");
    assetManager->enqueue("shaders/default-forward-rendering.vert.asset");

    auto graph = RenderGraphInfo()
            .setTarget("swapchain", RenderTargetInfo(RenderTargetSource::Swapchain))
            .setTarget("depth", RenderTargetInfo(RenderTargetSource::Image,
                                                 RenderTargetType::DepthStencil,
                                                 RenderFormat::D32Float,
                                                 std::nullopt))
            .setSubgraph("default", RenderSubgraphInfo()
                    .addAttachment(RenderAttachmentInfo("swapchain")
                                           .setClearValue(RenderAttachmentClearValueInfo({0, 0, 0, 1}))
                                           .setLoadOp(RenderAttachmentLoadOp::Clear)
                                           .setStoreOp(RenderAttachmentStoreOp::Store)
                                           .setInitialLayout(RenderAttachmentLayout::Undefined)
                                           .setFinalLayout(RenderAttachmentLayout::Present))
                    .addAttachment(RenderAttachmentInfo("depth")
                                           .setFormat(RenderFormat::D32Float)
                                           .setClearValue(RenderAttachmentClearValueInfo().setDepth(1))
                                           .setLoadOp(RenderAttachmentLoadOp::Clear)
                                           .setStoreOp(RenderAttachmentStoreOp::Store)
                                           .setInitialLayout(RenderAttachmentLayout::Undefined)
                                           .setFinalLayout(RenderAttachmentLayout::DepthStencilAttachment))
                    .addPass(RenderSubgraphPassInfo()
                                     .addColorAttachmentIdx(0)
                                     .setDepthStencilAttachment(1)
                                     .setOperatorInfo(RenderOperatorInfo(
                                             std::string(ForwardSceneDrawRenderOperator::NAME))))
            );

    auto renderContext = engine.resources().get<RenderGraphContext>();
    renderContext->setRenderGraph(graph);

    auto ecsManager = engine.resources().get<ECSManager>();
    auto sceneManager = engine.resources().get<SceneManager>();
    auto root = sceneManager->addRoot("Default");

    {
        auto entity = ecsManager->createEntity();

        auto meshRenderer = ecsManager->addComponent<MeshRendererComponent>(entity);
        meshRenderer->setMeshAsset("models/cube.asset");
        meshRenderer->setAlbedoTextureAsset("textures/texture-1024.asset");

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

    REQUIRE_NOTHROW([&]() { engine.run(); }());
}