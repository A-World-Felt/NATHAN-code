#ifndef MYGAMEENGINE_SDLINPUTDEVICE_H
#define MYGAMEENGINE_SDLINPUTDEVICE_H

#include "inputs/iinput_device.hpp"
#include "inputs/input_types.h"
#include <SDL3/SDL_scancode.h>
#include <unordered_map>

#include "SDL3/SDL_events.h"

namespace nathan {

class SDLInputDevice : public IInputDevice {
public:
    SDLInputDevice();
    void update() override;

    void new_frame() override;

    bool is_button_pressed(gamepad_button) override;
    bool is_button_released(gamepad_button) override;

    void handle_event(SDL_Event &event);
private:
    static SDL_GamepadButton to_sdl(gamepad_button key);
    gamepad_button to_engine(Uint8 key) const;

    std::unordered_map<Uint8, bool> current_button_;
    std::unordered_map<Uint8, bool> previous_button_;
};

} // sdl

#endif //MYGAMEENGINE_SDLINPUTDEVICE_H
