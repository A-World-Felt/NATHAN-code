#ifndef GAME_ENGINE_GAME_OBJECT_H_
#define GAME_ENGINE_GAME_OBJECT_H_

#include "node/event_node.hpp"
#include "game_world/transform.hpp"

namespace nathan {

// Node that exists in space within its scene.
// A game object exists in a 3D space, but its functionnality is mainly 2D. Rotation is simplified to a single angle in degrees.
class Node3D : public EventNode {
public:
    Node3D() = default;
    Node3D(Vector3 position) : transform(position) {};
    Node3D(float x, float y, float z = 0.0f) : transform(x, y, z) {};
    
    Transform& get_transform() { return transform; }
    const Transform& get_transform() const { return transform; }

    // After loop integrates physics. Child classes can override it but they must call the base class version to ensure proper functionality.
    void after_loop(float delta);

private:
    Transform transform;
};

}  // namespace nathan

#endif  // GAME_ENGINE_GAME_OBJECT_H_