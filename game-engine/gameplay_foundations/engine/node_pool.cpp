#include "node_pool.hpp"
#include "../node/node.hpp"

Node* NodePool::create(std::unique_ptr<Node> node) {
    Node* raw_ptr = node.get();
    node_pool_.push_back(std::move(node));
    raw_ptr->setup();  // Automatically call setup when node is created
    return raw_ptr;
}

void NodePool::destroy(Node* node) {
    if (node && !node->destroyed) {
        node->cleanup();      // Call cleanup before marking as destroyed
        node->destroyed = true;
    }
}

void NodePool::queue_remove(Node* node) {
    destroy(node);
}

void NodePool::cleanup_destroyed() {
    // Remove destroyed nodes from node_pool
    node_pool_.erase(
        std::remove_if(node_pool_.begin(), node_pool_.end(),
            [](const std::unique_ptr<Node>& ptr) {
                return ptr->destroyed;
            }),
        node_pool_.end()
    );
}

std::vector<std::unique_ptr<Node>>& NodePool::get_pool() {
    return node_pool_;
}

bool NodePool::contains(Node* node) const {
    for (const auto& ptr : node_pool_) {
        if (ptr.get() == node) {
            return true;
        }
    }
    return false;
}
