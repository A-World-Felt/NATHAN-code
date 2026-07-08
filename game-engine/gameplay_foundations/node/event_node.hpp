#pragma once

#include "node.hpp"
#include "../events/event_emitter.hpp"

// Node with event capabilities
class EventNode : public Node, public EventEmitter {
public:
    using Node::Node;
    
    // Emit event that bubbles up to parent
    template<typename T>
    void emit(std::string event_type, const T& data) {
        // First, notify local subscribers
        emit_internal<T>(event_type, data, this);
        
        // Then bubble up to parent if it's an EventNode
        if (parent) {
            if (auto* event_parent = dynamic_cast<EventNode*>(parent)) {
                event_parent->emit<T>(event_type, data);
            }
        }
    }
    
    // Emit event to children only
    template<typename T>
    void emit_to_children(std::string event_type, const T& data) {
        for (auto& child : children) {
            if (auto* event_child = dynamic_cast<EventNode*>(child.get())) {
                event_child->emit_internal<T>(event_type, data, this);
                event_child->emit_to_children<T>(event_type, data);
            }
        }
    }
};
