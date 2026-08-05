#include "events/event_types.hpp"

#include "game_world/node_3d.hpp"

namespace nathan {

SoundEvent::SoundEvent(EventNode *node, Sound& sound)
    : node_(node), node_3d_(dynamic_cast<Node3D*>(node)), sound_(&sound) {}

void SoundEvent::set_rel_position(const Node3D &head_node) const {
    if (node_3d_)
        sound_->set_rel_position(node_3d_->get_transform().get_relative_position_to(head_node.get_transform()));
}

void SoundEvent::set_node(EventNode *node) {
    node_ = node;
    // Copy of the node but give access to transform functions
    node_3d_ = dynamic_cast<Node3D*>(node);
}

} // namespace nathan
