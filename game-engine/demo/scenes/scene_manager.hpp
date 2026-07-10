#ifndef GAME_ENGINE_DEMO_SCENES_DEMO_SCENE_MANAGER_H_
#define GAME_ENGINE_DEMO_SCENES_DEMO_SCENE_MANAGER_H_

#include "node/scene_manager.hpp"

namespace nathan {

// Forward declarations for demo-specific scenes
class MainMenu;
class MiniGame;

// Demo-specific SceneManager that extends the generic one
class DemoSceneManager : public SceneManager {
public:
    DemoSceneManager();

    void setup() override;
    void start_mini_game();
    void return_to_main();
};

}  // namespace nathan

#endif  // GAME_ENGINE_DEMO_SCENES_DEMO_SCENE_MANAGER_H_
