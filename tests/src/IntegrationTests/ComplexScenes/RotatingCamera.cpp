#include <catch2/catch_all.hpp>

#include <ranges>

#include <Penrose/Assets/AssetManager.hpp>
#include <Penrose/ECS/EntityManager.hpp>
#include <Penrose/ECS/System.hpp>
#include <Penrose/Engine.hpp>
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

TEST_CASE("ComplexScenes_RotatingCamera", "[engine-int-test]") {

    class TestLogicSystem: public Resource<TestLogicSystem>,
                           public Initializable,
                           public System {
    public:
        explicit TestLogicSystem(const ResourceSet *resources)
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

            auto transform = this->_entityManager->getComponent<TransformComponent>(this->_targetEntity);
            transform->getRot().y = glm::radians(this->_passed * 360.0f);
        }

        void destroy() override { /* nothing to do */
        }

        [[nodiscard]] std::string getName() const override { return "TestLogic"; }

    private:
        ResourceProxy<EntityManager> _entityManager;

        float _passed = 0;
        Entity _targetEntity = -1;
    };

    Engine engine;

    engine.resources()
        .add<TestCountdownSystem>().group(ResourceGroup::ECSSystem)
        .implements<Initializable>()
        .implements<System>()
        .done();

    engine.resources()
        .add<TestLogicTargetComponentFactory>().group(ResourceGroup::ECSComponent)
        .implements<ComponentFactory>()
        .done();

    engine.resources()
        .add<TestLogicSystem>().group(ResourceGroup::ECSSystem)
        .implements<Initializable>()
        .implements<System>()
        .done();

    auto assetManager = engine.resources().get<AssetManager>();
    assetManager->addDir("tests/data");
    assetManager->load("models/cube.asset");
    assetManager->load("textures/texture-1024.asset");
    assetManager->load("shaders/default-forward-rendering.frag.asset");
    assetManager->load("shaders/default-forward-rendering.vert.asset");

    auto graph = RenderGraphInfo()
                     .setTarget("swapchain", RenderTargetInfo(RenderTargetSource::Swapchain))
                     .setTarget(
                         "depth", RenderTargetInfo(
                                      RenderTargetSource::Image, RenderTargetType::DepthStencil, RenderFormat::D32Float,
                                      std::nullopt
                                  )
                     )
                     .setSubgraph(
                         "default",
                         RenderSubgraphInfo()
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
                                          .setOperatorInfo(
                                              RenderOperatorInfo(std::string(ForwardSceneDrawRenderOperator::NAME))
                                          ))
                     );

    auto renderContext = engine.resources().get<RenderGraphContext>();
    renderContext->setRenderGraph(graph);

    auto entityManager = engine.resources().get<EntityManager>();
    auto sceneManager = engine.resources().get<SceneManager>();
    auto root = sceneManager->addRoot("Default");

    for (int x: std::ranges::views::iota(-2, 3)) {
        auto entity = entityManager->createEntity();

        auto meshRenderer = std::make_shared<MeshRendererComponent>();
        meshRenderer->setMeshAsset("models/cube.asset");
        meshRenderer->setAlbedoTextureAsset("textures/texture-1024.asset");

        auto transform = std::make_shared<TransformComponent>();
        transform->getPos() = glm::vec3(2.5 * x, 0, 4);

        entityManager->addComponent(entity, meshRenderer);
        entityManager->addComponent(entity, transform);

        sceneManager->insertEntityNode(root, entity);
    }

    {
        auto entity = entityManager->createEntity();

        auto camera = std::make_shared<PerspectiveCameraComponent>();
        camera->getFov() = glm::radians(120.0f);

        auto transform = std::make_shared<TransformComponent>();
        transform->getPos() = glm::vec3(0, 0, -4);
        transform->getRot() = glm::vec3(0, glm::radians(-90.0f), 0);

        entityManager->addComponent(entity, std::make_shared<TestLogicTargetComponent>());
        entityManager->addComponent(entity, std::make_shared<ViewComponent>());
        entityManager->addComponent(entity, camera);
        entityManager->addComponent(entity, transform);

        sceneManager->insertEntityNode(root, entity);
    }

    REQUIRE_NOTHROW([&]() { engine.run(); }());
}
