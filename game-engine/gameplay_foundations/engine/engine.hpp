#ifndef GAME_ENGINE_ENGINE_ENGINE_H_
#define GAME_ENGINE_ENGINE_ENGINE_H_

#include "engine/node_pool.hpp"

namespace nathan {

// Forward declaration
class Node;

// Engine is a singleton - only one instance exists
class Engine {
public:
    // Delete copy/move constructors and assignment
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    Engine(Engine&&) = delete;
    Engine& operator=(Engine&&) = delete;
    
    // Get the singleton instance
    static Engine& instance();
    
    void set_root(std::unique_ptr<Node> scene);
    void run();
    void stop();
    
    // Get current root
    Node* get_root() const { return root_; }
    
    // Access to NodePool
    NodePool& get_node_pool() { return node_pool_; }
    const NodePool& get_node_pool() const { return node_pool_; }

private:
    // Private constructor - use instance() to get the Engine
    Engine();
    
    Node* root_ = nullptr;  // Raw pointer - owned by node_pool_
    NodePool node_pool_;  // Owns all nodes
    bool running_ = true;
    
};

}  // namespace nathan

#endif  // GAME_ENGINE_ENGINE_ENGINE_H_
