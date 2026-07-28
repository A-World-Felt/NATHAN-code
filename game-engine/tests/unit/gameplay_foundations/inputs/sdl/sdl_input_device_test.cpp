// Unit tests for SDLInputDevice using Google Test and Google Mock

#include "inputs/sdl/sdl_input_device.hpp"
#include "events/event_bus.hpp"

#include <SDL3/SDL_gamepad.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdexcept>

namespace nathan {
namespace {

class SDLInputDeviceTest : public ::testing::Test {
public:
    SDLInputDeviceTest() : device_(event_bus_) {}

protected:
    // Builds a minimal SDL_Event for a gamepad button transition.
    static SDL_Event MakeButtonEvent(const SDL_EventType type, const SDL_GamepadButton button) {
        SDL_Event event{};
        event.type = static_cast<Uint32>(type);
        event.gbutton.type = type;
        event.gbutton.button = button;
        return event;
    }

    // Builds a minimal SDL_Event for a gamepad axis motion.
    static SDL_Event MakeAxisEvent(const SDL_GamepadAxis axis, const int16_t value) {
        SDL_Event event{};
        event.type = SDL_EVENT_GAMEPAD_AXIS_MOTION;
        event.gaxis.type = SDL_EVENT_GAMEPAD_AXIS_MOTION;
        event.gaxis.axis = axis;
        event.gaxis.value = value;
        return event;
    }

