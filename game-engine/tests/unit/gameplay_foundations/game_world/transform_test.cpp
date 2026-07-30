// Unit tests for Transform using Google Test

#include <gtest/gtest.h>

#include "gameplay_foundations/game_world/transform.hpp"
#include "gameplay_foundations/math/vector3.hpp"

TEST(TransformTest, DefaultConstructorInitializesToZero) {
    nathan::Transform t;

    nathan::Vector3 expected_position{0.0f, 0.0f, 0.0f};
    nathan::Vector3 expected_velocity{0.0f, 0.0f, 0.0f};
    nathan::Vector3 expected_acceleration{0.0f, 0.0f, 0.0f};
    EXPECT_EQ(t.get_position(), expected_position);
    EXPECT_FLOAT_EQ(t.get_rotation(), 0.0f);
    EXPECT_EQ(t.get_velocity(), expected_velocity);
    EXPECT_EQ(t.get_acceleration(), expected_acceleration);
    EXPECT_FALSE(t.is_static());
}

TEST(TransformTest, ConstructorWithVector3SetsPosition) {
    nathan::Transform t(nathan::Vector3{1.0f, 2.0f, 3.0f});

    nathan::Vector3 expected_position{1.0f, 2.0f, 3.0f};
    EXPECT_EQ(t.get_position(), expected_position);
}

TEST(TransformTest, ConstructorWithFloatsSetsPosition) {
    nathan::Transform t(1.0f, 2.0f, 3.0f);

    nathan::Vector3 expected_position{1.0f, 2.0f, 3.0f};
    EXPECT_EQ(t.get_position(), expected_position);
}

TEST(TransformTest, ConstructorWithFloatsDefaultsZToZero) {
    nathan::Transform t(1.0f, 2.0f);

    nathan::Vector3 expected_position{1.0f, 2.0f, 0.0f};
    EXPECT_EQ(t.get_position(), expected_position);
}

TEST(TransformTest, SetPositionWithVector3UpdatesPosition) {
    nathan::Transform t;

    t.set_position(nathan::Vector3{4.0f, 5.0f, 6.0f});

    nathan::Vector3 expected_position{4.0f, 5.0f, 6.0f};
    EXPECT_EQ(t.get_position(), expected_position);
}

TEST(TransformTest, SetPositionWithFloatsUpdatesPosition) {
    nathan::Transform t;

    t.set_position(4.0f, 5.0f, 6.0f);

    nathan::Vector3 expected_position{4.0f, 5.0f, 6.0f};
    EXPECT_EQ(t.get_position(), expected_position);
}

TEST(TransformTest, TranslateWithVector3AddsToPosition) {
    nathan::Transform t(1.0f, 1.0f, 1.0f);

    t.translate(nathan::Vector3{1.0f, 2.0f, 3.0f});

    nathan::Vector3 expected_position{2.0f, 3.0f, 4.0f};
    EXPECT_EQ(t.get_position(), expected_position);
}

TEST(TransformTest, TranslateWithFloatsAddsToPosition) {
    nathan::Transform t(1.0f, 1.0f, 1.0f);

    t.translate(1.0f, 2.0f, 3.0f);

    nathan::Vector3 expected_position{2.0f, 3.0f, 4.0f};
    EXPECT_EQ(t.get_position(), expected_position);
}

TEST(TransformTest, GetRelativePositionToReturnsDifferenceFromParent) {
    nathan::Transform child(10.0f, 10.0f, 10.0f);
    nathan::Transform parent(3.0f, 4.0f, 5.0f);

    nathan::Vector3 relative = child.get_relative_position_to(parent);

    nathan::Vector3 expected_relative{7.0f, 6.0f, 5.0f};
    EXPECT_EQ(relative, expected_relative);
}

TEST(TransformTest, SetRelativePositionToVector3SetsAbsolutePosition) {
    nathan::Transform child;
    nathan::Transform parent(3.0f, 4.0f, 5.0f);

    child.set_relative_position_to(nathan::Vector3{1.0f, 1.0f, 1.0f}, parent);

    nathan::Vector3 expected_position{4.0f, 5.0f, 6.0f};
    EXPECT_EQ(child.get_position(), expected_position);
}

TEST(TransformTest, SetRelativePositionToFloatsSetsAbsolutePosition) {
    nathan::Transform child;
    nathan::Transform parent(3.0f, 4.0f, 5.0f);

    child.set_relative_position_to(1.0f, 1.0f, 1.0f, parent);

    nathan::Vector3 expected_position{4.0f, 5.0f, 6.0f};
    EXPECT_EQ(child.get_position(), expected_position);
}

TEST(TransformTest, SetRotationUpdatesRotation) {
    nathan::Transform t;

    t.set_rotation(90.0f);

    EXPECT_FLOAT_EQ(t.get_rotation(), 90.0f);
}

TEST(TransformTest, RotateAddsToRotation) {
    nathan::Transform t;
    t.set_rotation(10.0f);

    t.rotate(15.0f);

    EXPECT_FLOAT_EQ(t.get_rotation(), 25.0f);
}

TEST(TransformTest, GetRelativeRotationToReturnsDifferenceFromParent) {
    nathan::Transform child;
    child.set_rotation(90.0f);
    nathan::Transform parent;
    parent.set_rotation(30.0f);

    EXPECT_FLOAT_EQ(child.get_relative_rotation_to(parent), 60.0f);
}

TEST(TransformTest, GetRelativeRotationToWrapsNegativeResultIntoRange) {
    nathan::Transform child;
    child.set_rotation(10.0f);
    nathan::Transform parent;
    parent.set_rotation(350.0f);

    // Raw difference would be -340, wrapped result should be 20
    EXPECT_FLOAT_EQ(child.get_relative_rotation_to(parent), 20.0f);
}

