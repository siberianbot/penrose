#include "Scene.hpp"

namespace Penrose {

    Scene::Scene() : Scene(std::make_shared<SceneTreeNode>()) {
        //
    }

    Scene::Scene(const std::shared_ptr<SceneTreeNode> &root) : _root(root) {
        //
    }
}
