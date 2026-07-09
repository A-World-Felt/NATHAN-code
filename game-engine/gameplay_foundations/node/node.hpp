#pragma once

#include <string>
#include <vector>
#include <memory>

// Scene Graph Node - represents an entity in the scene hierarchy
// A scene graph is a tree structure where nodes can have parent-child relationships
class Node {
public:
    std::string name;
    Node* parent = nullptr;
    std::vector<Node*> children;  // Raw pointers - owned by NodePool
    bool destroyed = false;       // Marked for deletion (deferred removal)

    virtual ~Node() = default;

    // Lifecycle methods - users override these
    virtual void setup();
    virtual void loop(float delta);
    virtual void cleanup();

    // Tree modification - uses global NodePool
    void add_child(std::unique_ptr<Node> child);
    void remove_child(Node* child);
    void destroy();  // Mark this node for deletion (removes self from parent)

    // Factory for creating event-enabled nodes
    template<typename T, typename... Args>
    static std::unique_ptr<T> create(Args&&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
};

// Include inline implementations
#include "node.inl.hpp"
