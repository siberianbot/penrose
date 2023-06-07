#include "Scene.hpp"

namespace Penrose {

    Scene::Scene() : Scene(std::make_shared<SceneNode>()) {
        //
    }

    Scene::Scene(const std::shared_ptr<SceneNode> &root) : _root(root) {
        //
    }
}
