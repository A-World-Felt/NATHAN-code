#include "events/event_types.hpp"

namespace nathan {

SoundEvent::SoundEvent(EventNode *node, Sound& sound)
    : node_(node), node_3d_(dynamic_cast<Node3D*>(node)), sound_(&sound) {}

void SoundEvent::set_rel_position(const Node3D &head_node) const {
    if (node_3d_) {}
        sound_->set_rel_position(node_3d_->get_transform().get_relative_position_to(head_node.get_transform()));
}

} // namespace nathan
