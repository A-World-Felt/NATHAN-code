#include "main_menu.hpp"
#include "scene_manager.hpp"

#include <iostream>

namespace nathan {

void MainMenu::setup() {
    std::cout << "[MainMenu] Setup\n";
}

void MainMenu::loop(float delta) {
    static float timer = 0.0f;
    timer += delta;
    
    std::cout << "[MainMenu] Running... (" << timer << "s)\n";
    
    // Simulate user pressing "Start Game" after 2 seconds
    if (timer >= 0.5f) {
        std::cout << "[MainMenu] User clicked 'Start Mini-Game' (parent=" 
                  << (get_parent() ? "not null" : "null") << ")\n";
        
        // Find SceneManager (parent) and request scene change
        if (get_parent()) {
            if (auto* manager = dynamic_cast<SceneManager*>(get_parent())) {
                std::cout << "[MainMenu] Found manager, calling start_mini_game\n";
                manager->start_mini_game();
            } else {
                std::cout << "[MainMenu] Parent is not SceneManager!\n";
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
