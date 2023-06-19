#ifndef PENROSE_SCENE_SCENE_HPP
#define PENROSE_SCENE_SCENE_HPP

#include <list>
#include <memory>
#include <optional>

#include "src/ECS/ECSBase.hpp"

namespace Penrose {

    struct SceneTreeNode {
        std::optional<Entity> entity;
        std::list<std::shared_ptr<SceneTreeNode>> descendants;
    };

    class Scene {
    private:
        std::shared_ptr<SceneTreeNode> _root;

    public:
        Scene();
        explicit Scene(const std::shared_ptr<SceneTreeNode> &root);

        [[nodiscard]] std::shared_ptr<SceneTreeNode> &getRoot() { return this->_root; }

        [[nodiscard]] const std::shared_ptr<SceneTreeNode> &getRoot() const { return this->_root; }
    };
}

#endif // PENROSE_SCENE_SCENE_HPP
