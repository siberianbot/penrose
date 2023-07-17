#include <Penrose/Scene/SceneNode.hpp>

#include <algorithm>

#include <Penrose/Common/EngineError.hpp>

namespace Penrose {

    SceneNode::SceneNode(const std::optional<std::weak_ptr<SceneNode>> &parent)
            : _parent(parent) {
        //
    }

    std::optional<std::shared_ptr<SceneNode>> SceneNode::getParent() const {
        if (!this->_parent.has_value()) {
            return std::nullopt;
        }

        if (this->_parent->expired()) {
            throw EngineError("Parent of node was expired");
        }

        return this->_parent->lock();
    }

    void SceneNode::setParent(const std::optional<std::shared_ptr<SceneNode>> &parent) {
        this->_parent = parent;
    }

    void SceneNode::addDescendant(const std::shared_ptr<SceneNode> &descendant) {
        this->_descendants.push_back(descendant);
    }

    void SceneNode::removeDescendant(const std::shared_ptr<SceneNode> &descendant) {
        auto it = std::find(this->_descendants.begin(), this->_descendants.end(), descendant);

        if (it != this->_descendants.end()) {
            this->_descendants.erase(it);
        }
    }
}
