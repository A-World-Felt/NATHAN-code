#include "game_world/transform.hpp"
#include <iostream>

namespace nathan {

Transform::Transform(Vector3 position) : position_(position) {}

Transform::Transform(float x, float y, float z) : position_{x, y, z} {}

void Transform::set_position(Vector3 position) {
    position_ = position;
}

void Transform::set_position(float x, float y, float z) {
    position_ = Vector3{x, y, z};
}

void Transform::translate(Vector3 delta) {
    position_ += delta;
}

void Transform::translate(float dx, float dy, float dz) {
    position_ += Vector3{dx, dy, dz};
}

Vector3 Transform::get_relative_position_to(const Transform& parent_absolute) const {
    return position_ - parent_absolute.position_;
}

void Transform::set_relative_position_to(const Vector3& relative_position, const Transform& parent_absolute) {
    position_ = parent_absolute.position_ + relative_position;
}

void Transform::set_relative_position_to(float relative_x, float relative_y, float relative_z, const Transform& parent_absolute) {
    position_ = parent_absolute.position_ + Vector3{relative_x, relative_y, relative_z};
}

float Transform::normalize_angle(float degrees) {
    while (degrees < 0.0f) {
        degrees += 360.0f;
    }
    while (degrees >= 360.0f) {
        degrees -= 360.0f;
    }
    return degrees;
}

void Transform::set_rotation(float degrees) {
    rotation_deg_ = normalize_angle(degrees);
}

void Transform::rotate(float delta_degrees) {
    float new_rotation = normalize_angle(rotation_deg_ + delta_degrees);
    rotation_deg_ = new_rotation;
}

float Transform::get_relative_rotation_to(const Transform& parent_absolute) const {
    return normalize_angle(rotation_deg_ - parent_absolute.rotation_deg_);
}

void Transform::set_relative_rotation_to(float relative_degrees, const Transform& parent_absolute) {
    rotation_deg_ = normalize_angle(parent_absolute.rotation_deg_ + relative_degrees);
}

void Transform::set_velocity(Vector3 velocity) {
    velocity_ = velocity;
}

void Transform::set_velocity(float vx, float vy, float vz) {
    velocity_ = Vector3{vx, vy, vz};
}

void Transform::set_acceleration(Vector3 acceleration) {
    acceleration_ = acceleration;
}

void Transform::set_acceleration(float ax, float ay, float az) {
    acceleration_ = Vector3{ax, ay, az};
}

void Transform::set_angular_velocity(float degrees_per_second) {
    angular_velocity_deg_ = degrees_per_second;
}

void Transform::set_angular_acceleration(float degrees_per_second_squared) {
    angular_acceleration_deg_ = degrees_per_second_squared;
}

void Transform::set_static(bool is_static) {
    is_static_ = is_static;
}

void Transform::integrate(float delta) {
    if (is_static_) {
        acceleration_ = Vector3{0.0f, 0.0f, 0.0f};
        velocity_ = Vector3{0.0f, 0.0f, 0.0f};
        angular_acceleration_deg_ = 0.0f;
        angular_velocity_deg_ = 0.0f;
        return;
    }

    position_ += velocity_ * delta + acceleration_ * (0.5f * delta*delta);
    velocity_ += acceleration_ * delta;

    rotation_deg_ += normalize_angle(angular_velocity_deg_ * delta + angular_acceleration_deg_ * (0.5f * delta*delta));
    angular_velocity_deg_ += angular_acceleration_deg_ * delta;
}

}  // namespace nathan