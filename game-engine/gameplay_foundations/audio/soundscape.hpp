#ifndef GAME_ENGINE_AUDIO_SOUNDSCAPE_H_
#define GAME_ENGINE_AUDIO_SOUNDSCAPE_H_

#include <queue>
#include <vector>

#include "events/event_bus.hpp"
#include "events/event_types.hpp"

namespace nathan {

// Forward declaration
class Node3D;

class Soundscape {
public:
    explicit Soundscape(EventBus &event_bus, Node3D *head_node = nullptr);

    Node3D* get_head_node() const { return head_node_; }
    void set_head_node (Node3D* node) { head_node_ = node; }
    
    [[maybe_unused]] std::vector<SoundEvent>& get_sounds() { return sounds_; }

    void update();

private:
    Node3D* head_node_;
    std::vector<SoundEvent> sounds_{};
    // std::queue<Sound*> shared_sounds_{};
};

} // namespace nathan

#endif //GAME_ENGINE_AUDIO_SOUNDSCAPE_H_