    EventBus event_bus_;
    SDLInputDevice device_;
};

// ---------------------------------------------------------------------
// SDL_GamepadButton / SDL_GamepadAxis -> engine enum mapping
//
// to_engine() is private, so these confirm the mapping through the
// "button_pressed"/"axis_value_changed" events that handle_event() emits,
// rather than calling to_engine() directly.
// ---------------------------------------------------------------------

TEST_F(SDLInputDeviceTest, NorthButtonMapsCorrectly) {
    GamepadButton received = GamepadButton::kInvalidButton;
    event_bus_.on_global<GamepadButton>("button_pressed",
        [&](const GamepadButton& button) { received = button; });

    device_.handle_event(MakeButtonEvent(SDL_EVENT_GAMEPAD_BUTTON_DOWN, SDL_GAMEPAD_BUTTON_NORTH));

    EXPECT_EQ(received, GamepadButton::kTopFaceButton);
}

TEST_F(SDLInputDeviceTest, InvalidButtonMapsCorrectly) {
    GamepadButton received = GamepadButton::kTopFaceButton;  // sentinel, overwritten if the callback fires
    event_bus_.on_global<GamepadButton>("button_pressed",
        [&](const GamepadButton& button) { received = button; });

    // SDL_GAMEPAD_BUTTON_BACK isn't handled in to_engine()'s switch, so it
    // maps to kInvalidButton (-1). The event still fires with that value,
    // but the subsequent current_button_.at(static_cast<uint8_t>(-1))
    // (i.e. .at(255)) is out of range for the 13-element array, so
    // handle_event() throws std::out_of_range. That's existing behavior in
    // the production code, not something introduced by this test - flagging
    // in case it's not intentional.
    EXPECT_THROW(
        device_.handle_event(MakeButtonEvent(SDL_EVENT_GAMEPAD_BUTTON_DOWN, SDL_GAMEPAD_BUTTON_BACK)),
        std::out_of_range);

    EXPECT_EQ(received, GamepadButton::kInvalidButton);
}

TEST_F(SDLInputDeviceTest, LeftXAxisMapsCorrectly) {
    std::pair<GamepadAxis, float> received{GamepadAxis::kInvalidAxis, 0.0f};
    event_bus_.on_global<std::pair<GamepadAxis, float>>("axis_value_changed",
        [&](const std::pair<GamepadAxis, float>& payload) { received = payload; });

    device_.handle_event(MakeAxisEvent(SDL_GAMEPAD_AXIS_LEFTX, 16384));

    EXPECT_EQ(received.first, GamepadAxis::kXLeftJoystickAxis);
}

// ---------------------------------------------------------------------
// Frame bookkeeping / edge-detection logic
// ---------------------------------------------------------------------

TEST_F(SDLInputDeviceTest, NewFrameCopiesCurrentState) {
    // Fresh device: current_button_ and previous_button_ both start false,
    // so pressing START creates a current != previous mismatch.
    device_.handle_event(MakeButtonEvent(SDL_EVENT_GAMEPAD_BUTTON_DOWN, SDL_GAMEPAD_BUTTON_START));
    ASSERT_TRUE(device_.is_button_pressed(GamepadButton::kStartButton));

    device_.new_frame();

    // previous_button_ now matches current_button_, so the "just pressed"
    // edge (current && !previous) is gone even though the button is still
    // held down - this only happens if new_frame() actually copied the
    // state.
    EXPECT_FALSE(device_.is_button_pressed(GamepadButton::kStartButton));
    EXPECT_FALSE(device_.is_button_released(GamepadButton::kStartButton));
}

TEST_F(SDLInputDeviceTest, ButtonPressedReturnsTrue) {
    device_.handle_event(MakeButtonEvent(SDL_EVENT_GAMEPAD_BUTTON_DOWN, SDL_GAMEPAD_BUTTON_SOUTH));

    EXPECT_TRUE(device_.is_button_pressed(GamepadButton::kBottomFaceButton));
}

TEST_F(SDLInputDeviceTest, ButtonReleasedReturnsTrue) {
    device_.handle_event(MakeButtonEvent(SDL_EVENT_GAMEPAD_BUTTON_DOWN, SDL_GAMEPAD_BUTTON_SOUTH));
    device_.new_frame();  // previous_button_ becomes true for SOUTH
    device_.handle_event(MakeButtonEvent(SDL_EVENT_GAMEPAD_BUTTON_UP, SDL_GAMEPAD_BUTTON_SOUTH));

    EXPECT_TRUE(device_.is_button_released(GamepadButton::kBottomFaceButton));
}

TEST_F(SDLInputDeviceTest, AxisValueReturnedCorrectly) {
    device_.handle_event(MakeAxisEvent(SDL_GAMEPAD_AXIS_LEFTX, 12345));

    EXPECT_EQ(device_.get_axis_value(GamepadAxis::kXLeftJoystickAxis), 12345);
}

// ---------------------------------------------------------------------
// handle_event() state updates
//
// These necessarily overlap with the is_button_pressed/released tests
// above, since without access to the private arrays there's no way to
// observe state changes except through those same accessors.
// ---------------------------------------------------------------------

TEST_F(SDLInputDeviceTest, HandleButtonDownUpdatesState) {
    device_.handle_event(MakeButtonEvent(SDL_EVENT_GAMEPAD_BUTTON_DOWN, SDL_GAMEPAD_BUTTON_SOUTH));

    EXPECT_TRUE(device_.is_button_pressed(GamepadButton::kBottomFaceButton));
    EXPECT_FALSE(device_.is_button_released(GamepadButton::kBottomFaceButton));
}

TEST_F(SDLInputDeviceTest, HandleButtonUpUpdatesState) {
    device_.handle_event(MakeButtonEvent(SDL_EVENT_GAMEPAD_BUTTON_DOWN, SDL_GAMEPAD_BUTTON_SOUTH));
    device_.new_frame();
    device_.handle_event(MakeButtonEvent(SDL_EVENT_GAMEPAD_BUTTON_UP, SDL_GAMEPAD_BUTTON_SOUTH));

    EXPECT_TRUE(device_.is_button_released(GamepadButton::kBottomFaceButton));
    EXPECT_FALSE(device_.is_button_pressed(GamepadButton::kBottomFaceButton));
}

TEST_F(SDLInputDeviceTest, HandleAxisMotionUpdatesAxis) {
    device_.handle_event(MakeAxisEvent(SDL_GAMEPAD_AXIS_RIGHTY, -20000));

    EXPECT_EQ(device_.get_axis_value(GamepadAxis::kYRightJoystickAxis), -20000);
}

// ---------------------------------------------------------------------
// handle_event() event emission
// ---------------------------------------------------------------------

TEST_F(SDLInputDeviceTest, ButtonDownEmitsEvent) {
    GamepadButton received = GamepadButton::kInvalidButton;
    bool called = false;
    event_bus_.on_global<GamepadButton>("button_pressed",
        [&](const GamepadButton& button) {
            called = true;
            received = button;
        });

    device_.handle_event(MakeButtonEvent(SDL_EVENT_GAMEPAD_BUTTON_DOWN, SDL_GAMEPAD_BUTTON_SOUTH));

    EXPECT_TRUE(called);
    EXPECT_EQ(received, GamepadButton::kBottomFaceButton);
}

TEST_F(SDLInputDeviceTest, ButtonUpEmitsEvent) {
    GamepadButton received = GamepadButton::kInvalidButton;
    bool called = false;
    event_bus_.on_global<GamepadButton>("button_released",
        [&](const GamepadButton& button) {
            called = true;
            received = button;
        });

    device_.handle_event(MakeButtonEvent(SDL_EVENT_GAMEPAD_BUTTON_UP, SDL_GAMEPAD_BUTTON_SOUTH));

    EXPECT_TRUE(called);
    EXPECT_EQ(received, GamepadButton::kBottomFaceButton);
}

TEST_F(SDLInputDeviceTest, AxisMotionEmitsEvent) {
    std::pair<GamepadAxis, float> received{GamepadAxis::kInvalidAxis, 0.0f};
    bool called = false;
    event_bus_.on_global<std::pair<GamepadAxis, float>>("axis_value_changed",
        [&](const std::pair<GamepadAxis, float>& payload) {
            called = true;
            received = payload;
        });

    device_.handle_event(MakeAxisEvent(SDL_GAMEPAD_AXIS_LEFTX, 16384));

    EXPECT_TRUE(called);
    EXPECT_EQ(received.first, GamepadAxis::kXLeftJoystickAxis);
    EXPECT_FLOAT_EQ(received.second, 16384.0f / SDL_MAX_SINT16);
}

}  // namespace
}  // namespace nathan