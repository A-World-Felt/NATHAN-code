#pragma once

#include <memory>
#include <vector>

// Forward declarations
class Node;
class EventNode;

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
    
    // Get the underlying node pool (for iteration)
    std::vector<std::unique_ptr<Node>>& get_pool();
    
    // Check if a node is in this pool
    bool contains(Node* node) const;

private:
    std::vector<std::unique_ptr<Node>> node_pool_;
};
