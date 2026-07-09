#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include "../engine/engine.hpp"

// Inline implementations for Node

inline void Node::setup() {
    // Default: do nothing
}

inline void Node::loop(float delta) {
    // Default: do nothing
}

inline void Node::cleanup() {
    // Default: do nothing
}

// Tree modification - uses singleton Engine
inline void Node::add_child(std::unique_ptr<Node> child) {
    Node* raw = child.get();
    raw->parent = this;
    children.push_back(raw);
    Engine::instance().get_node_pool().create(std::move(child));
    // Note: setup() is called automatically by NodePool::create()
}

inline void Node::remove_child(Node* child) {
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end()) {
        child->parent = nullptr;
        Engine::instance().get_node_pool().destroy(child);
        children.erase(it);
    }
}

inline void Node::destroy() {
    if (parent) {
        parent->remove_child(this);
    } else {
        Engine::instance().get_node_pool().destroy(this);
    }
}
