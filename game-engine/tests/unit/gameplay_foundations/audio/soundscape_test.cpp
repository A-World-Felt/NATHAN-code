// Unit tests for Soundscape using Google Test.

#include "audio/soundscape.hpp"

#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

#include "audio/sound.hpp"
#include "events/event_bus.hpp"
#include "events/event_types.hpp"
#include "game_world/node_3d.hpp"

namespace nathan {
namespace {

class SoundscapeTest : public ::testing::Test {
protected:
    void SetUp() override {
        existing_file_ = std::filesystem::temp_directory_path() / "nathan_engine_test_soundscape.wav";
        std::ofstream(existing_file_) << "dummy audio data";

        missing_file_ = std::filesystem::temp_directory_path() / "nathan_engine_test_soundscape_missing.wav";
        std::filesystem::remove(missing_file_);
    }

    void TearDown() override {
        std::filesystem::remove(existing_file_);
    }

    std::filesystem::path existing_file_;
    std::filesystem::path missing_file_;
    EventBus event_bus_;
};

}  // namespace

// ---------------------------------------------------------------------
// head node accessors
// ---------------------------------------------------------------------

TEST_F(SoundscapeTest, ConstructorStoresHeadNode) {
    Node3D head(1.0f, 2.0f, 3.0f);
    Soundscape soundscape(event_bus_, &head);

    EXPECT_EQ(soundscape.get_head_node(), &head);
}

TEST_F(SoundscapeTest, DefaultHeadNodeIsNull) {
    Soundscape soundscape(event_bus_);

    EXPECT_EQ(soundscape.get_head_node(), nullptr);
}

TEST_F(SoundscapeTest, SetHeadNodeUpdatesHeadNode) {
    Node3D head(0.0f, 0.0f, 0.0f);
    Soundscape soundscape(event_bus_);

    soundscape.set_head_node(&head);

    EXPECT_EQ(soundscape.get_head_node(), &head);
}

// ---------------------------------------------------------------------
// update() - positioning
// ---------------------------------------------------------------------

TEST_F(SoundscapeTest, UpdateSetsRelativePositionForPlayingSound) {
    Node3D head(1.0f, 2.0f, 3.0f);
    Node3D source(4.0f, 5.0f, 6.0f);
    Soundscape soundscape(event_bus_, &head);

    Sound sound(existing_file_);
    event_bus_.emit_global<SoundEvent>("play_sound", SoundEvent(&source, sound));

    soundscape.update();

    // SoundEvent::set_rel_position() stores the emitting node's position
    // relative to the head node's transform, i.e. source_pos - head_pos.
    EXPECT_EQ(sound.get_rel_position(), (Vector3{.x = 3.0f, .y = 3.0f, .z = 3.0f}));
    EXPECT_EQ(sound.get_state(), Sound::State::kPlaying);
}

TEST_F(SoundscapeTest, PlaySoundEventAddsSoundToList) {
    Node3D head(1.0f, 2.0f, 3.0f);
    Node3D source(4.0f, 5.0f, 6.0f);
    Soundscape soundscape(event_bus_, &head);

    Sound sound(existing_file_);
    event_bus_.emit_global<SoundEvent>("play_sound", SoundEvent(&source, sound));

    const auto sounds = soundscape.get_sounds();

    ASSERT_EQ(sounds.size(), 1u);
    EXPECT_EQ(sounds.front().get_sound(), &sound);
    EXPECT_EQ(sounds.front().get_node(), &source);
}

TEST_F(SoundscapeTest, UpdateDoesNotMoveNonPlayingSound) {
    Node3D head(1.0f, 2.0f, 3.0f);
    Node3D source(4.0f, 5.0f, 6.0f);
    Soundscape soundscape(event_bus_, &head);

    Sound sound(existing_file_);
    sound.set_state(Sound::State::kPaused);
    event_bus_.emit_global<SoundEvent>("play_sound", SoundEvent(&source, sound));

    soundscape.update();

    EXPECT_EQ(sound.get_rel_position(), Vector3{});
    EXPECT_EQ(sound.get_state(), Sound::State::kPaused);
}

TEST_F(SoundscapeTest, UpdateDoesNotMoveSoundWithoutHeadNode) {
    Node3D source(4.0f, 5.0f, 6.0f);
    Soundscape soundscape(event_bus_);  // no head node

    Sound sound(existing_file_);
    event_bus_.emit_global<SoundEvent>("play_sound", SoundEvent(&source, sound));

    soundscape.update();

    EXPECT_EQ(sound.get_rel_position(), Vector3{});
}

// ---------------------------------------------------------------------
// update() - filtering / lifecycle
// ---------------------------------------------------------------------

TEST_F(SoundscapeTest, UpdateIgnoresFileNotFoundSounds) {
    Node3D head(1.0f, 2.0f, 3.0f);
    Node3D source(4.0f, 5.0f, 6.0f);
    Soundscape soundscape(event_bus_, &head);

    Sound missing_sound(missing_file_);
    ASSERT_EQ(missing_sound.get_state(), Sound::State::kFileNotFound);

    event_bus_.emit_global<SoundEvent>("play_sound", SoundEvent(&source, missing_sound));

    // The "play_sound" handler should drop kFileNotFound sounds before
    // they're ever tracked.
    EXPECT_TRUE(soundscape.get_sounds().empty());

    EXPECT_NO_THROW(soundscape.update());
    EXPECT_EQ(missing_sound.get_state(), Sound::State::kFileNotFound);
    EXPECT_EQ(missing_sound.get_rel_position(), Vector3{});
}

TEST_F(SoundscapeTest, UpdateStopsSoundOnDestroyedNodeAsGraceFrame) {
    Node3D head(0.0f, 0.0f, 0.0f);
    Node3D source(4.0f, 5.0f, 6.0f);
    Soundscape soundscape(event_bus_, &head);

    Sound sound(existing_file_);
    event_bus_.emit_global<SoundEvent>("play_sound", SoundEvent(&source, sound));
    source.set_destroyed(true);

    // First update(): grace frame - stopped, but not yet erased.
    soundscape.update();

    EXPECT_EQ(sound.get_state(), Sound::State::kStopped);
    EXPECT_EQ(soundscape.get_sounds().size(), 1u);
}

TEST_F(SoundscapeTest, UpdateRemovesStoppedSoundOnFollowingFrame) {
    Node3D head(0.0f, 0.0f, 0.0f);
    Node3D source(4.0f, 5.0f, 6.0f);
    Soundscape soundscape(event_bus_, &head);

    Sound sound(existing_file_);
    event_bus_.emit_global<SoundEvent>("play_sound", SoundEvent(&source, sound));
    source.set_destroyed(true);
    soundscape.update();  // grace frame: state -> kStopped
    ASSERT_EQ(sound.get_state(), Sound::State::kStopped);

    // Second update(): the now-kStopped sound should be erased.
    soundscape.update();

    EXPECT_TRUE(soundscape.get_sounds().empty());
    EXPECT_EQ(sound.get_state(), Sound::State::kStopped);

    // Further update() calls should be harmless no-ops now that the list
    // is empty.
    EXPECT_NO_THROW(soundscape.update());
}

TEST_F(SoundscapeTest, UpdateRemovesSoundWithNullNodeWithoutTouchingIt) {
    Soundscape soundscape(event_bus_);

    Sound sound(existing_file_);
    event_bus_.emit_global<SoundEvent>("play_sound", SoundEvent(nullptr, sound));

    // A null node means the sound is erased before any state/position
    // mutation logic runs, so it should come out exactly as it went in.
    EXPECT_NO_THROW(soundscape.update());
    EXPECT_TRUE(soundscape.get_sounds().empty());
    EXPECT_EQ(sound.get_state(), Sound::State::kPlaying);
    EXPECT_EQ(sound.get_rel_position(), Vector3{});
}

}  // namespace nathan
