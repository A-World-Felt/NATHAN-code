#ifndef GAME_ENGINE_NODE_NODE_INL_H_
#define GAME_ENGINE_NODE_NODE_INL_H_

#include <algorithm>
#include <memory>

#include "engine/engine.hpp"

namespace nathan {

inline void Node::setup() {
    // Default: do nothing
}

inline void Node::loop(float delta) {
    // Default: do nothing
}

inline void Node::cleanup() {
    // Default: do nothing
}

// Tree modification - uses Engine's NodePool
inline void Node::add_child(std::unique_ptr<Node> child) {
    Node* raw = child.get();
    raw->parent_ = this;
    raw->engine_ = engine_;  // Child inherits engine from parent
    children_.push_back(raw);
    if (engine_) {
        engine_->get_node_pool().create(std::move(child));
    }
    // Note: setup() is called automatically by NodePool::create()
}

inline void Node::remove_child(Node* child) {
    auto it = std::find(children_.begin(), children_.end(), child);
    if (it != children_.end()) {
        child->parent_ = nullptr;
        if (engine_) {
            engine_->get_node_pool().destroy(child);
        }
        children_.erase(it);
    }
}

inline void Node::destroy() {
    if (parent_) {
        parent_->remove_child(this);
    } else if (engine_) {
        engine_->get_node_pool().destroy(this);
    }
}

}  // namespace nathan

#endif  // GAME_ENGINE_NODE_NODE_INL_H_
