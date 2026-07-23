//
// Created by Antoine Martin on 2026-07-15.
//

#ifndef MYGAMEENGINE_INPUT_TYPES_H
#define MYGAMEENGINE_INPUT_TYPES_H

enum class gamepad_button {
    invalid_button = -1,
    top_face_button,
    left_face_button,
    right_face_button,
    bottom_face_button,
    up_dpad_button,
    left_dpad_button,
    right_dpad_button,
    down_dpad_button,
    left_trigger_button,
    right_trigger_button,
    left_joystick_button,
    right_joystick_button,
    start_button,
    count
};

enum class gamepad_axis {
    invalid_axis = -1,
    x_left_joystick,
    y_left_joystick,
    x_right_joystick,
    y_right_joystick,
    left_trigger,
    right_trigger,
    count
};

#endif //MYGAMEENGINE_INPUT_TYPES_H
