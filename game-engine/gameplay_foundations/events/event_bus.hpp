#pragma once

#include "event_emitter.hpp"
#include <memory>

// Global event bus - singleton for global events (not hierarchical)
class EventBus : public EventEmitter {
public:
    static EventBus& global() {
        static EventBus instance;
        return instance;
    }

    // Emit globally
    template<typename T>
    void emit_global(std::string event_type, const T& data) {
        emit_internal<T>(event_type, data);
    }

    // Subscribe globally
    template<typename T>
    ConnectionToken on_global(std::string event_type, std::function<void(const T&)> callback) {
        return on<T>(event_type, callback);
    }

private:
    EventBus() = default;
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;
};
