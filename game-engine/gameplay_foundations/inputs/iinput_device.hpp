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
    virtual int16_t get_axis_value(gamepad_axis) = 0;
};

}

#endif
