#ifndef GAME_ENGINE_AUDIO_SOUND_H_
#define GAME_ENGINE_AUDIO_SOUND_H_

#include <atomic>
#include <cstdint>
#include <filesystem>

#include "math/vector3.hpp"

namespace nathan {

class Sound {
public:
    explicit Sound(const std::string_view path_to_file)
        : id_(get_next_id()), path_to_file_(path_to_file), rel_position_({.x = 0.0f,.y = 0.0f,.z = 0.0f}), gain_(1.0f), reverb_(1.0f) {}

    uint64_t get_id() const { return id_; }

    Vector3 get_rel_position() const { return rel_position_; }
    void set_rel_position(const Vector3 new_pos) { rel_position_ = new_pos; }

    float get_gain() const { return gain_; }
    void set_gain(const float gain) { gain_ = gain; }

    float get_reverb() const { return reverb_; }
    void set_reverb(const float reverb) { reverb_ = reverb; }

private:
    static uint64_t get_next_id() {
        static std::atomic<uint64_t> next_id{1};
        return next_id++;
    }

    uint64_t id_;
    std::filesystem::path path_to_file_;
    Vector3 rel_position_;
    float gain_;
    float reverb_;
};

} // namespace nathan

#endif // GAME_ENGINE_AUDIO_SOUND_H_
