#include "scene_manager.hpp"
#include "main_menu.hpp"
#include "mini_game.hpp"

#include <iostream>

namespace nathan {

DemoSceneManager::DemoSceneManager() {
    set_name("DemoSceneManager");
}

void DemoSceneManager::setup() {
    std::cout << "[DemoSceneManager] Initializing...\n";
    
    // Start with main menu
    switch_to_scene(std::make_unique<MainMenu>());
}

void DemoSceneManager::start_mini_game() {
    std::cout << "[DemoSceneManager] Starting mini-game...\n";
    switch_to_scene(std::make_unique<MiniGame>());
}

void DemoSceneManager::return_to_main() {
    std::cout << "[DemoSceneManager] Returning to main menu...\n";
    switch_to_scene(std::make_unique<MainMenu>());
}

}  // namespace nathan
