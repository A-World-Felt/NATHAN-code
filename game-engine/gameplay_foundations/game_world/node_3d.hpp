#ifndef GAME_ENGINE_GAME_WORLD_NODE_3D_H_
#define GAME_ENGINE_GAME_WORLD_NODE_3D_H_

#include "node/event_node.hpp"
#include "game_world/transform.hpp"

namespace nathan {

// Node that exists in space within its scene.
// A game object exists in a 3D space, but its functionnality is mainly 2D. Rotation is simplified to a single angle in degrees.
class Node3D : public EventNode {
public:
    Node3D() = default;
    Node3D(const Vector3 position) : transform(position) {};
    Node3D(const float x, const float y, const float z = 0.0f) : transform(x, y, z) {};
    
    Transform& get_transform() { return transform; }
    const Transform& get_transform() const { return transform; }

    // After loop integrates physics. Child classes can override it but they must call the base class version to ensure proper functionality.
    void after_loop(float delta) override;

    // Set the node as the "user's ears"
    void set_as_head_node();

private:
    Transform transform;
};

}  // namespace nathan

#endif  // GAME_ENGINE_GAME_WORLD_NODE_3D_H_