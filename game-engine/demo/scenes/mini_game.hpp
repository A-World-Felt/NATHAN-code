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

// --- Game Entities ---

class Player : public EventNode {
public:
    float x = 0, y = 0;
    float speed = 15.0f;
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

// --- MiniGame Scene ---

class MiniGame : public EventNode {
public:
    MiniGame();

    void setup() override;
    void loop(float delta) override;
    void cleanup() override;

private:
    float timer_ = 0.0f;
};
