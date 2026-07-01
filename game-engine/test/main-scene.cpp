#include "main-scene.hpp"

// ---------------- Player ----------------

void Player::setup() {
    std::cout << "Player created at (" << x << ", " << y << ")\n";
}

void Player::loop(float delta) {
    x += speed * delta;
    std::cout << "Player at (" << x << ", " << y << ")\n";
}

// ---------------- Coin ----------------

void Coin::setup() {
    std::cout << "Coin created at (" << x << ", " << y << ")\n";
}

void Coin::loop(float delta) {
    // idle
}

// ---------------- MainScene ----------------

MainScene::MainScene() {
    name = "MainScene";

    auto player = std::make_unique<Player>();
    player->name = "Player";
    add_child(std::move(player));

    auto coin = std::make_unique<Coin>();
    coin->name = "Coin";
    add_child(std::move(coin));
}

void MainScene::setup() {
    std::cout << "MainScene is ready!\n";
}

void MainScene::loop(float delta) {
    for (auto& child : children) {
        child->loop(delta);
    }
}