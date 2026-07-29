#ifndef GAME_ENGINE_EVENTS_EVENT_BUS_H_
#define GAME_ENGINE_EVENTS_EVENT_BUS_H_

#include "events/event_emitter.hpp"

#include <memory>

namespace nathan {

// Global event bus
class EventBus : public EventEmitter {
public:
    EventBus() = default;

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

    // Unsubscribe a specific global subscription
    void off_global(const ConnectionToken& token) {
        off(token);
    }

    // Delete copy but allow move
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;
    EventBus(EventBus&&) = default;
    EventBus& operator=(EventBus&&) = default;
};

}  // namespace nathan

#endif  // GAME_ENGINE_EVENTS_EVENT_BUS_H_
