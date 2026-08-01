#ifndef GAME_ENGINE_AUDIO_SOUNDSCAPE_H_
#define GAME_ENGINE_AUDIO_SOUNDSCAPE_H_

#include <vector>

#include "events/event_bus.hpp"
#include "events/event_types.hpp"

namespace nathan {

class Node3D;

class Soundscape {
public:
    explicit Soundscape(EventBus &event_bus, Node3D *head_node = nullptr);

    Node3D* get_node() const { return head_node_; }
    void set_head_node (Node3D* node) { head_node_ = node; }

    void update();

private:
    EventBus& event_bus_;
    Node3D* head_node_;
    std::vector<SoundEvent> sounds_{};
};

} // namespace nathan

#endif //GAME_ENGINE_AUDIO_SOUNDSCAPE_H_
