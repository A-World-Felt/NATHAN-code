#ifndef GAME_ENGINE_EVENTS_EVENT_EMITTER_H_
#define GAME_ENGINE_EVENTS_EVENT_EMITTER_H_

#include "events/connection.hpp"

#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace nathan {

// Forward declaration
class Node;

class EventEmitter {
public:
    virtual ~EventEmitter() = default;

    // Subscribe to events of type T, returns token for disconnecting
    template<typename T>
    ConnectionToken on(std::string event_type, std::function<void(const T&)> callback) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        auto token = ConnectionToken(static_cast<void*>(this));
        
        // Store type-erased callback
        auto wrapped = [callback](const void* data) {
            const T* typed_data = static_cast<const T*>(data);
            callback(*typed_data);
        };
        
        callbacks_[event_type].push_back({
            token.get_id(),
            wrapped,
            [this, token, event_type]() {
                this->disconnect_internal(token, event_type);
            }
        });
        
        return token;
    }

    // Subscribe once (auto-disconnect after first call)
    template<typename T>
    ConnectionToken once(std::string event_type, std::function<void(const T&)> callback) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        auto token = ConnectionToken(static_cast<void*>(this));
        
        auto wrapped = [callback, this, token, event_type](const void* data) {
            const T* typed_data = static_cast<const T*>(data);
            callback(*typed_data);
            this->disconnect_internal(token, event_type);
        };
        
        callbacks_[event_type].push_back({
            token.get_id(),
            wrapped,
            [this, token, event_type]() {
                this->disconnect_internal(token, event_type);
            }
        });
        
        return token;
    }

    // Disconnect a specific subscription
    void off(const ConnectionToken& token) {
        if (!token.is_valid()) return;
        
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto& [type, cbs] : callbacks_) {
            disconnect_internal(token, type);
        }
    }

    // Disconnect all subscriptions for a specific event type
    void off(std::string event_type) {
        std::lock_guard<std::mutex> lock(mutex_);
        callbacks_.erase(event_type);
    }

    // Disconnect all subscriptions
    void off_all() {
        std::lock_guard<std::mutex> lock(mutex_);
        callbacks_.clear();
    }

protected:
    // Emit event to subscribers (internal, for bubbling)
    template<typename T>
    void emit_internal(std::string event_type, const T& data, Node* sender = nullptr) {
        std::vector<CallbackEntry> callbacks_copy;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            auto it = callbacks_.find(event_type);
            if (it != callbacks_.end()) {
                callbacks_copy = it->second;
            }
        }
        
        // Execute outside lock
        for (auto& cb : callbacks_copy) {
            cb.callback(&data);
        }
    }

private:
    struct CallbackEntry {
        uint64_t id;
        std::function<void(const void*)> callback;
        std::function<void()> disconnect;
    };
    
    std::unordered_map<std::string, std::vector<CallbackEntry>> callbacks_;
    mutable std::mutex mutex_;
    
    void disconnect_internal(const ConnectionToken& token, std::string event_type) {
        auto it = callbacks_.find(event_type);
        if (it == callbacks_.end()) return;
        
        auto& entries = it->second;
        entries.erase(
            std::remove_if(entries.begin(), entries.end(),
                [id = token.get_id()](const CallbackEntry& entry) {
                    return entry.id == id;
                }),
            entries.end()
        );
    }
};

}  // namespace nathan

#endif  // GAME_ENGINE_EVENTS_EVENT_EMITTER_H_
