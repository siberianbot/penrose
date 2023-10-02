#include <Penrose/Scene/SceneManager.hpp>

#include <queue>
#include <set>

#include <fmt/core.h>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

namespace Penrose {

    void SceneManager::destroy() {
        this->_roots.clear();
    }

    SceneNodePtr SceneManager::addRoot(std::string &&name) {
        if (this->_roots.contains(name)) {
            throw EngineError(fmt::format("Root {} already exists", name));
        }

        auto root = std::make_shared<SceneNode>();

        this->_roots[name] = root;

        return root;
    }

    void SceneManager::removeRoot(std::string &&name) {
        auto it = this->_roots.find(name);

        if (it == this->_roots.end()) {
            throw EngineError(fmt::format("Root {} not found", name));
        }

        this->_roots.erase(it);
    }

    SceneNodePtr SceneManager::getOrAddRoot(std::string &&name) {
        auto maybeRoot = this->tryGetRoot(name);

        if (maybeRoot.has_value()) {
            return *maybeRoot;
        }

        return this->addRoot(std::forward<decltype(name)>(name));
    }

    std::optional<SceneNodePtr> SceneManager::tryGetRoot(const std::string &name) const {
        auto it = this->_roots.find(name);

        if (it == this->_roots.end()) {
            return std::nullopt;
        }

        return it->second;
    }

    std::optional<SceneNodePtr> SceneManager::tryGetRoot(const SceneNodePtr &node) const {
        auto current = node;

        while (true) {
            auto maybeParent = current->getParent();

            if (!maybeParent.has_value()) {
                break;
            }

            current = *maybeParent;
        }

        for (const auto &[name, root]: this->_roots) {
            if (root == current) {
                return root;
            }
        }

        return std::nullopt;
    }

    SceneNodePtr SceneManager::getRoot(const std::string &name) const {
        return orElseThrow(this->tryGetRoot(name),
                           EngineError(fmt::format("Root {} not found", name)));
    }

    SceneNodePtr SceneManager::getRoot(const SceneNodePtr &node) const {
        return orElseThrow(this->tryGetRoot(node),
                           EngineError("Node is not part of any root"));
    }

    std::optional<std::string> SceneManager::tryGetRootNameOf(const SceneNodePtr &node) const {
        auto current = node;

        while (true) {
            auto maybeParent = node->getParent();

            if (!maybeParent.has_value()) {
                break;
            }

            current = *maybeParent;
        }

        for (const auto &[name, root]: this->_roots) {
            if (root == current) {
                return name;
            }
        }

        return std::nullopt;
    }

    std::string SceneManager::getRootNameOf(const SceneNodePtr &node) const {
        return orElseThrow(this->tryGetRootNameOf(node),
                           EngineError("Node is not part of any root"));
    }

    std::optional<SceneNodePtr> SceneManager::tryFindEntityNode(const Entity &entity) const {
        for (const auto &[_, root]: this->_roots) {
            auto result = SceneManager::tryFindEntityNode(root, entity);

            if (result.has_value()) {
                return result;
            }
        }

        return std::nullopt;
    }

    SceneNodePtr SceneManager::findEntityNode(const Entity &entity) const {
        return orElseThrow(this->tryFindEntityNode(entity),
                           EngineError("Entity is not mounted"));
    }

    SceneNodePtr SceneManager::insertEmptyNode(const SceneNodePtr &parent) {
        auto node = std::make_shared<SceneNode>(parent);

        parent->addDescendant(node);

        return node;
    }

    SceneNodePtr SceneManager::insertEntityNode(const SceneNodePtr &parent, const Entity &entity) {
        auto maybeMounted = this->tryFindEntityNode(entity);

        if (maybeMounted.has_value()) {
            throw EngineError(fmt::format("Entity {} already mounted", entity));
        }

        auto node = this->insertEmptyNode(parent);
        node->getEntity() = entity;

        return node;
    }

    void SceneManager::moveNode(const SceneNodePtr &newParent, const SceneNodePtr &node) {
        auto maybeNodeParent = node->getParent();
        if (maybeNodeParent.has_value()) {
            if (maybeNodeParent == newParent) {
                return;
            }

            (*maybeNodeParent)->removeDescendant(node);
        }

        node->setParent(newParent->getParent());
        newParent->addDescendant(node);
    }

    void SceneManager::removeNode(SceneNodePtr &&node, bool reparentDescendants) {
        auto maybeNodeParent = node->getParent();

        if (!maybeNodeParent.has_value()) {
            throw EngineError("Unable to remove node: node is root");
        }

        (*maybeNodeParent)->removeDescendant(node);

        if (!reparentDescendants) {
            return;
        }

        for (const auto &descendant: node->getDescendants()) {
            descendant->setParent(maybeNodeParent);
            (*maybeNodeParent)->addDescendant(descendant);
        }
    }

    std::optional<SceneNodePtr> SceneManager::tryFindEntityNode(const SceneNodePtr &root,
                                                                const Entity &entity) {
        std::queue<SceneNodePtr> queue;

        queue.push(root);

        while (!queue.empty()) {
            auto current = queue.front();
            queue.pop();

            if (current->getEntity() == entity) {
                return current;
            }

            for (const auto &descendant: current->getDescendants()) {
                queue.push(descendant);
            }
        }

        return std::nullopt;
    }
}
