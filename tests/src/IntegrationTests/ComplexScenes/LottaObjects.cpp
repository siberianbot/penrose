#include <catch2/catch_all.hpp>

#include <Penrose/Engine.hpp>
#include <Penrose/Assets/AssetDictionary.hpp>
#include <Penrose/Assets/AssetManager.hpp>
#include <Penrose/ECS/EntityManager.hpp>
#include <Penrose/ECS/System.hpp>
#include <Penrose/Math/Constants.hpp>
#include <Penrose/Rendering/RenderGraphContext.hpp>
#include <Penrose/Rendering/RenderGraphInfo.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Scene/SceneManager.hpp>

#include <Penrose/Builtin/Penrose/ECS/MeshRendererComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/PerspectiveCameraComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/TransformComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/ViewComponent.hpp>
#include <Penrose/Builtin/Penrose/Rendering/ForwardSceneDrawRenderOperator.hpp>

#include "src/ECS/TestCountdownSystem.hpp"
#include "src/ECS/TestLogicTargetComponent.hpp"

using namespace Penrose;

TEST_CASE("ComplexScenes_LottaObjects", "[engine-int-test]") {

    class TestLogicSystem : public Resource<TestLogicSystem, ResourceGroup::ECSSystem>,
                            public Initializable,
                            public System {
    public:
        explicit TestLogicSystem(ResourceSet *resources)
                : _entityManager(resources->get<EntityManager>()) {
            //
        }

        ~TestLogicSystem() override = default;

        void init() override {
            this->_passed = 0;

            this->_targetEntity = this->_entityManager->query()
                    .component<TestLogicTargetComponent>()
                    .front()
                    .value()
                    .entity;
        }

        void update(float delta) override {
            this->_passed += delta;

            const int r = 10;
            auto angle = glm::radians(this->_passed * 90.0f);

            auto transform = this->_entityManager->getComponent<TransformComponent>(this->_targetEntity);
            transform->getPos() = glm::vec3(r * cos(angle), 3, r * sin(angle));
            transform->getRot().y = PI_F - angle;
        }

        void destroy() override { /* nothing to do */ }

        [[nodiscard]] std::string getName() const override { return "TestLogic"; }

    private:
        ResourceProxy<EntityManager> _entityManager;

        float _passed = 0;
        Entity _targetEntity = -1;
    };

    Engine engine;

    engine.resources().add<TestCountdownSystem, ResourceGroup::ECSSystem>()
            .implements<Initializable>()
            .implements<System>()
            .done();

    engine.resources().add<TestLogicTargetComponentFactory, ResourceGroup::ECSComponent>()
            .implements<ComponentFactory>()
            .done();

    engine.resources().add<TestLogicSystem, ResourceGroup::ECSSystem>()
            .implements<Initializable>()
            .implements<System>()
            .done();

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

    auto entityManager = engine.resources().get<EntityManager>();
    auto sceneManager = engine.resources().get<SceneManager>();
    auto root = sceneManager->addRoot("Default");

    const int width = 32;
    const int height = 32;
    const float offset = 4;
    const float totalWidth = width * offset;
    const float totalHeight = height * offset;

    for (float x = -(totalWidth / 2); x <= (totalWidth / 2); x += offset) {
        for (float y = -(totalHeight / 2); y <= (totalHeight / 2); y += offset) {
            auto entity = entityManager->createEntity();

            auto meshRenderer = std::make_shared<MeshRendererComponent>();
            meshRenderer->setMeshAsset("models/cube.asset");
            meshRenderer->setAlbedoTextureAsset("textures/texture-1024.asset");

            auto transform = std::make_shared<TransformComponent>();
            transform->getPos() = glm::vec3(x, 0, y);

            entityManager->addComponent(entity, meshRenderer);
            entityManager->addComponent(entity, transform);

            sceneManager->insertEntityNode(root, entity);
        }
    }

    {
        auto entity = entityManager->createEntity();

        auto camera = std::make_shared<PerspectiveCameraComponent>();
        camera->getFov() = glm::radians(90.0f);

        auto transform = std::make_shared<TransformComponent>();
        transform->getPos() = glm::vec3(0);
        transform->getRot() = glm::vec3(0, 0, glm::radians(-15.0f));

        entityManager->addComponent(entity, std::make_shared<TestLogicTargetComponent>());
        entityManager->addComponent(entity, std::make_shared<ViewComponent>());
        entityManager->addComponent(entity, camera);
        entityManager->addComponent(entity, transform);

        sceneManager->insertEntityNode(root, entity);
    }

    REQUIRE_NOTHROW([&]() { engine.run(); }());
}
