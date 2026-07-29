#ifndef GAME_ENGINE_NODE_EVENT_NODE_H_
#define GAME_ENGINE_NODE_EVENT_NODE_H_

#include "node/node.hpp"
#include "events/event_emitter.hpp"
#include "events/connection.hpp"

#include <vector>

namespace nathan {

// Node with event capabilities
class EventNode : public Node, public EventEmitter {
public:
    // Emit event that bubbles up to parent
    template<typename T>
    void emit(std::string event_type, const T& data) {
        // First, notify local subscribers
        emit_internal<T>(event_type, data, this);
        
        // Then bubble up to parent if it's an EventNode
        if (auto* event_parent = dynamic_cast<EventNode*>(get_parent())) {
            event_parent->emit<T>(event_type, data);
        }
    }
    
    // Emit event to children only
    template<typename T>
    void emit_to_children(std::string event_type, const T& data) {
        for (Node* child : get_children()) {
            if (!child->is_destroyed()) {
                if (EventNode* event_child = dynamic_cast<EventNode*>(child)) {
                    event_child->emit_internal<T>(event_type, data, this);
                    event_child->emit_to_children<T>(event_type, data);
                }
            }
        }
    }

    // Emit event on the global event bus
    template<typename T>
    void emit_global(std::string event_type, const T& data) {
        if (Engine *engine = get_engine()) {
            engine->get_event_bus().emit_global<T>(event_type, data);
        }
    }

    // Subscribe to events on the global event bus
    template<typename T>
    ConnectionToken on_global(std::string event_type, std::function<void(const T&)> callback) {
        if (Engine *engine = get_engine()) {
            ConnectionToken token = engine->get_event_bus().on_global<T>(event_type, callback);
            if (token.is_valid()) {
                global_connections_.push_back(token);
            }
            return token;
        }
        return {};
    }

    // Unsubscribe a specific global subscription
    void off_global(const ConnectionToken& token) {
        if (Engine *engine = get_engine()) {
            engine->get_event_bus().off_global(token);
        }
        // Remove from our tracking
        global_connections_.erase(
            std::remove_if(global_connections_.begin(), global_connections_.end(),
                [&token](const ConnectionToken& t) { return t == token; }),
            global_connections_.end()
        );
    }

    // Unsubscribe all global subscriptions for this node
    void off_all_global() {
        if (Engine *engine = get_engine()) {
            for (const auto& token : global_connections_) {
                engine->get_event_bus().off_global(token);
            }
        }
        global_connections_.clear();
    }

private:
    std::vector<ConnectionToken> global_connections_;
};

}  // namespace nathan

#endif  // GAME_ENGINE_NODE_EVENT_NODE_H_
