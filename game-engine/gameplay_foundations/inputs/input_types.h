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

template<typename T>
struct vector2 {
    T x;
    T y;
};

#endif //MYGAMEENGINE_INPUT_TYPES_H
