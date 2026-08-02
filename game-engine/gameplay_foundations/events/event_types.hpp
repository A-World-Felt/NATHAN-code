#ifndef GAME_ENGINE_EVENTS_EVENT_TYPES_H_
#define GAME_ENGINE_EVENTS_EVENT_TYPES_H_

#include "audio/sound.hpp"

namespace nathan {

// Forward declarations
class EventNode;
class Node3D;

class SoundEvent {
public:

    SoundEvent(EventNode *node, Sound& sound);

    // Interface to modify the relative position in the Sound reference
    void set_rel_position(const Node3D &head_node) const;

    EventNode* get_node() const { return node_; }
    void set_node(EventNode* node) { node_ = node; }

    Sound* get_sound() const { return sound_; }
    void set_sound(Sound* sound) { sound_ = sound; }

private:
    EventNode* node_;
    Sound* sound_;
};

} // namespace nathan

#endif // GAME_ENGINE_EVENTS_EVENT_TYPES_H_
