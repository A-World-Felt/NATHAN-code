#pragma once

#include "../node/node.hpp"
#include <vector>
#include <unordered_map>

// Iterative tree traversal - avoids stack overflow and is faster
class TreeTraverser {
public:
    // Post-order traversal: children first, then parent
    static void traverse_setup(Node* root) {
        if (!root) return;
        
        std::vector<Node*> stack;
        std::vector<Node*> post_order;
        std::unordered_map<Node*, bool> visited;
        
        stack.push_back(root);
        
        while (!stack.empty()) {
            Node* node = stack.back();
            stack.pop_back();
            
            if (visited.find(node) != visited.end()) {
                post_order.push_back(node);
                continue;
            }
            
            visited[node] = true;
            stack.push_back(node); // Push again for post-processing
            
            // Push children in reverse order
            for (auto it = node->children.rbegin(); it != node->children.rend(); ++it) {
                stack.push_back(it->get());
            }
        }
        
        // Execute setup in post-order
        for (Node* node : post_order) {
            node->setup();
        }
    }
    
    // Pre-order traversal: parent first, then children
    static void traverse_loop(Node* root, float delta) {
        if (!root) return;
        
        std::vector<Node*> stack;
        stack.push_back(root);
        
        while (!stack.empty()) {
            Node* node = stack.back();
            stack.pop_back();
            
            // Process node first
            node->loop(delta);
            
            // Push children in reverse order (so first child is processed next)
            for (auto it = node->children.rbegin(); it != node->children.rend(); ++it) {
                stack.push_back(it->get());
            }
        }
    }
};
