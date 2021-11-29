// Copyright 2021 byteihq <kotov038@gmail.com>

#ifndef BINARYTREE_BINARYTREE_H
#define BINARYTREE_BINARYTREE_H

#include <memory>

template<typename T, typename Allocator = std::allocator<T>>
class BinaryTree {
private:
    struct Node {
        T value_;
        Node *left_;
        Node *right_;

        explicit Node(const T &value) : value_(value), left_(nullptr), right_(nullptr) {}

        T &value() {
            return value_;
        }

        const T &value() const {
            return value_;
        }

        Node *left() {
            return left_;
        }

        Node *right() {
            return right_;
        }
    };

    using node_alloc = typename Allocator::template rebind<Node>::other;
    node_alloc allocator_;
    using AllocTraits = std::allocator_traits<node_alloc>;
    Node *root_;
    size_t size_;

    void try_construct(Node *&node, const T &value) {
        try {
            node = AllocTraits::allocate(allocator_, 1);
            AllocTraits::construct(allocator_, node, value);
        } catch (...) {
            AllocTraits::destroy(allocator_, node);
            AllocTraits::deallocate(allocator_, node, 1);
            throw;
        }
    }

    void append(const T &value, Node *node) {
        if (value < node->value_) {
            if (node->left_ != nullptr) {
                append(value, node->left_);
            } else {
                try_construct(node->left_, value);
            }
        } else {
            if (node->right_ != nullptr) {
                append(value, node->right_);
            } else {
                try_construct(node->right_, value);
            }
        }
    }

    Node *find(const T &value, Node *node) {
        if (node == nullptr || node->value_ == value) {
            return node;
        }
        if (value < node->value_) {
            return find(value, node->left_);
        } else {
            return find(value, node->right_);
        }
    }

    Node *get_parent(const T &value, Node *node, Node *prev) {
        if (node->value_ == value) {
            return prev;
        }
        if (value < node->value_) {
            return get_parent(value, node->left_, node);
        } else {
            return get_parent(value, node->right_, node);
        }
    }

    Node *get_minimum_from_this(Node *node) {
        while (node->left_ != nullptr) {
            node = node->left_;
        }
        return node;
    }

    Node *get_parent(const T &value) {
        if (!has(value)) {
            return nullptr;
        }
        return get_parent(value, root_, nullptr);
    }

    void remove_all(Node* node) {
        if (node != nullptr) {
            remove_all(node->left_);
            remove_all(node->right_);
            AllocTraits::destroy(allocator_, node);
            AllocTraits::deallocate(allocator_, node, 1);
        }
    }
public:
    BinaryTree() : root_(nullptr), size_(0) {}

    explicit BinaryTree(const T &value) {
        try_construct(root_, value);
        size_ = 1;
    }

    Node *root() {
        return root_;
    }

    void append(const T &value) {
        if (root_ == nullptr) {
            try_construct(root_, value);
            ++size_;
            return;
        }
        append(value, root_);
        ++size_;
    }

    Node *find(const T &value) {
        return find(value, root_);
    }

    bool has(const T &value) {
        return find(value) != nullptr;
    }

    size_t size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    bool remove(const T &value) {
        auto node = find(value);
        if (node == nullptr) {
            return false;
        }
        auto parent = get_parent(value);
        // node doesn't have children
        if (node->left_ == nullptr && node->right_ == nullptr) {
            if (node != root_) {
                if (parent->left_ == node) {
                    parent->left_ = nullptr;
                } else {
                    parent->right_ = nullptr;
                }
            } else {
                root_ = nullptr;
            }
            AllocTraits::destroy(allocator_, node);
            AllocTraits::deallocate(allocator_, node, 1);
        } else if (node->left_ != nullptr && node->right_ != nullptr) { // node has two children
            auto min_value = get_minimum_from_this(node)->value_;
            remove(min_value);
            node->value_ = min_value;
        } else { // node has only one child
            auto child = node->left_ != nullptr ? node->left_ : node->right_;
            if (node != root_) {
                if (node == parent->left_) {
                    parent->left_ = child;
                } else {
                    parent->right_ = child;
                }
            } else {
                root_ = child;
            }
            AllocTraits::destroy(allocator_, node);
            AllocTraits::deallocate(allocator_, node, 1);
        }
        --size_;
        return true;
    }

    ~BinaryTree() {
        remove_all(root_);
    }
};

#endif //BINARYTREE_BINARYTREE_H
