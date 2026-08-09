// Unit tests for SDLInputDevice using Google Test and Google Mock

#include "inputs/sdl/sdl_input_device.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <SDL3/SDL_gamepad.h>

#include "stdexcept"
#include "events/event_bus.hpp"
#include "events/event_types.hpp"

namespace nathan {
namespace {

class SDLInputDeviceTest : public ::testing::Test {
public:
    SDLInputDeviceTest() : device_(event_bus_) {}

protected:
    // Builds a minimal SDL_Event for a gamepad button transition.
    static SDL_Event MakeButtonEvent(const SDL_EventType type, const SDL_GamepadButton button) {
        SDL_Event event{};
        event.type = type;
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

    // Builds a minimal SDL_Event for a gamepad device add/remove.
    static SDL_Event MakeDeviceEvent(const SDL_EventType type, const SDL_JoystickID which) {
        SDL_Event event{};
        event.type = type;
        event.gdevice.type = type;
        event.gdevice.which = which;
        return event;
    }

    EventBus event_bus_;
    SDLInputDevice device_;
};

}  // namespace

// ---------------------------------------------------------------------
// SDL_GamepadButton / SDL_GamepadAxis -> engine enum mapping
// ---------------------------------------------------------------------

TEST_F(SDLInputDeviceTest, NorthButtonMapsCorrectly) {
    GamepadButton received = GamepadButton::kInvalidButton;
    event_bus_.on_global<GamepadButtonEvent>("button_pressed",
        [&](const GamepadButtonEvent& event) { received = event.gamepad_button; });

    device_.handle_event(MakeButtonEvent(SDL_EVENT_GAMEPAD_BUTTON_DOWN, SDL_GAMEPAD_BUTTON_NORTH));

    EXPECT_EQ(received, GamepadButton::kTopFaceButton);
}

TEST_F(SDLInputDeviceTest, InvalidButtonMapsCorrectly) {
    GamepadButton received = GamepadButton::kTopFaceButton;  // sentinel, overwritten if the callback fires
    event_bus_.on_global<GamepadButtonEvent>("button_pressed",
        [&](const GamepadButtonEvent& event) { received = event.gamepad_button; });

    device_.handle_event(MakeButtonEvent(SDL_EVENT_GAMEPAD_BUTTON_DOWN, SDL_GAMEPAD_BUTTON_BACK));

    EXPECT_EQ(received, GamepadButton::kInvalidButton);
}

TEST_F(SDLInputDeviceTest, LeftXAxisMapsCorrectly) {
    GamepadAxis received = GamepadAxis::kInvalidAxis;
    event_bus_.on_global<GamepadAxisEvent>("axis_value_changed",
        [&](const GamepadAxisEvent& event) { received = event.gamepad_axis; });

    device_.handle_event(MakeAxisEvent(SDL_GAMEPAD_AXIS_LEFTX, 16384));

    EXPECT_EQ(received, GamepadAxis::kXLeftJoystickAxis);
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

    EXPECT_FLOAT_EQ(device_.get_axis_value(GamepadAxis::kXLeftJoystickAxis),
                     12345.0f / SDL_MAX_SINT16);
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

    EXPECT_FLOAT_EQ(device_.get_axis_value(GamepadAxis::kYRightJoystickAxis),
                     -20000.0f / SDL_MAX_SINT16);
}

// ---------------------------------------------------------------------
// handle_event() event emission
// ---------------------------------------------------------------------

TEST_F(SDLInputDeviceTest, ButtonDownEmitsEvent) {
    GamepadButtonEvent received{GamepadButton::kInvalidButton, GamepadButtonEvent::Action::kReleased};
    bool called = false;
    event_bus_.on_global<GamepadButtonEvent>("button_pressed",
        [&](const GamepadButtonEvent& event) {
            called = true;
            received = event;
        });

    device_.handle_event(MakeButtonEvent(SDL_EVENT_GAMEPAD_BUTTON_DOWN, SDL_GAMEPAD_BUTTON_SOUTH));

    EXPECT_TRUE(called);
    EXPECT_EQ(received.gamepad_button, GamepadButton::kBottomFaceButton);
    EXPECT_EQ(received.action, GamepadButtonEvent::Action::kPressed);
}

TEST_F(SDLInputDeviceTest, ButtonUpEmitsEvent) {
    GamepadButtonEvent received{GamepadButton::kInvalidButton, GamepadButtonEvent::Action::kPressed};
    bool called = false;
    event_bus_.on_global<GamepadButtonEvent>("button_released",
        [&](const GamepadButtonEvent& event) {
            called = true;
            received = event;
        });

    device_.handle_event(MakeButtonEvent(SDL_EVENT_GAMEPAD_BUTTON_UP, SDL_GAMEPAD_BUTTON_SOUTH));

    EXPECT_TRUE(called);
    EXPECT_EQ(received.gamepad_button, GamepadButton::kBottomFaceButton);
    EXPECT_EQ(received.action, GamepadButtonEvent::Action::kReleased);
}

TEST_F(SDLInputDeviceTest, AxisMotionEmitsEvent) {
    GamepadAxisEvent received{GamepadAxis::kInvalidAxis, 0.0f};
    bool called = false;
    event_bus_.on_global<GamepadAxisEvent>("axis_value_changed",
        [&](const GamepadAxisEvent& event) {
            called = true;
            received = event;
        });

    device_.handle_event(MakeAxisEvent(SDL_GAMEPAD_AXIS_LEFTX, 16384));

    EXPECT_TRUE(called);
    EXPECT_EQ(received.gamepad_axis, GamepadAxis::kXLeftJoystickAxis);
    EXPECT_FLOAT_EQ(received.val, 16384.0f / SDL_MAX_SINT16);
}

// ---------------------------------------------------------------------
// Gamepad added/removed - smoke tests only (no accessor for `gamepad`)
// ---------------------------------------------------------------------

TEST_F(SDLInputDeviceTest, HandleGamepadAddedWithBogusIdDoesNotCrash) {
    // SDL_IsGamepad() should return false for a device id that was never
    // actually plugged in, so this should be a no-op rather than trying to
    // open a nonexistent device.
    EXPECT_NO_THROW(
        device_.handle_event(MakeDeviceEvent(SDL_EVENT_GAMEPAD_ADDED, /*which=*/9999)));
}

TEST_F(SDLInputDeviceTest, HandleGamepadRemovedWithNoGamepadDoesNotCrash) {
    // No real gamepad is connected in this environment, so `gamepad` should
    // already be null and the removal branch should be skipped entirely.
    EXPECT_NO_THROW(
        device_.handle_event(MakeDeviceEvent(SDL_EVENT_GAMEPAD_REMOVED, /*which=*/9999)));
}

}  // namespace nathan
