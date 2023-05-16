#include "Engine.hpp"

#include "src/Backends/GlfwBackend.hpp"
#include "src/Events/EventQueue.hpp"
#include "src/Rendering/Surface.hpp"

namespace Penrose {

    Engine::Engine() {
        this->_resources.add<EventQueue>();
        this->_resources.add<GlfwBackend>();
        this->_resources.add<Surface>();
    }

    void Engine::run() {
        this->_resources.initAll();

        auto eventQueue = this->_resources.get<EventQueue>()->lock();
        auto surface = this->_resources.get<Surface>()->lock();

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
