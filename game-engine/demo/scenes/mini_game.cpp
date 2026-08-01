#include "mini_game.hpp"
#include "scene_manager.hpp"

#include <iostream>

namespace nathan {

// PLAYER - Demonstrates: emitting events, subscribing to events

void Player::setup() {
    set_as_head_node();

    std::cout << "[Player] Created at (" << get_transform().get_position().x 
              << ", " << get_transform().get_position().y << ")\n";
    
    on<CollisionEvent>("collision", [this](const CollisionEvent& event) {
        std::cout << "[Player] Collided with " << event.with 
                  << " (force: " << event.force << ")\n";
        coins_collected_++;
        emit<ScoreEvent>("score", {static_cast<int>(event.force * 10), "coin_collect"});
    });
    
    on<std::string>("welcome", [this](const std::string& msg) {
        std::cout << "[Player] Received welcome: " << msg << "\n";
    });

    on_global<std::pair<GamepadAxis, float>>("axis_value_changed", [this](const std::pair<GamepadAxis, float>& axis) {
        constexpr float kDeadzone = 0.1f;
        const float val = std::abs(axis.second) < kDeadzone ? 0.0f : axis.second;

        if (axis.first == GamepadAxis::kXLeftJoystickAxis) {
            axis_x_ = val;
        } else if (axis.first == GamepadAxis::kYLeftJoystickAxis) {
            axis_y_ = val;
        }
    });

    on_global<GamepadButton>("button_pressed", [this](const GamepadButton& button) {
        if (button == GamepadButton::kBottomFaceButton) {
            std::cout << "[Player] Jumping!\n";
        }
    });
}

void Player::loop(float delta) {
    get_transform().translate(axis_x_ * speed_ * delta, axis_y_ * speed_ * delta);

    std::cout << "[Player] X: " << get_transform().get_position().x << " Y: " << get_transform().get_position().y << "\n";
    
    // int x_int = static_cast<int>(get_x());
    // if (x_int != get_last_position() && x_int <= 60) {
    //     set_last_position(x_int);
    //     if (x_int > 0) {
    //         emit<std::string>("position_update", "x=" + std::to_string(x_int));
    //     }
    // }
    
    if (coins_collected_ == 0 && get_transform().get_position().x >= 49.5f && get_transform().get_position().y <= 50.5f) {
        std::cout << "[Player] Reached coin position! Triggering collision...\n";
        emit<CollisionEvent>("collision", {.with = "coin", .force = 2.5f});
        emit_global<SoundEvent>("play_sound", { this, sound_ });
        // set_coins_collected(1);
    }
    
    // if (get_coins_collected() == 0) {
    //     if (x_int >= 20 && x_int < 21 && get_jump_count() == 0) {
    //         set_jump_count(1);
    //         std::cout << "[Player] Jumping at x=" << x_int << "!\n";
    //         emit<std::string>("player_jump", "hop");
    //     } else if (x_int >= 40 && x_int < 41 && get_jump_count() == 1) {
    //         set_jump_count(2);
    //         std::cout << "[Player] Jumping at x=" << x_int << "!\n";
    //         emit<std::string>("player_jump", "double_hop");
    //     }
    // }
}

// COIN - Demonstrates: subscribing to events from other nodes

void Coin::setup() {
    std::cout << "[Coin] Created at (" << get_transform().get_position().x << ", " 
              << get_transform().get_position().y << ") with value " << get_value() << "\n";
    
    on<std::string>("position_update", [this](const std::string& pos) {
        std::cout << "[Coin] Heard position update: " << pos << "\n";
    });
    
    on<std::string>("welcome", [this](const std::string& msg) {
        std::cout << "[Coin] Received welcome: " << msg << "\n";
    });
}

void Coin::loop(float delta) {}

// SCORE DISPLAY - Demonstrates: subscribing to events from any node

void ScoreDisplay::setup() {
    std::cout << "[ScoreDisplay] Ready. Score: " << get_score() << "\n";
    
    on<ScoreEvent>("score", [this](const ScoreEvent& event) {
        set_score(get_score() + event.points);
        std::cout << "[ScoreDisplay] +" << event.points << " points (" 
                  << event.source << "). Total: " << get_score() << "\n";
    });
    
    on<std::string>("welcome", [this](const std::string& msg) {
        std::cout << "[ScoreDisplay] Received welcome: " << msg << "\n";
    });
}

// MINI GAME - Demonstrates: scene management, event forwarding

MiniGame::MiniGame() {
    set_name("MiniGame");
}

void MiniGame::setup() {
    std::cout << "[MiniGame] Setting up game entities...\n";
    
    auto player = std::make_unique<Player>();
    player->set_name("Player");
    add_child(std::move(player));
    
    // Spawn coin at (50, 50)
    auto coin = std::make_unique<Coin>(50.0f, 50.0f);
    coin->set_name("Coin");
    add_child(std::move(coin));
    
    
    auto score_display = std::make_unique<ScoreDisplay>();
    score_display->set_name("ScoreDisplay");
    add_child(std::move(score_display));
    
    std::cout << "[MiniGame] Game ready!\n";
    
    on<ScoreEvent>("score", [this](const ScoreEvent& event) {
        std::cout << "[MiniGame] Score event received: +" << event.points 
                  << " (" << event.source << ") - forwarding to children\n";
        emit_to_children<ScoreEvent>("score", event);
    });
    
    on<std::string>("position_update", [this](const std::string& pos) {
        emit_to_children<std::string>("position_update", pos);
    });
    
    on<std::string>("player_jump", [this](const std::string& jump_type) {
        std::cout << "[MiniGame] Player jumped: " << jump_type << "\n";
    });
    
    emit_to_children<std::string>("welcome", "Mini-game started!");
}

void MiniGame::loop(float delta) {
    timer_ += delta;
    // std::cout << "[MiniGame] Running... (" << timer_ << "s)\n";
    
    if (timer_ >= 50.0f) {
        std::cout << "[MiniGame] User clicked 'Quit'\n";
        if (get_parent()) {
            if (auto* manager = dynamic_cast<DemoSceneManager*>(get_parent())) {
                manager->return_to_main();
            }
        }
        timer_ = 0.0f;
    }
}

void MiniGame::cleanup() {
    std::cout << "[MiniGame] Cleanup\n";
}

}  // namespace nathan
