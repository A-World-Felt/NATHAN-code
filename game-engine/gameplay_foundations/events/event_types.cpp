#include "events/event_types.hpp"

#include "game_world/node_3d.hpp"

namespace nathan {

SoundEvent::SoundEvent(EventNode *node, const std::string_view path_to_file, const Type sound_type)
    : node_(node), sound_(path_to_file), type_(sound_type), state_(State::kPlaying) {}

void SoundEvent::set_rel_position(const Node3D &head_node) {
    if (const auto node_3d = dynamic_cast<Node3D*>(node_))
        sound_.set_rel_position(node_3d->get_transform().get_relative_position_to(head_node.get_transform()));
}

} // namespace nathan
