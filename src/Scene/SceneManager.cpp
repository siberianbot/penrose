#include <Penrose/Scene/SceneManager.hpp>

namespace Penrose {

    void SceneManager::destroy() {
        this->_currentScene = std::nullopt;
    }

    void SceneManager::setCurrentScene(const std::optional<Scene> &scene) {
        this->_currentScene = scene;
    }
}
