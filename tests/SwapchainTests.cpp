#include <catch2/catch.hpp>

#include <chrono>
#include <optional>
#include <thread>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Core/Engine.hpp>
#include <Penrose/ECS/System.hpp>
#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Rendering/RenderGraphContext.hpp>
#include <Penrose/Rendering/Surface.hpp>
#include <Penrose/Rendering/SurfaceManager.hpp>
#include <Penrose/Resources/Resource.hpp>

using namespace Penrose;

TEST_CASE("Attempts to resize swapchain",
          "[engine][event-queue][ecs][ecs-system][surface][surface-manager]") {
    class CountdownSystem : public Resource, public System {
    public:
        explicit CountdownSystem(ResourceSet *resources)
                : _eventQueue(resources->getLazy<EventQueue>()) {
            //
        }

        ~CountdownSystem() override {
            this->_timerThread = std::nullopt;
        }

        void update(float delta) override {
            this->_passed += delta;

            if (this->_passed > 3.0f) {
                this->_eventQueue->push(makeEvent(EventType::EngineDestroyRequested));

                if (!this->_timerThread.has_value()) {
                    this->_timerThread = std::thread([]() {
                        auto startTime = std::chrono::high_resolution_clock::now();
                        auto now = startTime;

                        while (std::chrono::duration<float>(now - startTime).count() < 3.0f) {
                            now = std::chrono::high_resolution_clock::now();
                        }

                        throw EngineError("Timeout, test failed");
                    });
                }
            }
        }

        [[nodiscard]] std::string getName() const override { return "CountdownSystem"; }

    private:
        Lazy<EventQueue> _eventQueue;

        float _passed = 0;
        std::optional<std::thread> _timerThread;
    };

    class SurfaceResizeSystem : public Resource, public System {
    public:
        explicit SurfaceResizeSystem(ResourceSet *resources)
                : _surfaceManager(resources->getLazy<SurfaceManager>()) {
            //
        }

        ~SurfaceResizeSystem() override = default;

        void update(float delta) override {
            this->_passed += delta;

            if (this->_passed > 1.0f) {
                this->_passed = 0;

                auto surface = this->_surfaceManager->getSurface();

                auto [width, height] = surface->getSize();
                surface->setSize({height, width});
            }
        }

        [[nodiscard]] std::string getName() const override { return "SurfaceResizeSystem"; }

    private:
        Lazy<SurfaceManager> _surfaceManager;

        float _passed = 0;
    };

    Engine engine;

    engine.resources().add<CountdownSystem, System>();
    engine.resources().add<SurfaceResizeSystem, System>();

    auto graph = RenderGraphInfo()
            .setTarget("swapchain", RenderTargetInfo(RenderTargetSource::Swapchain))
            .setSubgraph("default", RenderSubgraphInfo()
                    .addAttachment(RenderAttachmentInfo("swapchain")
                                           .setClearValue(RenderAttachmentClearValueInfo({0, 0, 0, 1}))
                                           .setLoadOp(RenderAttachmentLoadOp::Clear)
                                           .setStoreOp(RenderAttachmentStoreOp::Store)
                                           .setInitialLayout(RenderAttachmentLayout::Undefined)
                                           .setFinalLayout(RenderAttachmentLayout::Present))
                    .addPass(RenderSubgraphPassInfo().addColorAttachmentIdx(0))
            );
    engine.resources().get<RenderGraphContext>()->setRenderGraph(graph);

    engine.run();
}
