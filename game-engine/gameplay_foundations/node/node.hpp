#pragma once

#include <string>
#include <vector>
#include <memory>

// Forward declaration
class Node;

// Pure tree structure - no events, no logic
class Node {
public:
    std::string name;
    Node* parent = nullptr;
    std::vector<std::unique_ptr<Node>> children;

    virtual ~Node() = default;

    // Lifecycle methods - users override these
    virtual void setup();
    virtual void loop(float delta);

    // Tree modification
    void add_child(std::unique_ptr<Node> child);
    void remove_child(const std::string& name);

    // Factory for creating event-enabled nodes
    template<typename T, typename... Args>
    static std::unique_ptr<T> create(Args&&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
};

// Include inline implementations
#include "node.inl.hpp"
