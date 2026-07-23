#ifndef MYGAMEENGINE_SOUND_H
#define MYGAMEENGINE_SOUND_H

#include <cstdint>
#include <vector>
#include <filesystem>

namespace nathan {
struct Sound {
    uint8_t id_;
    std::filesystem::path path_to_file_;
    float rel_x_;
    float rel_y_;
    float rel_z_;
    float gain = 1;
};

struct SoundScape {
    std::vector<Sound> begin_sounds_;
    std::vector<uint8_t> end_sounds_;
};

}

#endif //MYGAMEENGINE_SOUND_H
