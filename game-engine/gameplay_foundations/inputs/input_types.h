//
// Created by Antoine Martin on 2026-07-15.
//

#ifndef MYGAMEENGINE_INPUT_TYPES_H
#define MYGAMEENGINE_INPUT_TYPES_H

enum class gamepad_button {
    unknown = 0,
    top_action_button,
    left_action_button,
    right_action_button,
    bottom_action_button
};

template<typename T>
struct vector2 {
    T x;
    T y;
};

#endif //MYGAMEENGINE_INPUT_TYPES_H
