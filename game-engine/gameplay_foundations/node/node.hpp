#ifndef GAME_ENGINE_NODE_NODE_H_
#define GAME_ENGINE_NODE_NODE_H_

#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace nathan {

// Scene Graph Node - represents an entity in the scene hierarchy
// A scene graph is a tree structure where nodes can have parent-child relationships
class Engine;  // Forward declaration

class Node {
public:
    virtual ~Node() = default;

    // Lifecycle methods - users override these
    virtual void setup();
    virtual void loop(float delta);
    virtual void after_loop(float delta);
    virtual void cleanup();

    // Accessors
    std::string_view get_name() const { return name_; }
    void set_name(std::string name) { name_ = std::move(name); }

    Node* get_parent() const { return parent_; }
    void set_parent(Node* parent) { parent_ = parent; }

    const std::vector<Node*>& get_children() const { return children_; }

    bool is_destroyed() const { return destroyed_; }
    void set_destroyed(bool destroyed) { destroyed_ = destroyed; }

    // Engine accessor
    Engine* get_engine() const { return engine_; }
    void set_engine(Engine* engine) { engine_ = engine; }

    // Tree modification - uses Engine's NodePool
    void add_child(std::unique_ptr<Node> child);
    void remove_child(Node* child);
    void destroy();  // Mark this node for deletion (removes self from parent)

private:
    std::string name_;
    Node* parent_ = nullptr;
    std::vector<Node*> children_;  // Raw pointers - owned by NodePool
    bool destroyed_ = false;       // Marked for deletion (deferred removal)
    Engine* engine_ = nullptr;     // Engine this node belongs to
};

}  // namespace nathan

// Include inline implementations
#include "node/node.inl.hpp"

#endif  // GAME_ENGINE_NODE_NODE_H_
