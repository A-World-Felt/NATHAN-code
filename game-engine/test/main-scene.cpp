#include "main-scene.hpp"

// ---------------- Player ----------------

void Player::setup() {
    std::cout << "[Player] Created at (" << x << ", " << y << ")\n";
    
    // Listen for collision events
    on<CollisionEvent>("collision", [this](const CollisionEvent& event) {
        std::cout << "[Player] Collided with " << event.with 
                  << " (force: " << event.force << ")\n";
        coins_collected++;
        emit<ScoreEvent>("score", {static_cast<int>(event.force * 10), "coin_collect"});
    });
    
    // Listen for welcome message from parent
    on<std::string>("welcome", [this](const std::string& msg) {
        std::cout << "[Player] Received welcome: " << msg << "\n";
    });
}

void Player::loop(float delta) {
    x += speed * delta;
    
    // Emit position update when integer position changes (bubbles up to parent)
    int x_int = static_cast<int>(x);
    if (x_int != last_position && x_int <= 60) {
        last_position = x_int;
        if (x_int > 0) {
            emit<std::string>("position_update", "x=" + std::to_string(x_int));
        }
    }
    
    // Simulate collision with coin at x=50
    if (!coins_collected && x >= 49.9f && x <= 50.1f) {
        std::cout << "[Player] Reached coin position! Triggering collision...\n";
        emit<CollisionEvent>("collision", {"coin", 2.5f});
        coins_collected = 1;
    }
    
    // Jump at x=20 and x=40 (demonstrates multiple events)
    if (!coins_collected) {
        if (x_int == 20 && jump_count == 0) {
            jump_count = 1;
            std::cout << "[Player] Jumping at x=" << x_int << "!\n";
            emit<std::string>("player_jump", "hop");
        } else if (x_int == 40 && jump_count == 1) {
            jump_count = 2;
            std::cout << "[Player] Jumping at x=" << x_int << "!\n";
            emit<std::string>("player_jump", "double_hop");
        }
    }
}

// ---------------- Coin ----------------

void Coin::setup() {
    std::cout << "[Coin] Created at (" << x << ", " << y << ") with value " << value << "\n";
    
    // Listen for player position updates (bubbling from child)
    on<std::string>("position_update", [this](const std::string& pos) {
        std::cout << "[Coin] Heard position update: " << pos << "\n";
    });
    
    // Listen for welcome from parent
    on<std::string>("welcome", [this](const std::string& msg) {
        std::cout << "[Coin] Received welcome: " << msg << "\n";
    });
}

void Coin::loop(float delta) {
    // Coin listens for position updates but doesn't emit anything in this demo
}

// ---------------- ScoreDisplay ----------------

void ScoreDisplay::setup() {
    std::cout << "[ScoreDisplay] Ready. Score: " << score << "\n";
    
    // Listen for score events from anywhere in the tree (bubbles up)
    on<ScoreEvent>("score", [this](const ScoreEvent& event) {
        score += event.points;
        std::cout << "[ScoreDisplay] +" << event.points << " points (" 
                  << event.source << "). Total: " << score << "\n";
    });
    
    // Listen for welcome from parent
    on<std::string>("welcome", [this](const std::string& msg) {
        std::cout << "[ScoreDisplay] Received welcome: " << msg << "\n";
    });
}

// ---------------- MainScene ----------------

MainScene::MainScene() {
    name = "MainScene";

    // Create game entities
    auto player = std::make_unique<Player>();
    player->name = "Player";
    add_child(std::move(player));

    auto coin = std::make_unique<Coin>();
    coin->name = "Coin";
    add_child(std::move(coin));

    // Create systems
    auto score_display = std::make_unique<ScoreDisplay>();
    score_display->name = "ScoreDisplay";
    add_child(std::move(score_display));
}

void MainScene::setup() {
    std::cout << "[MainScene] Ready!\n";
    
    // Listen for scene-level events
    on<std::string>("scene_ready", [](const std::string& msg) {
        std::cout << "[MainScene] Event received: " << msg << "\n";
    });
    
    // Forward score events to all children (so ScoreDisplay can hear them)
    on<ScoreEvent>("score", [this](const ScoreEvent& event) {
        std::cout << "[MainScene] Score event received: +" << event.points 
                  << " (" << event.source << ") - forwarding to children\n";
        emit_to_children<ScoreEvent>("score", event);
    });
    
    // Listen for player jumps
    on<std::string>("player_jump", [this](const std::string& jump_type) {
        std::cout << "[MainScene] Player jumped: " << jump_type << "\n";
    });
    
    // Forward position updates to all children
    on<std::string>("position_update", [this](const std::string& pos) {
        emit_to_children<std::string>("position_update", pos);
    });
    
    // Broadcast a welcome message to all children
    emit_to_children<std::string>("welcome", "Game started!");
}

// LOOP METHOD NO LONGER NEEDED - handled automatically by Engine!