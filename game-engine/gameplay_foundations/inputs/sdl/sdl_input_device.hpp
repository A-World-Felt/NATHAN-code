#ifndef GAME_ENGINE_INPUTS_SDL_INPUT_DEVICE_H_
#define GAME_ENGINE_INPUTS_SDL_INPUT_DEVICE_H_

#include <array>
#include <SDL3/SDL_events.h>
#include "inputs/iinput_device.hpp"
#include "inputs/input_types.hpp"

namespace nathan {

class SDLInputDevice : public IInputDevice {
public:
    explicit SDLInputDevice(EventBus& event_bus);
    void update() override;
    void new_frame() override;

    void handle_event(const SDL_Event &event);

    // Provided functions for polling instead of using the event bus
    bool is_button_pressed(GamepadButton) override;
    bool is_button_released(GamepadButton) override;
    int16_t get_axis_value(GamepadAxis) override;

private:
    static GamepadButton to_engine(SDL_GamepadButton sdl_gb);
    static GamepadAxis to_engine(SDL_GamepadAxis sdl_ga);

    std::array<bool, static_cast<size_t>(GamepadButton::kCount)> current_button_{};
    std::array<bool, static_cast<size_t>(GamepadButton::kCount)> previous_button_{};
    std::array<int16_t, static_cast<size_t>(GamepadAxis::kCount)> axis_{};
};

} // namespace nathan

#endif // GAME_ENGINE_INPUTS_SDL_INPUT_DEVICE_H_
