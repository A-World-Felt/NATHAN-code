#pragma once

#include "node.hpp"
#include <iostream>
#include <memory>

// Forward declarations are NOT needed here because we define everything in same header
// (but could be used if you split further later)

class Player : public Node {
public:
    float x = 0, y = 0;
    float speed = 1.0f;

    void setup() override;
    void loop(float delta) override;
};

class Coin : public Node {
public:
    float x = 50, y = 50;

    void setup() override;
    void loop(float delta) override;
};

class MainScene : public Node {
public:
    MainScene();

    void setup() override;
    void loop(float delta) override;
};