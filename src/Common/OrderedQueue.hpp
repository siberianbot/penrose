#ifndef PENROSE_COMMON_ORDERED_QUEUE_HPP
#define PENROSE_COMMON_ORDERED_QUEUE_HPP

#include <functional>
#include <memory>

namespace Penrose {

    template <typename T, typename EqualTo = std::equal_to<T>, typename Less = std::less<T>>
    class OrderedQueue {
    public:
        void push(T &&value) { this->insert(std::weak_ptr<Node>(), this->_root, std::forward<decltype(value)>(value)); }

        void pop() {
            auto node = visit(this->_root);

            if (node == nullptr) {
                return;
            }

            node->values.pop_front();

            if (!node->values.empty()) {
                return;
            }

            if (node == this->_root) {
                this->_root = node->right;

                if (this->_root != nullptr) {
                    this->_root->parent.reset();
                }
            } else {
                auto parent = node->parent.lock();

                parent->left = node->right;

                if (parent->left != nullptr) {
                    parent->left->parent = parent;
                }
            }
        }

        [[nodiscard]] bool empty() const { return this->_root == nullptr; }

        [[nodiscard]] T &front() { return visit(this->_root)->values.front(); }

        void clear() { this->_root = nullptr; }

    private:
        struct Node {
            std::weak_ptr<Node> parent;
            std::shared_ptr<Node> left, right;
            std::list<T> values;
        };

        std::shared_ptr<Node> _root;

        static void insert(std::weak_ptr<Node> &&parent, std::shared_ptr<Node> &node, T &&value) {
            if (node == nullptr) {
                node = std::make_shared<Node>();
                node->parent = parent;
                node->values.push_back(std::forward<decltype(value)>(value));
            } else if (EqualTo {}(node->values.front(), value)) {
                node->values.push_back(std::forward<decltype(value)>(value));
            } else {
                auto &target = Less {}(value, node->values.front()) ? node->left : node->right;

                insert(std::weak_ptr<Node>(node), target, std::forward<decltype(value)>(value));
            }
        }

        static std::shared_ptr<Node> &visit(std::shared_ptr<Node> &node) {
            if (node != nullptr && node->left != nullptr) {
                return visit(node->left);
            }

            return node;
        }
    };
}

#endif // PENROSE_COMMON_ORDERED_QUEUE_HPP
