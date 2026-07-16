#ifndef IInput_hpp
#define IInput_hpp
#include "input_types.h"

namespace nathan {

class IInputDevice {
public:
    virtual ~IInputDevice() = default;
    virtual void update() = 0;

    virtual void new_frame() = 0;

    virtual bool is_button_pressed(gamepad_button) = 0;
    virtual bool is_button_released(gamepad_button) = 0;
};

}

#endif

// vector2 left_joystick;
// vector2 right_joystick;
// bool left_joystick_pressed;
// bool right_joystick_pressed;
//
// float left_bumper_pressed;
// float right_bumper_pressed;
//
// bool left_trigger_pressed;
// bool right_trigger_pressed;
//
// bool main_button_pressed;
// bool top_action_button_pressed;
// bool bottom_action_button_pressed;
// bool left_action_button_pressed;
// bool right_action_button_pressed;
//
// bool dpad_up_pressed;
// bool dpad_down_pressed;
// bool dpad_left_pressed;
// bool dpad_right_pressed;
