// Unit tests for Node3D using Google Test

#include <gtest/gtest.h>

#include "gameplay_foundations/game_world/node_3d.hpp"
#include "gameplay_foundations/math/vector3.hpp"

TEST(Node3DTest, ConstructorWithVector3SetsInitialPosition) {
    nathan::Node3D node(nathan::Vector3{1.0f, 2.0f, 3.0f});
    
    nathan::Vector3 expected_result = nathan::Vector3{1.0f, 2.0f, 3.0f};
    EXPECT_EQ(node.get_transform().get_position(), expected_result);
}

TEST(Node3DTest, ConstructorWithFloatsSetsInitialPosition) {
    nathan::Node3D node(1.0f, 2.0f, 3.0f);

    nathan::Vector3 expected_result = nathan::Vector3{1.0f, 2.0f, 3.0f};
    EXPECT_EQ(node.get_transform().get_position(), expected_result);
}

TEST(Node3DTest, ConstructorWithFloatsDefaultsZToZero) {
    nathan::Node3D node(1.0f, 2.0f);

    nathan::Vector3 expected_result = nathan::Vector3{1.0f, 2.0f, 0.0f};
    EXPECT_EQ(node.get_transform().get_position(), expected_result);
}

TEST(Node3DTest, AfterLoopIntegratesVelocityAndAccelerationIntoPosition) {
    nathan::Node3D node;
    node.get_transform().set_velocity(1.0f, 0.0f, 0.0f);
    node.get_transform().set_acceleration(0.0f, 2.0f, 0.0f);

    node.after_loop(1.0f);  // delta = 1s

    nathan::Vector3 expected_velocity = nathan::Vector3{1.0f, 2.0f, 0.0f};
    nathan::Vector3 expected_position = nathan::Vector3{1.0f, 2.0f, 0.0f};
    EXPECT_EQ(node.get_transform().get_velocity(), expected_velocity);
    EXPECT_EQ(node.get_transform().get_position(), expected_position);
}

TEST(Node3DTest, AfterLoopIsNoOpWhenTransformIsStatic) {
    nathan::Node3D node;
    node.get_transform().set_static(true);
    node.get_transform().set_velocity(1.0f, 0.0f, 0.0f);
    node.get_transform().set_acceleration(0.0f, 2.0f, 0.0f);

    node.after_loop(1.0f);

    nathan::Vector3 expected_acceleration = nathan::Vector3{0.0f, 0.0f, 0.0f};
    nathan::Vector3 expected_velocity = nathan::Vector3{0.0f, 0.0f, 0.0f};
    nathan::Vector3 expected_position = nathan::Vector3{0.0f, 0.0f, 0.0f};
    EXPECT_EQ(node.get_transform().get_acceleration(), expected_acceleration);
    EXPECT_EQ(node.get_transform().get_velocity(), expected_velocity);
    EXPECT_EQ(node.get_transform().get_position(), expected_position);
}

TEST(Node3DTest, AfterLoopAccumulatesOverMultipleCalls) {
    nathan::Node3D node;
    node.get_transform().set_velocity(2.0f, 0.0f, 0.0f);

    node.after_loop(0.5f);
    node.after_loop(0.5f);

    nathan::Vector3 expected_position = nathan::Vector3{2.0f, 0.0f, 0.0f};
    EXPECT_EQ(node.get_transform().get_position(), expected_position);
}
