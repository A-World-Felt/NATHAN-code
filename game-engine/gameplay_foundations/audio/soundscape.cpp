#include "audio/soundscape.hpp"

#include "sound.hpp"
#include "node/event_node.hpp"

namespace nathan {

Soundscape::Soundscape(EventBus &event_bus, Node3D* head_node) : head_node_(head_node) {
    event_bus.on_global<SoundEvent>("play_sound", [this](const SoundEvent &sound_event) {
        if (sound_event.get_sound()->get_state() == Sound::State::kFileNotFound)
            return;
        sounds_.emplace_back(sound_event);
    });
}

void Soundscape::update() {
    for (auto it = sounds_.begin(); it != sounds_.end();) {
        // Deletion after one frame on State::kStopped
        if (!it->get_node() || it->get_sound()->get_state() == Sound::State::kStopped) {
            it = sounds_.erase(it);
            continue;
        }

        // Give one frame on State::kStopped to terminate the audio
        if (it->get_node()->is_destroyed())
            it->get_sound()->set_state(Sound::State::kStopped);
        else if (head_node_ && it->get_sound()->get_state() == Sound::State::kPlaying)
            it->set_rel_position(*head_node_);

        // Communication with audio module. To be determined.
        // shared_sounds_.push(it->get_sound());
        ++it;
    }
}

} // namespace nathan
