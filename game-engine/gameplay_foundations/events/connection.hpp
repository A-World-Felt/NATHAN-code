#ifndef GAME_ENGINE_EVENTS_CONNECTION_H_
#define GAME_ENGINE_EVENTS_CONNECTION_H_

#include <atomic>
#include <cstdint>
#include <functional>

namespace nathan {

// Unique token for disconnecting event listeners
class ConnectionToken {
public:
    ConnectionToken() : id_(0), emitter_(nullptr) {}
    explicit ConnectionToken(void* emitter) : id_(get_next_id()), emitter_(emitter) {}
    
    bool is_valid() const { return id_ != 0 && emitter_ != nullptr; }
    uint64_t get_id() const { return id_; }
    void* get_emitter() const { return emitter_; }
    
    bool operator==(const ConnectionToken& other) const {
        return id_ == other.id_ && emitter_ == other.emitter_;
    }
    bool operator!=(const ConnectionToken& other) const {
        return !(*this == other);
    }

private:
    static uint64_t get_next_id() {
        static std::atomic<uint64_t> next_id{1};
        return next_id++;
    }
    
    uint64_t id_;
    void* emitter_;
};

}  // namespace nathan

#endif  // GAME_ENGINE_EVENTS_CONNECTION_H_
