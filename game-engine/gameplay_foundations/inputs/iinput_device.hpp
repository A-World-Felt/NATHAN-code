#ifndef GAME_ENGINE_INPUTS_IINPUT_DEVICE_H_
#define GAME_ENGINE_INPUTS_IINPUT_DEVICE_H_

#include "input_types.hpp"

namespace nathan {

class IInputDevice {
public:
    virtual ~IInputDevice() = default;

    virtual void update() = 0;
    virtual void new_frame() = 0;

    virtual bool is_button_pressed(GamepadButton) = 0;
    virtual bool is_button_released(GamepadButton) = 0;
    virtual int16_t get_axis_value(GamepadAxis) = 0;
};

} // namespace nathan

#endif // GAME_ENGINE_INPUTS_IINPUT_DEVICE_H_
