// Unit tests for Sound using Google Test.

#include "audio/sound.hpp"

#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

namespace nathan {
namespace {

class SoundTest : public ::testing::Test {
protected:
    void SetUp() override {
        existing_file_ = std::filesystem::temp_directory_path() / "nathan_engine_test_sound.wav";
        std::ofstream(existing_file_) << "dummy audio data";

        missing_file_ = std::filesystem::temp_directory_path() / "nathan_engine_test_sound_missing.wav";
        std::filesystem::remove(missing_file_);  // make sure it really doesn't exist
    }

    void TearDown() override {
        std::filesystem::remove(existing_file_);
    }

    std::filesystem::path existing_file_;
    std::filesystem::path missing_file_;
};

}  // namespace

// ---------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------

TEST_F(SoundTest, ConstructorWithExistingFileStartsPlaying) {
    Sound sound(existing_file_);

    EXPECT_EQ(sound.get_state(), Sound::State::kPlaying);
}

TEST_F(SoundTest, ConstructorWithMissingFileSetsFileNotFound) {
    Sound sound(missing_file_);

    EXPECT_EQ(sound.get_state(), Sound::State::kFileNotFound);
}

TEST_F(SoundTest, GetIdReturnsDistinctIncreasingIds) {
    Sound first(existing_file_);
    Sound second(existing_file_);

    EXPECT_NE(first.get_id(), second.get_id());
    EXPECT_GT(second.get_id(), first.get_id());
}

// ---------------------------------------------------------------------
// Relative position
// ---------------------------------------------------------------------

TEST_F(SoundTest, DefaultRelPositionIsZero) {
    Sound sound(existing_file_);

    EXPECT_EQ(sound.get_rel_position(), Vector3{});
}

TEST_F(SoundTest, SetRelPositionUpdatesPosition) {
    Sound sound(existing_file_);

    sound.set_rel_position(Vector3{.x = 1.0f, .y = 2.0f, .z = 3.0f});

    EXPECT_EQ(sound.get_rel_position(), (Vector3{.x = 1.0f, .y = 2.0f, .z = 3.0f}));
}

// ---------------------------------------------------------------------
// State
// ---------------------------------------------------------------------

TEST_F(SoundTest, SetStateUpdatesStateWhenFileWasFound) {
    Sound sound(existing_file_);

    sound.set_state(Sound::State::kPaused);

    EXPECT_EQ(sound.get_state(), Sound::State::kPaused);
}

TEST_F(SoundTest, SetStateIsNoOpOnceFileNotFound) {
    Sound sound(missing_file_);
    ASSERT_EQ(sound.get_state(), Sound::State::kFileNotFound);

    // kFileNotFound should be sticky: set_state() must not be able to move
    // out of it once entered.
    sound.set_state(Sound::State::kPlaying);

    EXPECT_EQ(sound.get_state(), Sound::State::kFileNotFound);
}

// ---------------------------------------------------------------------
// Gain
// ---------------------------------------------------------------------

TEST_F(SoundTest, DefaultGainIsOne) {
    Sound sound(existing_file_);

    EXPECT_FLOAT_EQ(sound.get_gain(), 1.0f);
}

TEST_F(SoundTest, SetGainUpdatesGain) {
    Sound sound(existing_file_);

    sound.set_gain(0.5f);

    EXPECT_FLOAT_EQ(sound.get_gain(), 0.5f);
}

// ---------------------------------------------------------------------
// Reverb
// ---------------------------------------------------------------------

TEST_F(SoundTest, DefaultReverbIsOne) {
    Sound sound(existing_file_);

    EXPECT_FLOAT_EQ(sound.get_reverb(), 1.0f);
}

TEST_F(SoundTest, SetReverbUpdatesReverb) {
    Sound sound(existing_file_);

    sound.set_reverb(0.25f);

    EXPECT_FLOAT_EQ(sound.get_reverb(), 0.25f);
}

}  // namespace nathan
