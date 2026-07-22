#ifndef MYGAMEENGINE_SDLINPUTDEVICE_H
#define MYGAMEENGINE_SDLINPUTDEVICE_H

#include <array>
#include <vector>
#include <SDL3/SDL_events.h>
#include "inputs/iinput_device.hpp"
#include "inputs/input_types.h"

namespace nathan {

class SDLInputDevice : public IInputDevice {
public:
    SDLInputDevice();
    void update() override;

    void new_frame() override;

    bool is_button_pressed(gamepad_button) override;
    bool is_button_released(gamepad_button) override;

    void handle_event(const SDL_Event &event);
private:
    static SDL_GamepadButton to_sdl(gamepad_button gb);
    static gamepad_button to_engine(SDL_GamepadButton sdl_gb);
    static gamepad_button to_engine(Uint8 sdl_gb);

    std::vector<SDL_Gamepad*> gamepads_;
    std::array<bool, static_cast<size_t>(gamepad_button::count)> current_button_{};
    std::array<bool, static_cast<size_t>(gamepad_button::count)> previous_button_{};
};

} // sdl

#endif //MYGAMEENGINE_SDLINPUTDEVICE_H
