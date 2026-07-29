#ifndef GAME_ENGINE_AUDIO_SOUND_H_
#define GAME_ENGINE_AUDIO_SOUND_H_

#include "event_bus.hpp"

#include <cstdint>
#include <filesystem>
#include <queue>

namespace nathan {

class Sound {
public:
    Sound(const std::string_view path_to_file, const float rel_x, const float rel_y, const float rel_z, const float gain = 1.0f)
        : id_(get_next_id()), path_to_file_(path_to_file), rel_x_(rel_x), rel_y_(rel_y), rel_z_(rel_z), gain_(gain) {}

    void update_position(const float rel_x, const float rel_y, const float rel_z) {
        rel_x_ = rel_x;
        rel_y_ = rel_y;
        rel_z_ = rel_z;
    };

    uint64_t get_id() const { return id_; }

private:
    static uint64_t get_next_id() {
        static std::atomic<uint64_t> next_id{1};
        return next_id++;
    }

    uint64_t id_;
    std::filesystem::path path_to_file_;
    // Relative position needs to be updated continuously if the player moves
    float rel_x_;
    float rel_y_;
    float rel_z_;
    float gain_;
};

class SoundScape {
public:
    SoundScape(EventBus &event_bus, Node *head_node);

    void set_head_node (Node& node) {
        head_node_ = &node;
    }

private:
    EventBus& event_bus_;
    Node* head_node_;
    std::queue<Sound> begin_sounds_{};
    std::queue<uint16_t> end_sounds_{};
    std::queue<Sound> sounds_{};
};

inline SoundScape::SoundScape(EventBus &event_bus, Node* head_node = nullptr) : event_bus_(event_bus), head_node_(head_node) {
    event_bus.on_global<Sound>("play sound", [this](const Sound &sound) {
        // Lock mutex for interprocess communication
        begin_sounds_.push(sound);
    });

    event_bus.on_global<Sound>("stop sound", [this](const Sound &sound) {
        // Lock mutex for interprocess communication
        end_sounds_.push(sound.get_id());
    });
}

} // namespace nathan

#endif // GAME_ENGINE_AUDIO_SOUND_H_
