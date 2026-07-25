#ifndef GAME_ENGINE_INPUTS_INPUT_TYPES_H_
#define GAME_ENGINE_INPUTS_INPUT_TYPES_H_

enum class GamepadButton {
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

enum class GamepadAxis {
    kInvalidAxis = -1,
    kXLeftJoystickAxis,
    kYLeftJoystickAxis,
    kXRightJoystickAxis,
    kYRightJoystickAxis,
    kLeftTriggerAxis,
    kRightTriggerAxis,
    kCount
};

#endif // GAME_ENGINE_INPUTS_INPUT_TYPES_H_