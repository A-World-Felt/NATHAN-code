#include "mini_game.hpp"
#include "scene_manager.hpp"

// ============================================================================
// PLAYER - Demonstrates: emitting events, subscribing to events
// ============================================================================

void Player::setup() {
    std::cout << "[Player] Created at (" << x << ", " << y << ")\n";
    
    // Listen for collision events
    on<CollisionEvent>("collision", [this](const CollisionEvent& event) {
        std::cout << "[Player] Collided with " << event.with 
                  << " (force: " << event.force << ")\n";
        coins_collected++;
        emit<ScoreEvent>("score", {static_cast<int>(event.force * 10), "coin_collect"});
    });
    
    // Listen for welcome message from parent (MiniGame)
    on<std::string>("welcome", [this](const std::string& msg) {
        std::cout << "[Player] Received welcome: " << msg << "\n";
    });
}

void Player::loop(float delta) {
    x += speed * delta;
    
    // Emit position update (bubbles up to parent)
    int x_int = static_cast<int>(x);
    if (x_int != last_position && x_int <= 60) {
        last_position = x_int;
        if (x_int > 0) {
            emit<std::string>("position_update", "x=" + std::to_string(x_int));
        }
    }
    
    // Simulate collision with coin at x=50
    if (!coins_collected && x >= 49.5f && x <= 50.5f) {
        std::cout << "[Player] Reached coin position! Triggering collision...\n";
        emit<CollisionEvent>("collision", {"coin", 2.5f});
        coins_collected = 1;
    }
    
    // Jump at x=20 and x=40 (demonstrates multiple events)
    if (!coins_collected) {
        if (x_int >= 20 && x_int < 21 && jump_count == 0) {
            jump_count = 1;
            std::cout << "[Player] Jumping at x=" << x_int << "!\n";
            emit<std::string>("player_jump", "hop");
        } else if (x_int >= 40 && x_int < 41 && jump_count == 1) {
            jump_count = 2;
            std::cout << "[Player] Jumping at x=" << x_int << "!\n";
            emit<std::string>("player_jump", "double_hop");
        }
    }
}

// ============================================================================
// COIN - Demonstrates: subscribing to events from other nodes
// ============================================================================

void Coin::setup() {
    std::cout << "[Coin] Created at (" << x << ", " << y << ") with value " << value << "\n";
    
    // Listen for player position updates (bubbling from sibling Player)
    on<std::string>("position_update", [this](const std::string& pos) {
        std::cout << "[Coin] Heard position update: " << pos << "\n";
    });
    
    // Listen for welcome from parent (MiniGame)
    on<std::string>("welcome", [this](const std::string& msg) {
        std::cout << "[Coin] Received welcome: " << msg << "\n";
    });
}

void Coin::loop(float delta) {
    // Coin just listens, doesn't emit in this demo
}

// ============================================================================
// SCORE DISPLAY - Demonstrates: subscribing to events from any node
// ============================================================================

void ScoreDisplay::setup() {
    std::cout << "[ScoreDisplay] Ready. Score: " << score << "\n";
    
    // Listen for score events (bubbles up from Player through MiniGame)
    on<ScoreEvent>("score", [this](const ScoreEvent& event) {
        score += event.points;
        std::cout << "[ScoreDisplay] +" << event.points << " points (" 
                  << event.source << "). Total: " << score << "\n";
    });
    
    // Listen for welcome from parent (MiniGame)
    on<std::string>("welcome", [this](const std::string& msg) {
        std::cout << "[ScoreDisplay] Received welcome: " << msg << "\n";
    });
}

// ============================================================================
// MINI GAME - Demonstrates: scene management, event forwarding
// ============================================================================

MiniGame::MiniGame() {
    name = "MiniGame";
}

void MiniGame::setup() {
    std::cout << "[MiniGame] Setting up game entities...\n";
    
    // Create game entities - back to simple one-liners!
    auto player = std::make_unique<Player>();
    player->name = "Player";
    add_child(std::move(player));

    auto coin = std::make_unique<Coin>();
    coin->name = "Coin";
    add_child(std::move(coin));

    // Create UI
    auto score_display = std::make_unique<ScoreDisplay>();
    score_display->name = "ScoreDisplay";
    add_child(std::move(score_display));

    std::cout << "[MiniGame] Game ready!\n\n";
    
    // Forward score events to all children (so ScoreDisplay can hear them)
    on<ScoreEvent>("score", [this](const ScoreEvent& event) {
        std::cout << "[MiniGame] Score event received: +" << event.points 
                  << " (" << event.source << ") - forwarding to children\n";
        emit_to_children<ScoreEvent>("score", event);
    });
    
    // Forward position updates to all children (Coin listens for these)
    on<std::string>("position_update", [this](const std::string& pos) {
        emit_to_children<std::string>("position_update", pos);
    });
    
    // Listen for player jumps
    on<std::string>("player_jump", [this](const std::string& jump_type) {
        std::cout << "[MiniGame] Player jumped: " << jump_type << "\n";
    });
    
    // Broadcast a welcome message to all children
    emit_to_children<std::string>("welcome", "Mini-game started!");
}

void MiniGame::loop(float delta) {
    timer_ += delta;
    
    std::cout << "[MiniGame] Running... (" << timer_ << "s)\n";
    
    // Simulate user pressing "Quit" after 10 seconds (was 5, extended for demo)
    if (timer_ >= 5.0f) {
        std::cout << "[MiniGame] User clicked 'Quit'\n";
        
        // Find SceneManager (parent) and request return to main menu
        if (parent) {
            if (auto* manager = dynamic_cast<SceneManager*>(parent)) {
                manager->return_to_main();
            }
        }
        timer_ = 0.0f;
    }
}

void MiniGame::cleanup() {
    std::cout << "[MiniGame] Cleanup\n";
}
