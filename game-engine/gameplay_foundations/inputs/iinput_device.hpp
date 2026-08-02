#ifndef GAME_ENGINE_INPUTS_IINPUT_DEVICE_H_
#define GAME_ENGINE_INPUTS_IINPUT_DEVICE_H_

#include "events/event_bus.hpp"
#include "input_types.hpp"

namespace nathan {

class IInputDevice {
public:
    virtual ~IInputDevice() = default;

    virtual void update() = 0;
    virtual void new_frame() = 0;

    virtual bool is_button_pressed(GamepadButton) = 0;
    virtual bool is_button_released(GamepadButton) = 0;
    virtual float get_axis_value(GamepadAxis) = 0; // [-1.0, 1.0]

protected:
    explicit IInputDevice(EventBus& event_bus): event_bus_(event_bus) {}
    EventBus& event_bus_;
};

} // namespace nathan

#endif // GAME_ENGINE_INPUTS_IINPUT_DEVICE_H_
