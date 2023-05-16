#include "Engine.hpp"

#include "src/Backends/GlfwBackend.hpp"
#include "src/Rendering/Surface.hpp"

namespace Penrose {

    Engine::Engine() {
        this->_resources.add<GlfwBackend>();
        this->_resources.add<Surface>();
    }

    void Engine::run() {
        this->_resources.initAll();

        // TODO

        this->_resources.destroyAll();
    }
}
