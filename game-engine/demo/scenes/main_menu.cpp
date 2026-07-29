#include "main_menu.hpp"
#include "scene_manager.hpp"

#include <iostream>
#include <inputs/input_types.hpp>

namespace nathan {

void MainMenu::setup() {
    std::cout << "[MainMenu] Setup\n";
    std::cout << "[MainMenu] Press any button to start...\n";
    
    // ConfigManager example usage
    if (Engine* engine = get_engine()) {
        auto& config_manager = engine->get_config_manager();
        
        // Use predefined ConfigDef (name and path are linked)
        auto& config = config_manager.get_or_create(ConfigManager::kDefaultConfig);
        
        // Load the config file (if it exists)
        if (!config.load()) {
            std::cout << "[MainMenu] No existing config found, using defaults\n";
            
            // Set some default values
            config.set_float("volume", 0.8f);
            config.set_int("difficulty", 2);
            config.set_bool("fullscreen", false);
            config.set_string("player_name", "Player1");
            
            // Save the defaults
            config.save();
        } else {
            std::cout << "[MainMenu] Loaded config from " << config.get_path() << "\n";
            
            // Read and display the values
            std::cout << "[MainMenu] Volume: " << config.get_float("volume", 0.8f) << "\n";
            std::cout << "[MainMenu] Difficulty: " << config.get_int("difficulty", 2) << "\n";
            std::cout << "[MainMenu] Fullscreen: " << (config.get_bool("fullscreen", false) ? "true" : "false") << "\n";
            std::cout << "[MainMenu] Player Name: " << config.get_string("player_name", "Player1") << "\n";
        }
    }
    
    on_global<GamepadButton>(sdl_events::button_pressed, [this](const GamepadButton& _) {
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
