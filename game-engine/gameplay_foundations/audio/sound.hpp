#ifndef GAME_ENGINE_AUDIO_SOUND_H_
#define GAME_ENGINE_AUDIO_SOUND_H_

#include "event_bus.hpp"

#include <cstdint>
#include <filesystem>
#include <queue>

namespace nathan {

class Sound {
public:
    Sound(const std::string_view path_to_file, const float rel_x, const float rel_y, const float rel_z, const float gain)
        : id_(get_next_id()), path_to_file_(path_to_file), rel_x_(rel_x), rel_y_(rel_y), rel_z_(rel_z), gain_(gain) {}

    uint16_t get_id() const { return id_; }

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
    float gain_ = 1;
};

class SoundScape {
public:
    explicit SoundScape(EventBus& event_bus);

private:
    EventBus& event_bus_;
    std::queue<Sound> begin_sounds_{};
    std::queue<uint16_t> end_sounds_{};
};

inline SoundScape::SoundScape(EventBus &event_bus) : event_bus_(event_bus) {
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
