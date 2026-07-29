#include "engine/node_pool.hpp"
#include "node/event_node.hpp"
#include "node/node.hpp"

namespace nathan {

Node* NodePool::create(std::unique_ptr<Node> node) {
    if (!node) {
        return nullptr;
    }
    node_pool_.push_back(std::move(node));
    Node* raw_ptr = node_pool_.back().get();
    raw_ptr->setup();  // Automatically call setup when node is created
    return raw_ptr;
}

void NodePool::destroy(Node* node) {
    if (node && !node->is_destroyed()) {
        // Disconnect event subscriptions first to prevent dangling callbacks
        if (auto* event_node = dynamic_cast<EventNode*>(node)) {
            event_node->off_all();
            event_node->off_all_global();
        }
        
        // Recursively destroy all children first (depth-first)
        std::vector<Node*> children_copy(node->get_children());
        for (Node* child : children_copy) {
            destroy(child);
        }
        
        node->cleanup();      // Call cleanup before marking as destroyed
        node->set_destroyed(true);
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
                return ptr->is_destroyed();
            }),
        node_pool_.end()
    );
}

std::vector<std::unique_ptr<Node>>& NodePool::get_pool() {
    return node_pool_;
}

bool NodePool::contains(Node* node) const {
    if (!node) {
        return false;
    }
    for (const auto& ptr : node_pool_) {
        if (ptr.get() == node) {
            return true;
        }
    }
    return false;
}

}  // namespace nathan
