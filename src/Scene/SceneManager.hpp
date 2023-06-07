#ifndef PENROSE_SCENE_SCENE_MANAGER_HPP
#define PENROSE_SCENE_SCENE_MANAGER_HPP

#include <optional>

#include "src/Common/Initializable.hpp"
#include "src/Resources/Resource.hpp"
#include "src/Scene/Scene.hpp"

namespace Penrose {

    class SceneManager : public Resource, public Initializable {
    private:
        std::optional<Scene> _currentScene;

    public:
        ~SceneManager() override = default;

        void init() override { /* nothing to do */ }

        void destroy() override;

        void setCurrentScene(const std::optional<Scene> &scene);

        [[nodiscard]] const std::optional<Scene> &getCurrentScene() const { return this->_currentScene; }
    };
}

#endif // PENROSE_SCENE_SCENE_MANAGER_HPP
