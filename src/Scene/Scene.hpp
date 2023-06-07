#ifndef PENROSE_SCENE_SCENE_HPP
#define PENROSE_SCENE_SCENE_HPP

#include <list>
#include <memory>
#include <optional>

#include "src/ECS/ECSBase.hpp"

namespace Penrose {

    struct SceneNode {
        std::optional<Entity> entity;
        std::list<std::shared_ptr<SceneNode>> descendants;
    };

    class Scene {
    private:
        std::shared_ptr<SceneNode> _root;

    public:
        Scene();
        explicit Scene(const std::shared_ptr<SceneNode> &root);

        [[nodiscard]] std::shared_ptr<SceneNode> &getRoot() { return this->_root; }
    };
}

#endif // PENROSE_SCENE_SCENE_HPP