TEST(TransformTest, SetRelativeRotationToSetsAbsoluteRotation) {
    nathan::Transform child;
    nathan::Transform parent;
    parent.set_rotation(30.0f);

    child.set_relative_rotation_to(60.0f, parent);

    EXPECT_FLOAT_EQ(child.get_rotation(), 90.0f);
}

TEST(TransformTest, SetRelativeRotationToWrapsResultIntoRange) {
    nathan::Transform child;
    nathan::Transform parent;
    parent.set_rotation(350.0f);

    child.set_relative_rotation_to(30.0f, parent);

    // 350 + 30 = 380, wrapped result should be 20
    EXPECT_FLOAT_EQ(child.get_rotation(), 20.0f);
}

TEST(TransformTest, SetRotationWrapsValuesAt360OrAboveIntoRange) {
    nathan::Transform t;

    t.set_rotation(370.0f);

    EXPECT_FLOAT_EQ(t.get_rotation(), 10.0f);
}

TEST(TransformTest, SetRotationWrapsNegativeValuesIntoRange) {
    nathan::Transform t;

    t.set_rotation(-10.0f);

    EXPECT_FLOAT_EQ(t.get_rotation(), 350.0f);
}

TEST(TransformTest, SetRotationAcceptsExactBoundaryValues) {
    nathan::Transform t;

    t.set_rotation(0.0f);
    EXPECT_FLOAT_EQ(t.get_rotation(), 0.0f);

    t.set_rotation(360.0f);
    EXPECT_FLOAT_EQ(t.get_rotation(), 0.0f);
}

TEST(TransformTest, RotateWrapsPastUpperBoundIntoRange) {
    nathan::Transform t;
    t.set_rotation(350.0f);

    t.rotate(20.0f);

    EXPECT_FLOAT_EQ(t.get_rotation(), 10.0f);
}

TEST(TransformTest, RotateWrapsPastLowerBoundIntoRange) {
    nathan::Transform t;
    t.set_rotation(10.0f);

    t.rotate(-20.0f);

    EXPECT_FLOAT_EQ(t.get_rotation(), 350.0f);
}

TEST(TransformTest, SetVelocityWithVector3UpdatesVelocity) {
    nathan::Transform t;

    t.set_velocity(nathan::Vector3{1.0f, 2.0f, 3.0f});

    nathan::Vector3 expected_velocity{1.0f, 2.0f, 3.0f};
    EXPECT_EQ(t.get_velocity(), expected_velocity);
}

TEST(TransformTest, SetVelocityWithFloatsUpdatesVelocity) {
    nathan::Transform t;

    t.set_velocity(1.0f, 2.0f, 3.0f);

    nathan::Vector3 expected_velocity{1.0f, 2.0f, 3.0f};
    EXPECT_EQ(t.get_velocity(), expected_velocity);
}

TEST(TransformTest, SetAccelerationWithVector3UpdatesAcceleration) {
    nathan::Transform t;

    t.set_acceleration(nathan::Vector3{1.0f, 2.0f, 3.0f});

    nathan::Vector3 expected_acceleration{1.0f, 2.0f, 3.0f};
    EXPECT_EQ(t.get_acceleration(), expected_acceleration);
}

TEST(TransformTest, SetAccelerationWithFloatsUpdatesAcceleration) {
    nathan::Transform t;

    t.set_acceleration(1.0f, 2.0f, 3.0f);

    nathan::Vector3 expected_acceleration{1.0f, 2.0f, 3.0f};
    EXPECT_EQ(t.get_acceleration(), expected_acceleration);
}

TEST(TransformTest, SetStaticUpdatesFlag) {
    nathan::Transform t;

    t.set_static(true);

    EXPECT_TRUE(t.is_static());
}

TEST(TransformTest, IntegrateUpdatesVelocityThenPositionWhenNotStatic) {
    nathan::Transform t;
    t.set_acceleration(0.0f, 10.0f, 0.0f);
    t.set_velocity(1.0f, 0.0f, 0.0f);

    t.integrate(1.0f);  // delta = 1s

    nathan::Vector3 expected_velocity{1.0f, 10.0f, 0.0f};
    nathan::Vector3 expected_position{1.0f, 10.0f, 0.0f};
    EXPECT_EQ(t.get_velocity(), expected_velocity);
    EXPECT_EQ(t.get_position(), expected_position);
}

TEST(TransformTest, IntegrateIsNoOpWhenStatic) {
    nathan::Transform t;
    t.set_static(true);
    t.set_acceleration(0.0f, 10.0f, 0.0f);
    t.set_velocity(1.0f, 0.0f, 0.0f);

    t.integrate(1.0f);

    nathan::Vector3 expected_acceleration{0.0f, 0.0f, 0.0f};
    nathan::Vector3 expected_velocity{0.0f, 0.0f, 0.0f};
    nathan::Vector3 expected_position{0.0f, 0.0f, 0.0f};
    EXPECT_EQ(t.get_acceleration(), expected_acceleration);
    EXPECT_EQ(t.get_velocity(), expected_velocity);
    EXPECT_EQ(t.get_position(), expected_position);
}

TEST(TransformTest, IntegrateAccumulatesOverMultipleTicks) {
    nathan::Transform t;
    t.set_acceleration(0.0f, 0.0f, 0.0f);
    t.set_velocity(2.0f, 0.0f, 0.0f);

    t.integrate(0.5f);
    t.integrate(0.5f);

    nathan::Vector3 expected_position{2.0f, 0.0f, 0.0f};
    EXPECT_EQ(t.get_position(), expected_position);
}


