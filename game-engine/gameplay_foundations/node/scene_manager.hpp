#ifndef NATHAN_NODE_SCENE_MANAGER_H_
#define NATHAN_NODE_SCENE_MANAGER_H_

#include <memory>

#include "node/event_node.hpp"

namespace nathan {

// Generic SceneManager - part of the game engine
// Manages scene transitions between any Node-based scenes
class SceneManager : public EventNode {
public:
    SceneManager();

    void setup() override;
    void loop(float delta) override;
    void cleanup() override;

    // Switch to a new scene (takes ownership of the unique_ptr)
    void switch_to_scene(std::unique_ptr<Node> new_scene);

    // Get the current scene
    Node* get_current_scene() const { return current_scene_; }

private:
    Node* current_scene_ = nullptr;
};

}  // namespace nathan

#endif  // NATHAN_NODE_SCENE_MANAGER_H_
