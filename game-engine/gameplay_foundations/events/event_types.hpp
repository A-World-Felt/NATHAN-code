#ifndef GAME_ENGINE_EVENTS_EVENT_TYPES_H_
#define GAME_ENGINE_EVENTS_EVENT_TYPES_H_

#include "audio/sound.hpp"
#include "game_world/node_3d.hpp"
#include "inputs/input_types.hpp"

namespace nathan {

// Forward declarations
class EventNode;
class Node3D;

struct GamepadButtonEvent {
    enum class Action : uint8_t { kPressed, kReleased };

    GamepadButton gamepad_button;
    Action action;
};

struct  GamepadAxisEvent {
    GamepadAxis gamepad_axis;
    float val;
};

class SoundEvent {
public:

    SoundEvent(EventNode *node, Sound& sound);

    // Interface to modify the relative position in the Sound reference
    void set_rel_position(const Node3D &head_node) const;

    EventNode* get_node() const { return node_; }
    void set_node(EventNode* node) { node_ = node; node_3d_ = dynamic_cast<Node3D*>(node); }

    Sound* get_sound() const { return sound_; }
    void set_sound(Sound* sound) { sound_ = sound; }

private:
    EventNode* node_;
    Node3D* node_3d_;
    Sound* sound_;
};

} // namespace nathan

#endif // GAME_ENGINE_EVENTS_EVENT_TYPES_H_
