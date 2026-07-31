#ifndef GAME_ENGINE_MATH_TRANSFORM_H_
#define GAME_ENGINE_MATH_TRANSFORM_H_

#include "math/vector3.hpp"

namespace nathan {

// Spatial state of a GameObject
class Transform {
public:
    Transform() = default;
    Transform(Vector3 position);
    Transform(float x, float y, float z = 0.0f);

    // Position methods
    Vector3 get_position() const { return position_; }
    void set_position(Vector3 position);
    void set_position(float x, float y, float z = 0.0f);
    void translate(Vector3 delta);
    void translate(float dx, float dy, float dz = 0.0f);
    Vector3 get_relative_position_to(const Transform& parent_absolute) const;
    void set_relative_position_to(const Vector3& relative_position, const Transform& parent_absolute);
    void set_relative_position_to(float relative_x, float relative_y, float relative_z, const Transform& parent_absolute);
   
    // Rotation methods
    float get_rotation() const { return rotation_deg_; }
    void set_rotation(float degrees);
    void rotate(float delta_degrees);
    float get_relative_rotation_to(const Transform& parent_absolute) const;
    void set_relative_rotation_to(float relative_degrees, const Transform& parent_absolute);

    // Physics methods
    Vector3 get_velocity() const { return velocity_; }
    void set_velocity(Vector3 velocity);
    void set_velocity(float vx, float vy, float vz = 0.0f);

    Vector3 get_acceleration() const { return acceleration_; }
    void set_acceleration(Vector3 acceleration);
    void set_acceleration(float ax, float ay, float az = 0.0f);

    float get_angular_velocity() const { return angular_velocity_deg_; }
    void set_angular_velocity(float degrees_per_second);

    float get_angular_acceleration() const { return angular_acceleration_deg_; }
    void set_angular_acceleration(float degrees_per_second_squared);

    bool is_static() const { return is_static_; }
    void set_static(bool is_static);

    // Integrates acceleration -> velocity -> position for one tick if static is false.
    void integrate(float delta);

private:
    static float normalize_angle(float degrees);

    Vector3 position_;
    Vector3 velocity_;
    Vector3 acceleration_;
    
    float rotation_deg_ = 0.0f;
    float angular_velocity_deg_ = 0.0f;
    float angular_acceleration_deg_ = 0.0f;

    bool is_static_ = false;
};

}  // namespace nathan

#endif  // GAME_ENGINE_MATH_TRANSFORM_H_