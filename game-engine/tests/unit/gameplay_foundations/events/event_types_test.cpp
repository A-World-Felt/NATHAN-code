// Unit tests for event_types (GamepadButtonEvent, GamepadAxisEvent,
// SoundEvent) using Google Test.

#include "events/event_types.hpp"

#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

#include "audio/sound.hpp"
#include "game_world/node_3d.hpp"
#include "node/event_node.hpp"

namespace nathan {
namespace {

class EventTypesTest : public ::testing::Test {
protected:
    void SetUp() override {
        existing_file_ = std::filesystem::temp_directory_path() / "nathan_engine_test_event_types.wav";
        std::ofstream(existing_file_) << "dummy audio data";
    }

    void TearDown() override {
        std::filesystem::remove(existing_file_);
    }

    std::filesystem::path existing_file_;
};

}  // namespace

// ---------------------------------------------------------------------
// GamepadButtonEvent / GamepadAxisEvent - plain data aggregates
// ---------------------------------------------------------------------

TEST_F(EventTypesTest, GamepadButtonEventStoresFields) {
    GamepadButtonEvent event{.gamepad_button = GamepadButton::kTopFaceButton, .action = GamepadButtonEvent::Action::kPressed};

    EXPECT_EQ(event.gamepad_button, GamepadButton::kTopFaceButton);
    EXPECT_EQ(event.action, GamepadButtonEvent::Action::kPressed);
}

TEST_F(EventTypesTest, GamepadAxisEventStoresFields) {
    GamepadAxisEvent event{.gamepad_axis = GamepadAxis::kXLeftJoystickAxis, .val = 0.75f};

    EXPECT_EQ(event.gamepad_axis, GamepadAxis::kXLeftJoystickAxis);
    EXPECT_FLOAT_EQ(event.val, 0.75f);
}

// ---------------------------------------------------------------------
// SoundEvent - construction and accessors
// ---------------------------------------------------------------------

TEST_F(EventTypesTest, ConstructorStoresNodeAndSound) {
    Node3D node;
    Sound sound(existing_file_);

    SoundEvent event(&node, sound);

    EXPECT_EQ(event.get_node(), &node);
    EXPECT_EQ(event.get_sound(), &sound);
}

TEST_F(EventTypesTest, SetNodeUpdatesNode) {
    Node3D node_a;
    Node3D node_b;
    Sound sound(existing_file_);
    SoundEvent event(&node_a, sound);

    event.set_node(&node_b);

    EXPECT_EQ(event.get_node(), &node_b);
}

TEST_F(EventTypesTest, SetSoundUpdatesSound) {
    Node3D node;
    Sound sound_a(existing_file_);
    Sound sound_b(existing_file_);
    SoundEvent event(&node, sound_a);

    event.set_sound(&sound_b);

    EXPECT_EQ(event.get_sound(), &sound_b);
}

// ---------------------------------------------------------------------
// SoundEvent::set_rel_position()
// ---------------------------------------------------------------------

TEST_F(EventTypesTest, SetRelPositionWithNode3DUpdatesSoundPosition) {
    Node3D source(4.0f, 5.0f, 6.0f);
    Node3D head(1.0f, 2.0f, 3.0f);
    Sound sound(existing_file_);
    SoundEvent event(&source, sound);

    event.set_rel_position(head);

    // Transform::get_relative_position_to() returns position_ minus the
    // argument's position, called here as source.transform.get_relative_
    // position_to(head.transform) -> source_pos - head_pos.
    EXPECT_EQ(sound.get_rel_position(), (Vector3{.x = 3.0f, .y = 3.0f, .z = 3.0f}));
}

TEST_F(EventTypesTest, SetRelPositionWithNonNode3DNodeIsNoOp) {
    // set_rel_position() dynamic_casts its stored EventNode* to Node3D*;
    // a plain EventNode (not a Node3D) should make that cast fail and
    // leave the sound's position untouched.
    EventNode plain_node;
    Node3D head(1.0f, 2.0f, 3.0f);
    Sound sound(existing_file_);
    SoundEvent event(&plain_node, sound);

    event.set_rel_position(head);

    EXPECT_EQ(sound.get_rel_position(), Vector3{});
}

}  // namespace nathan
