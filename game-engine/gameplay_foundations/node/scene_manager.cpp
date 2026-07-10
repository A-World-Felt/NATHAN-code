#include "node/scene_manager.hpp"

#include <iostream>

namespace nathan {

SceneManager::SceneManager() {
    set_name("SceneManager");
}

void SceneManager::setup() {
    std::cout << "[SceneManager] Initializing...\n";
}

void SceneManager::loop(float delta) {
    // SceneManager itself doesn't need to do anything in the loop
    // The current scene will handle its own updates
}

void SceneManager::cleanup() {
    std::cout << "[SceneManager] Cleaning up...\n";
    if (current_scene_) {
        current_scene_->destroy();
        current_scene_ = nullptr;
    }
}

void SceneManager::switch_to_scene(std::unique_ptr<Node> new_scene) {
    std::cout << "[SceneManager] Switching to new scene...\n";

    // Clean up current scene
    if (current_scene_) {
        current_scene_->destroy();
        current_scene_ = nullptr;
    }

    // Set new scene
    new_scene->set_name("current_scene");
    current_scene_ = new_scene.get();
    add_child(std::move(new_scene));

    // Initialize the new scene
    current_scene_->setup();

    std::cout << "[SceneManager] Scene switched!\n";
}

}  // namespace nathan
