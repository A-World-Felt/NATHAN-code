#include "sdl_input_device.hpp"

#include <cstdlib>
#include <iostream>
#include <SDL3/SDL_init.h>

#include "event_types.hpp"

namespace nathan {

SDLInputDevice::SDLInputDevice(EventBus& event_bus) : IInputDevice(event_bus) {
    std::cout << "[InputDevice] Initializing..." << std::endl;
    if (!SDL_Init(SDL_INIT_GAMEPAD)) {
        std::cerr << "[InputDevice] Initialization failed: " << SDL_GetError() << std::endl;
        exit(-1);
    }

    int count = 0;
    SDL_JoystickID *devices = SDL_GetGamepads(&count);
    if (count > 0) {
        gamepad = SDL_OpenGamepad(devices[0]);
        std::cout << "[InputDevice] Gamepad detected: " << SDL_GetGamepadName(gamepad) << std::endl;
    }
    else
        std::cout << "[InputDevice] Gamepad not detected" << std::endl;

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

bool SDLInputDevice::is_button_pressed(const GamepadButton gb) {
    const auto button_id = static_cast<size_t>(gb);
    const auto is_cur_down = current_button_.at(button_id);
    const auto is_prev_down = previous_button_.at(button_id);

    return is_cur_down && !is_prev_down;
}

bool SDLInputDevice::is_button_released(const GamepadButton gb) {
    const auto button_id = static_cast<size_t>(gb);
    const auto is_cur_down = current_button_.at(button_id);
    const auto is_prev_down = previous_button_.at(button_id);

    return !is_cur_down && is_prev_down;
}

float SDLInputDevice::get_axis_value(GamepadAxis ga) {
    const auto axis_id = static_cast<size_t>(ga);
    return axis_.at(axis_id);
}

void SDLInputDevice::handle_event(const SDL_Event &event) {
    if (event.type == SDL_EVENT_GAMEPAD_ADDED) {
        if (!gamepad && SDL_IsGamepad(event.gdevice.which)) {
            gamepad = SDL_OpenGamepad(event.gdevice.which);
            std::cout << "[InputDevice] Gamepad added: " << SDL_GetGamepadName(gamepad) << std::endl;
        }
    }
    else if (event.type == SDL_EVENT_GAMEPAD_REMOVED) {
        if (gamepad) {
            std::cout << "[InputDevice] Gamepad removed: " << SDL_GetGamepadName(gamepad) << std::endl;
            SDL_CloseGamepad(gamepad);
            gamepad = nullptr;
        }
    }
    else if (event.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN) {
        GamepadButton gamepad_button = to_engine(static_cast<SDL_GamepadButton>(event.gbutton.button));
        event_bus_.emit_global<GamepadButtonEvent>(
            "button_pressed", { .gamepad_button = gamepad_button, .action = GamepadButtonEvent::Action::kPressed }
        );
        current_button_.at(static_cast<uint8_t>(gamepad_button)) = true;
    }
    else if (event.type == SDL_EVENT_GAMEPAD_BUTTON_UP) {
        GamepadButton gamepad_button = to_engine(static_cast<SDL_GamepadButton>(event.gbutton.button));
        event_bus_.emit_global<GamepadButtonEvent>(
            "button_released", {.gamepad_button = gamepad_button, .action = GamepadButtonEvent::Action::kReleased}
        );
        current_button_.at(static_cast<uint8_t>(gamepad_button)) = false;
    }
    else if (event.type == SDL_EVENT_GAMEPAD_AXIS_MOTION) {
        GamepadAxis gamepad_axis = to_engine(static_cast<SDL_GamepadAxis>(event.gaxis.axis));
        const auto val = static_cast<float>(event.gaxis.value) / SDL_MAX_SINT16;
        event_bus_.emit_global<GamepadAxisEvent>(
            "axis_value_changed", {.gamepad_axis = gamepad_axis, .val = val}
        );
        axis_.at(static_cast<uint8_t>(gamepad_axis)) = val;
    }
}

GamepadButton SDLInputDevice::to_engine(const SDL_GamepadButton sdl_gb) {
    switch (sdl_gb) {
        case SDL_GAMEPAD_BUTTON_NORTH:          return GamepadButton::kTopFaceButton;
        case SDL_GAMEPAD_BUTTON_WEST:           return GamepadButton::kLeftFaceButton;
        case SDL_GAMEPAD_BUTTON_EAST:           return GamepadButton::kRightFaceButton;
        case SDL_GAMEPAD_BUTTON_SOUTH:          return GamepadButton::kBottomFaceButton;
        case SDL_GAMEPAD_BUTTON_DPAD_UP:        return GamepadButton::kUpDPadButton;
        case SDL_GAMEPAD_BUTTON_DPAD_LEFT:      return GamepadButton::kLeftDPadButton;
        case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:     return GamepadButton::kRightDPadButton;
        case SDL_GAMEPAD_BUTTON_DPAD_DOWN:      return GamepadButton::kDownDPadButton;
        case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER:  return GamepadButton::kLeftBumperButton;
        case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER: return GamepadButton::kRightBumperButton;
        case SDL_GAMEPAD_BUTTON_LEFT_STICK:     return GamepadButton::kLeftJoystickButton;
        case SDL_GAMEPAD_BUTTON_RIGHT_STICK:    return GamepadButton::kRightJoystickButton;
        case SDL_GAMEPAD_BUTTON_START:          return GamepadButton::kStartButton;
        default: return GamepadButton::kInvalidButton;
    }
}

GamepadAxis SDLInputDevice::to_engine(const SDL_GamepadAxis sdl_ga) {
    switch (sdl_ga) {
        case SDL_GAMEPAD_AXIS_LEFTX:            return GamepadAxis::kXLeftJoystickAxis;
        case SDL_GAMEPAD_AXIS_LEFTY:            return GamepadAxis::kYLeftJoystickAxis;
        case SDL_GAMEPAD_AXIS_RIGHTX:           return GamepadAxis::kXRightJoystickAxis;
        case SDL_GAMEPAD_AXIS_RIGHTY:           return GamepadAxis::kYRightJoystickAxis;
        case SDL_GAMEPAD_AXIS_LEFT_TRIGGER:     return GamepadAxis::kLeftTriggerAxis;
        case SDL_GAMEPAD_AXIS_RIGHT_TRIGGER:    return GamepadAxis::kRightTriggerAxis;
        default: return GamepadAxis::kInvalidAxis;
    }
}

} // namespace nathan