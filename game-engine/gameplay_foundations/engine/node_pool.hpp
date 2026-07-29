#ifndef GAME_ENGINE_ENGINE_NODE_POOL_H_
#define GAME_ENGINE_ENGINE_NODE_POOL_H_

#include <memory>
#include <vector>

namespace nathan {

// Forward declarations
class Engine;
class Node;

// NodePool manages ownership and lifecycle of all Nodes
// This is separate from the tree structure (which uses raw pointers)
class NodePool {
public:
    NodePool() = default;
    
    // Create a node and take ownership
    Node* create(std::unique_ptr<Node> node);
    
    // Mark a node for deletion (deferred)
    void destroy(Node* node);
    
    // Alias for destroy
    void queue_remove(Node* node);
    
    // Clean up all destroyed nodes from the pool
    void cleanup_destroyed();
    
    // Check if a node is in this pool
    bool contains(Node* node) const;

private:
    friend class Engine;
    
    std::vector<std::unique_ptr<Node>> node_pool_;
    
    // For iteration - returns reference to internal pool
    std::vector<std::unique_ptr<Node>>& get_pool();
};

}  // namespace nathan

#endif  // GAME_ENGINE_ENGINE_NODE_POOL_H_
