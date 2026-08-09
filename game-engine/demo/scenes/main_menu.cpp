#include "main_menu.hpp"
#include "scene_manager.hpp"

#include <iostream>
#include <inputs/input_types.hpp>

namespace nathan {

void MainMenu::setup() {
    std::cout << "[MainMenu] Setup\n";
    std::cout << "[MainMenu] Press any button to start...\n";
    on_global<GamepadButtonEvent>("button_pressed", [this](const GamepadButtonEvent& _) {
        is_game_started_ = true;
    });
}

void MainMenu::loop(float delta) {
    static float timer = 0.0f;
    timer += delta;

    if (is_game_started_ || timer >= 15.0f) {
        std::cout << "[MainMenu] User clicked 'Start Mini-Game' (parent="
              << (get_parent() ? "not null" : "null") << ")\n";

        // Find DemoSceneManager (parent) and request scene change
        if (get_parent()) {
            if (auto* manager = dynamic_cast<DemoSceneManager*>(get_parent())) {
                std::cout << "[MainMenu] Found manager, calling start_mini_game\n";
                manager->start_mini_game();
            } else {
                std::cout << "[MainMenu] Parent is not DemoSceneManager!\n";
            }
        } else {
            std::cout << "[MainMenu] ERROR: parent is nullptr!\n";
        }
        timer = 0.0f;
    }
}

void MainMenu::cleanup() {
    std::cout << "[MainMenu] Cleanup\n";
}

}  // namespace nathan
