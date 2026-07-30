#include "game_world/node_3d.hpp"

namespace nathan {

Node3D::Node3D() = default;

void Node3D::after_loop(float delta) {
    transform.integrate(delta);
}

}  // namespace nathan