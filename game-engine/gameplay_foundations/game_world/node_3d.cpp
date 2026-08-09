#include "game_world/node_3d.hpp"

namespace nathan {

void Node3D::after_loop(float delta) {
    transform.integrate(delta);
}

void Node3D::set_as_head_node() {
    if (const auto engine = get_engine())
        engine->get_soundscape().set_head_node(this);
}

}  // namespace nathan