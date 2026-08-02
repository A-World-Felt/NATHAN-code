#include "events/event_types.hpp"

#include "game_world/node_3d.hpp"

namespace nathan {

SoundEvent::SoundEvent(EventNode *node, Sound& sound)
    : node_(node), sound_(&sound) {}

void SoundEvent::set_rel_position(const Node3D &head_node) const {
    if (const auto node_3d = dynamic_cast<Node3D*>(node_))
        sound_->set_rel_position(head_node.get_transform().get_relative_position_to(node_3d->get_transform()));
}

} // namespace nathan
