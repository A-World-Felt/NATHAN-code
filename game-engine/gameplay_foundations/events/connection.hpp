#pragma once

#include <cstdint>
#include <functional>
#include <atomic>

// Unique token for disconnecting event listeners
class ConnectionToken {
private:
    uint64_t id;
    void* emitter;
    
    static std::atomic<uint64_t> next_id;

public:
    ConnectionToken() : id(0), emitter(nullptr) {}
    ConnectionToken(void* emitter) : id(next_id++), emitter(emitter) {}
    
    bool is_valid() const { return id != 0 && emitter != nullptr; }
    uint64_t get_id() const { return id; }
    void* get_emitter() const { return emitter; }
    
    bool operator==(const ConnectionToken& other) const {
        return id == other.id && emitter == other.emitter;
    }
    bool operator!=(const ConnectionToken& other) const {
        return !(*this == other);
    }
};

// Initialize static member
inline std::atomic<uint64_t> ConnectionToken::next_id{1};
