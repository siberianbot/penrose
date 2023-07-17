#ifndef PENROSE_SCENE_SCENE_NODE_HPP
#define PENROSE_SCENE_SCENE_NODE_HPP

#include <list>
#include <memory>
#include <optional>

#include <Penrose/ECS/Entity.hpp>

namespace Penrose {

    class SceneNode {
    public:
        explicit SceneNode(const std::optional<std::weak_ptr<SceneNode>> &parent = std::nullopt);

        [[nodiscard]] std::optional<std::shared_ptr<SceneNode>> getParent() const;

        void setParent(const std::optional<std::shared_ptr<SceneNode>> &parent);

        [[nodiscard]] const std::list<std::shared_ptr<SceneNode>> &getDescendants() const {
            return this->_descendants;
        }

        void addDescendant(const std::shared_ptr<SceneNode> &descendant);
        void removeDescendant(const std::shared_ptr<SceneNode> &descendant);

        [[nodiscard]] std::optional<Entity> &getEntity() { return this->_entity; }

    private:
        std::optional<std::weak_ptr<SceneNode>> _parent;
        std::list<std::shared_ptr<SceneNode>> _descendants;
        std::optional<Entity> _entity;
    };

    using SceneNodePtr = std::shared_ptr<SceneNode>;
}

#endif // PENROSE_SCENE_SCENE_NODE_HPP
