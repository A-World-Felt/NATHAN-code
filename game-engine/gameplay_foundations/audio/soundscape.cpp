#include "audio/soundscape.hpp"

#include "node/event_node.hpp"

namespace nathan {

Soundscape::Soundscape(EventBus &event_bus, Node3D* head_node) : event_bus_(event_bus), head_node_(head_node) {
    event_bus.on_global<SoundEvent>("play sound", [this](const SoundEvent &sound_event) {
        // Lock mutex for interprocess communication
        sounds_.emplace_back(sound_event);
    });

    event_bus.on_global<SoundEvent>("stop sound", [this](const SoundEvent &sound_event) {
        // Lock mutex for interprocess communication
        // end_sounds_.push(sound.get_id());
    });
}

void Soundscape::update() {
    for (auto it = sounds_.begin(); it != sounds_.end();) {

        if (it->get_node()->is_destroyed()) {
            it->set_state(SoundEvent::State::kStopped);
        }
        else if (head_node_ != nullptr && it->get_type() == SoundEvent::Type::kDynamic && it->get_state() == SoundEvent::State::kPlaying)
            it->set_rel_position(*head_node_);

        ++it;
    }
}

} // namespace nathan
