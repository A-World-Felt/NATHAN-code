#ifndef GAME_ENGINE_INPUTS_INPUT_TYPES_H_
#define GAME_ENGINE_INPUTS_INPUT_TYPES_H_

#include <string_view>

enum class GamepadButton : int8_t {
    kInvalidButton = -1,
    kTopFaceButton,
    kLeftFaceButton,
    kRightFaceButton,
    kBottomFaceButton,
    kUpDPadButton,
    kLeftDPadButton,
    kRightDPadButton,
    kDownDPadButton,
    kLeftBumperButton,
    kRightBumperButton,
    kLeftJoystickButton,
    kRightJoystickButton,
    kStartButton,
    kCount
};

enum class GamepadAxis : int8_t {
    kInvalidAxis = -1,
    kXLeftJoystickAxis,
    kYLeftJoystickAxis,
    kXRightJoystickAxis,
    kYRightJoystickAxis,
    kLeftTriggerAxis,
    kRightTriggerAxis,
    kCount
};

namespace sdl_events {

// SDL input event types
inline constexpr std::string_view button_pressed = "button_pressed";
inline constexpr std::string_view button_released = "button_released";
inline constexpr std::string_view axis_value_changed = "axis_value_changed";

} // namespace sdl_events

#endif // GAME_ENGINE_INPUTS_INPUT_TYPES_H_