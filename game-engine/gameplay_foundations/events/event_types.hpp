#ifndef GAME_ENGINE_EVENTS_EVENT_TYPES_H_
#define GAME_ENGINE_EVENTS_EVENT_TYPES_H_

#include "audio/sound.hpp"

namespace nathan {

// Forward declarations
class EventNode;
class Node3D;

class SoundEvent {
public:
    enum class Type : uint8_t { kStatic, kDynamic };
    enum class State : uint8_t { kPlaying, kStopped };

    SoundEvent(EventNode *node, std::string_view path_to_file, Type sound_type = Type::kDynamic);

    EventNode* get_node() const { return node_; }
    void set_node(EventNode* node) { node_ = node; }

    void set_rel_position(const Node3D &head_node);

    Type get_type() const { return type_; }
    void set_type(const Type sound_type) { type_ = sound_type; }

    State get_state() const { return state_; }
    void set_state(const State state) { state_ = state; }

private:
    EventNode* node_;
    Sound sound_;
    Type type_;
    State state_;
};

} // namespace nathan

#endif // GAME_ENGINE_EVENTS_EVENT_TYPES_H_
