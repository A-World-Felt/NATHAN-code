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

    SoundEvent(EventNode *node, Sound& sound, Type sound_type = Type::kDynamic);

    // Interface to modify the relative position in the Sound reference
    void set_rel_position(const Node3D &head_node) const;

    EventNode* get_node() const { return node_; }
    void set_node(EventNode* node) { node_ = node; }

    Sound* get_sound() const { return sound_; }
    void set_sound(Sound* sound) { sound_ = sound; }

    Type get_type() const { return type_; }
    void set_type(const Type sound_type) { type_ = sound_type; }

private:
    EventNode* node_;
    Sound* sound_;
    Type type_;
};

} // namespace nathan

#endif // GAME_ENGINE_EVENTS_EVENT_TYPES_H_
