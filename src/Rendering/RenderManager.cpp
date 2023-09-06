#include <Penrose/Rendering/RenderManager.hpp>

#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    RenderManager::RenderManager(ResourceSet *resources)
            : _renderSystem(resources->getLazy<RenderSystem>()) {
        //
    }

    void RenderManager::run() {
        this->_thread = std::jthread([this](const std::stop_token &stopToken) {
            while (!stopToken.stop_requested()) {
                this->_renderSystem->renderFrame();
            }
        });
    }

    void RenderManager::stop() {
        if (!this->_thread.has_value()) {
            return;
        }

        this->_thread->request_stop();

        if (this->_thread->joinable()) {
            this->_thread->join();
        }

        this->_thread = std::nullopt;
    }
}
