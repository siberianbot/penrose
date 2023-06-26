#include "Scene.hpp"

#include <queue>
#include <ranges>
#include <set>

namespace Penrose {

    Scene::Scene() : Scene(std::make_shared<SceneTreeNode>()) {
        //
    }

    Scene::Scene(const std::shared_ptr<SceneTreeNode> &root) : _root(root) {
        //
    }

    std::optional<std::shared_ptr<SceneTreeNode>> Scene::findEntityNode(const Entity &entity) const {
        std::set<std::shared_ptr<SceneTreeNode>> discovered;
        std::queue<std::shared_ptr<SceneTreeNode>> queue;
        queue.push(this->_root);

        while (!queue.empty()) {
            auto current = queue.front();
            queue.pop();

            if (current->entity == entity) {
                return current;
            }

            for (const auto &descendant: std::views::reverse(current->descendants)) {
                if (discovered.contains(descendant)) {
                    continue;
                }

                queue.push(descendant);
            }

            discovered.insert(current);
        }

        return std::nullopt;
    }

    void Scene::insertEntity(const std::shared_ptr<SceneTreeNode> &node, const Entity &entity) {
        auto descendant = std::make_shared<SceneTreeNode>();
        descendant->parent = node;
        descendant->entity = entity;

        node->descendants.push_back(descendant);
    }
}
