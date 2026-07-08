#pragma once

#include "node/event_node.hpp"
#include <iostream>
#include <memory>

// --- Event Types for the demo ---
struct CollisionEvent {
    std::string with;
    float force;
};

struct ScoreEvent {
    int points;
    std::string source;
};

// Forward declarations are NOT needed here because we define everything in same header
// (but could be used if you split further later)

class Player : public EventNode {
public:
    float x = 0, y = 0;
    float speed = 15.0f; // Moderate speed for demo
    int coins_collected = 0;
    int jump_count = 0;
    int last_position = 0;

    void setup() override;
    void loop(float delta) override;
};

class Coin : public EventNode {
public:
    float x = 50, y = 50;
    int value = 10;
    bool collected = false;

    void setup() override;
    void loop(float delta) override;
};

class ScoreDisplay : public EventNode {
public:
    int score = 0;

    void setup() override;
    void loop(float delta) override {}
};

class MainScene : public EventNode {
public:
    MainScene();

    void setup() override;
    // loop() no longer needed - handled automatically by Engine
};