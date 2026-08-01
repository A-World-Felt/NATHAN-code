#ifndef GAME_ENGINE_DEMO_SCENES_MINI_GAME_H_
#define GAME_ENGINE_DEMO_SCENES_MINI_GAME_H_

#include "node/event_node.hpp"
#include "game_world/node_3d.hpp"
#include <string>
#include <string_view>

namespace nathan {

// Event Types for the demo
struct CollisionEvent {
    std::string with;
    float force;
};

struct ScoreEvent {
    int points;
    std::string source;
};

// Game Entities

class Player : public Node3D {
public:
    float get_axis_x() const { return axis_x_; }
    void set_axis_x(float v) { axis_x_ = v; }
    float get_axis_y() const { return axis_y_; }
    void set_axis_y(float v) { axis_y_ = v; }
    float get_speed() const { return speed_; }
    void set_speed(float speed) { speed_ = speed; }
    int get_coins_collected() const { return coins_collected_; }
    void set_coins_collected(int coins) { coins_collected_ = coins; }
    int get_jump_count() const { return jump_count_; }
    void set_jump_count(int count) { jump_count_ = count; }
    int get_last_position() const { return last_position_; }
    void set_last_position(int pos) { last_position_ = pos; }

    void setup() override;
    void loop(float delta) override;

private:
    float axis_x_ = 0.0f, axis_y_ = 0.0f;
    float speed_ = 15.0f;
    int coins_collected_ = 0;
    int jump_count_ = 0;
    int last_position_ = 0;
    Sound sound_ = Sound{R"(..\resource\ring.mp3)"};
};

class Coin : public Node3D {
public:
    Coin(float x = 0.0f, float y = 0.0f) : Node3D(x, y, 0.0f) {}
    int get_value() const { return value_; }
    void set_value(int value) { value_ = value; }
    bool is_collected() const { return collected_; }
    void set_collected(bool collected) { collected_ = collected; }

    void setup() override;
    void loop(float delta) override;

private:
    int value_ = 10;
    bool collected_ = false;
};

class ScoreDisplay : public EventNode {
public:
    int get_score() const { return score_; }
    void set_score(int score) { score_ = score; }

    void setup() override;
    void loop(float delta) override {};

private:
    int score_ = 0;
};

// MiniGame Scene

class MiniGame : public EventNode {
public:
    MiniGame();

    void setup() override;
    void loop(float delta) override;
    void cleanup() override;

private:
    float timer_ = 0.0f;
};

}  // namespace nathan

#endif  // GAME_ENGINE_DEMO_SCENES_MINI_GAME_H_
