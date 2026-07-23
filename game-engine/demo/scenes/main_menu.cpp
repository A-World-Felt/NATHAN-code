#include "main_menu.hpp"
#include "scene_manager.hpp"

#include <iostream>
#include <inputs/input_types.h>

namespace nathan {

void MainMenu::setup() {
    std::cout << "[MainMenu] Setup\n";
}

void MainMenu::loop(float delta) {
    // Input test
    if (Engine::instance().get_input_device()->is_button_pressed(gamepad_button::left_face_button)) {
        std::cout << "Button pressed\n";
    }

    std::cout << Engine::instance().get_input_device()->get_axis_value(gamepad_axis::x_left_joystick) << std::endl;

    // static float timer = 0.0f;
    // timer += delta;
    //
    // std::cout << "[MainMenu] Running... (" << timer << "s)\n";
    //
    // // Simulate user pressing "Start Game" after 2 seconds
    // if (timer >= 0.5f) {
    //     std::cout << "[MainMenu] User clicked 'Start Mini-Game' (parent="
    //               << (get_parent() ? "not null" : "null") << ")\n";
    //
    //     // Find DemoSceneManager (parent) and request scene change
    //     if (get_parent()) {
    //         if (auto* manager = dynamic_cast<DemoSceneManager*>(get_parent())) {
    //             std::cout << "[MainMenu] Found manager, calling start_mini_game\n";
    //             manager->start_mini_game();
    //         } else {
    //             std::cout << "[MainMenu] Parent is not DemoSceneManager!\n";
    //         }
    //     } else {
    //         std::cout << "[MainMenu] ERROR: parent is nullptr!\n";
    //     }
    //     timer = 0.0f;
    // }
}

void MainMenu::cleanup() {
    std::cout << "[MainMenu] Cleanup\n";
}

}  // namespace nathan
