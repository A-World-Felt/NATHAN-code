#include "sdl_input_device.hpp"

namespace nathan {

SDLInputDevice::SDLInputDevice() {
    previous_button_ = current_button_;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN) {
            current_button_[event.gbutton.button] = true;
        }
        else if (event.type == SDL_EVENT_GAMEPAD_BUTTON_UP) {
            current_button_[event.key.scancode] = false;
        }
    }
}

void SDLInputDevice::update() {
}

void SDLInputDevice::new_frame() {
    previous_button_ = current_button_;
}

bool SDLInputDevice::is_button_pressed(gamepad_button gbutton) {
    const SDL_GamepadButton sdl_button = to_sdl(gbutton);
    const auto cur = current_button_.find(sdl_button);
    const auto prev = previous_button_.find(sdl_button);

    const bool is_cur_down = (cur!= current_button_.end()) ? cur->second : false;
    const bool is_prev_down = (prev!= previous_button_.end()) ? cur->second : false;
    return is_cur_down && !is_prev_down;
}

bool SDLInputDevice::is_button_released(gamepad_button gbutton) {
    const SDL_GamepadButton sdl_button = to_sdl(gbutton);
    const auto cur = current_button_.find(sdl_button);
    const auto prev = previous_button_.find(sdl_button);

    const bool is_cur_down = (cur!= current_button_.end()) ? cur->second : false;
    const bool is_prev_down = (prev!= previous_button_.end()) ? cur->second : false;
    return !is_cur_down && is_prev_down;
}

void SDLInputDevice::handle_event(SDL_Event &event) {
    if (event.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN) {
        current_button_[event.gbutton.button] = true;
    }
    else if (event.type == SDL_EVENT_GAMEPAD_BUTTON_UP) {
        current_button_[event.key.scancode] = false;
    }
}

SDL_GamepadButton SDLInputDevice::to_sdl(const gamepad_button key) {
    switch (key) {
        case gamepad_button::bottom_action_button: return SDL_GAMEPAD_BUTTON_SOUTH;
        case gamepad_button::left_action_button: return SDL_GAMEPAD_BUTTON_EAST;
        case gamepad_button::right_action_button: return SDL_GAMEPAD_BUTTON_WEST;
        case gamepad_button::top_action_button: return SDL_GAMEPAD_BUTTON_NORTH;
        default: return SDL_GAMEPAD_BUTTON_INVALID;
    }
}

gamepad_button SDLInputDevice::to_engine(Uint8 key) const {
    return gamepad_button::unknown;
}

} // nathan