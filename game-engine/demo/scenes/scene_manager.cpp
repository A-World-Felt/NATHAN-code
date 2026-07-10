#include "scene_manager.hpp"
#include "main_menu.hpp"
#include "mini_game.hpp"

#include <iostream>

namespace nathan {

SceneManager::SceneManager() {
    set_name("SceneManager");
}

void SceneManager::setup() {
    std::cout << "[SceneManager] Initializing...\n";
    
    // Start with main menu
    switch_to_scene(std::make_unique<MainMenu>());
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
    
    std::cout << "[SceneManager] Scene switched!\n";
}

void SceneManager::start_mini_game() {
    std::cout << "[SceneManager] Starting mini-game...\n";
    switch_to_scene(std::make_unique<MiniGame>());
}

void SceneManager::return_to_main() {
    std::cout << "[SceneManager] Returning to main menu...\n";
    switch_to_scene(std::make_unique<MainMenu>());
}

}  // namespace nathan
