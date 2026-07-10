#ifndef GAME_ENGINE_DEMO_SCENES_SCENE_MANAGER_H_
#define GAME_ENGINE_DEMO_SCENES_SCENE_MANAGER_H_

#include "node/event_node.hpp"
#include <memory>

namespace nathan {

// Forward declarations
class MainMenu;
class MiniGame;

class SceneManager : public EventNode {
public:
    SceneManager();

    void setup() override;
    void loop(float delta) override {};
    void cleanup() override;

    void switch_to_scene(std::unique_ptr<Node> new_scene);
    void start_mini_game();
    void return_to_main();

private:
    Node* current_scene_ = nullptr;
};

}  // namespace nathan

#endif  // GAME_ENGINE_DEMO_SCENES_SCENE_MANAGER_H_
