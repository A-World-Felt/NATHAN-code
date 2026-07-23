#include "sdl_input_device.hpp"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>

namespace nathan {

SDLInputDevice::SDLInputDevice() {
    if (!SDL_Init(SDL_INIT_GAMEPAD)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        exit(-1);
    }
    SDL_Log("SDL_Init successfully");

    int count = 0;
    SDL_JoystickID* devices = SDL_GetGamepads(&count);
    for (int i = 0; i < count; i++) {
        SDL_Gamepad* gamepad = SDL_OpenGamepad(devices[i]);
        SDL_Log("%s", SDL_GetGamepadName(gamepad));
        gamepads_.push_back(gamepad);
    }
    SDL_free(devices);
}

void SDLInputDevice::update() {
    new_frame();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        handle_event(event);
    }
}

void SDLInputDevice::new_frame() {
    previous_button_ = current_button_;
}

bool SDLInputDevice::is_button_pressed(const gamepad_button gb) {
    const auto button_id = static_cast<size_t>(gb);
    const auto is_cur_down = current_button_.at(button_id);
    const auto is_prev_down = previous_button_.at(button_id);

    return is_cur_down && !is_prev_down;
}

bool SDLInputDevice::is_button_released(const gamepad_button gb) {
    const auto button_id = static_cast<size_t>(gb);
    const auto is_cur_down = current_button_.at(button_id);
    const auto is_prev_down = previous_button_.at(button_id);

    return !is_cur_down && is_prev_down;
}

int16_t SDLInputDevice::get_axis_value(gamepad_axis ga) {
    const auto axis_id = static_cast<size_t>(ga);
    return axis_.at(axis_id);
}

void SDLInputDevice::handle_event(const SDL_Event &event) {
    if (event.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN) {
        current_button_.at(static_cast<uint8_t>(to_engine(static_cast<SDL_GamepadButton>(event.gbutton.button)))) = true;
    }
    else if (event.type == SDL_EVENT_GAMEPAD_BUTTON_UP) {
        current_button_.at(static_cast<uint8_t>(to_engine(static_cast<SDL_GamepadButton>(event.gbutton.button)))) = false;
    }
    else if (event.type == SDL_EVENT_GAMEPAD_AXIS_MOTION) {
        axis_.at(static_cast<uint8_t>(to_engine(static_cast<SDL_GamepadAxis>(event.gaxis.axis)))) = event.gaxis.value;
    }
}

gamepad_button SDLInputDevice::to_engine(const SDL_GamepadButton sdl_gb) {
    switch (sdl_gb) {
        case SDL_GAMEPAD_BUTTON_NORTH:          return gamepad_button::top_face_button;
        case SDL_GAMEPAD_BUTTON_WEST:           return gamepad_button::left_face_button;
        case SDL_GAMEPAD_BUTTON_EAST:           return gamepad_button::right_face_button;
        case SDL_GAMEPAD_BUTTON_SOUTH:          return gamepad_button::bottom_face_button;
        case SDL_GAMEPAD_BUTTON_DPAD_UP:        return gamepad_button::up_dpad_button;
        case SDL_GAMEPAD_BUTTON_DPAD_LEFT:      return gamepad_button::left_dpad_button;
        case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:     return gamepad_button::right_dpad_button;
        case SDL_GAMEPAD_BUTTON_DPAD_DOWN:      return gamepad_button::down_dpad_button;
        case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER:  return gamepad_button::left_trigger_button;
        case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER: return gamepad_button::right_trigger_button;
        case SDL_GAMEPAD_BUTTON_LEFT_STICK:     return gamepad_button::left_joystick_button;
        case SDL_GAMEPAD_BUTTON_RIGHT_STICK:    return gamepad_button::right_joystick_button;
        case SDL_GAMEPAD_BUTTON_START:          return gamepad_button::start_button;
        default: return gamepad_button::invalid_button;
    }
}

gamepad_axis SDLInputDevice::to_engine(const SDL_GamepadAxis sdl_ga) {
    switch (sdl_ga) {
        case SDL_GAMEPAD_AXIS_LEFTX:            return gamepad_axis::x_left_joystick;
        case SDL_GAMEPAD_AXIS_LEFTY:            return gamepad_axis::y_left_joystick;
        case SDL_GAMEPAD_AXIS_RIGHTX:           return gamepad_axis::x_right_joystick;
        case SDL_GAMEPAD_AXIS_RIGHTY:           return gamepad_axis::y_right_joystick;
        case SDL_GAMEPAD_AXIS_LEFT_TRIGGER:     return gamepad_axis::left_trigger;
        case SDL_GAMEPAD_AXIS_RIGHT_TRIGGER:    return gamepad_axis::right_trigger;
        default: return gamepad_axis::invalid_axis;
    }
}

} // nathan