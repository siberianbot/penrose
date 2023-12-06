#include <catch2/catch_all.hpp>

#include <array>

#include <glm/vec3.hpp>

#include <Penrose/Engine.hpp>
#include <Penrose/Assets/AssetManager.hpp>
#include <Penrose/ECS/EntityManager.hpp>
#include <Penrose/Rendering/RenderGraphContext.hpp>
#include <Penrose/Rendering/RenderGraphInfo.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Scene/SceneManager.hpp>

#include <Penrose/Builtin/Penrose/ECS/MeshRendererComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/PerspectiveCameraComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/TransformComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/ViewComponent.hpp>
#include <Penrose/Builtin/Penrose/Rendering/ForwardSceneDrawRenderOperator.hpp>

#include "src/ECS/TestCountdownSystem.hpp"

using namespace Penrose;

struct Cube {
    glm::vec3 pos;
    glm::vec3 rot;
    glm::vec3 color;
};

constexpr static const std::array<Cube, 7> CUBES = {
        Cube{
                glm::vec3(-6, 0, -4),
                glm::vec3(glm::radians(45.0f), 0, 0),
                glm::vec3(1, 0, 0)
        },
        Cube{
                glm::vec3(-4, 0, -4),
                glm::vec3(0, glm::radians(45.0f), 0),
                glm::vec3(1, 0.5, 0)
        },
        Cube{
                glm::vec3(-2, 0, -4),
                glm::vec3(0, 0, glm::radians(45.0f)),
                glm::vec3(1, 1, 0)
        },
        Cube{
                glm::vec3(0, 0, -4),
                glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0),
                glm::vec3(0, 1, 0)
        },
        Cube{
                glm::vec3(2, 0, -4),
                glm::vec3(0, glm::radians(45.0f), glm::radians(45.0f)),
                glm::vec3(0, 1, 1)
        },
        Cube{
                glm::vec3(4, 0, -4),
                glm::vec3(glm::radians(45.0f), 0, glm::radians(45.0f)),
                glm::vec3(0, 0, 1)
        },
        Cube{
                glm::vec3(6, 0, -4),
                glm::vec3(glm::radians(45.0f), glm::radians(45.0f), glm::radians(45.0f)),
                glm::vec3(0.5, 0, 1)
        },
};

TEST_CASE("Projection", "[engine-int-test]") {

    Engine engine;

    engine.resources().add<TestCountdownSystem>().group(ResourceGroup::ECSSystem)
            .implements<Initializable>()
            .implements<System>()
            .done();

    auto assetManager = engine.resources().get<AssetManager>();
    assetManager->addDir("tests/data");
    assetManager->load("models/cube.asset");
    assetManager->load("textures/white-16.asset");
    assetManager->load("shaders/default-forward-rendering.frag.asset");
    assetManager->load("shaders/default-forward-rendering.vert.asset");

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

    for (auto cube: CUBES) {
        auto entity = entityManager->createEntity();

        auto meshRenderer = std::make_shared<MeshRendererComponent>();
        meshRenderer->setMeshAsset("models/cube.asset");
        meshRenderer->setAlbedoTextureAsset("textures/white-16.asset");
        meshRenderer->setColor(cube.color);

        auto transform = std::make_shared<TransformComponent>();
        transform->getPos() = glm::vec3(cube.pos);
        transform->getRot() = glm::vec3(cube.rot);

        entityManager->addComponent(entity, meshRenderer);
        entityManager->addComponent(entity, transform);

        sceneManager->insertEntityNode(root, entity);
    }

    {
        auto entity = entityManager->createEntity();

        auto camera = std::make_shared<PerspectiveCameraComponent>();
        camera->getFov() = glm::radians(90.0f);

        auto transform = std::make_shared<TransformComponent>();
        transform->getPos() = glm::vec3(0, 0, 4);
        transform->getRot() = glm::vec3(0, glm::radians(90.0f), 0);

        entityManager->addComponent(entity, std::make_shared<ViewComponent>());
        entityManager->addComponent(entity, camera);
        entityManager->addComponent(entity, transform);

        sceneManager->insertEntityNode(root, entity);
    }

    REQUIRE_NOTHROW([&]() { engine.run(); }());
}
