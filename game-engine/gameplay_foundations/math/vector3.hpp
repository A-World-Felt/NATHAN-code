#ifndef GAME_ENGINE_MATH_VECTOR3_H_
#define GAME_ENGINE_MATH_VECTOR3_H_

#include <ostream>

namespace nathan {

// Lightweight 3-component float vector
struct Vector3 {
    static constexpr float kEpsilon = 1e-5f;
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Vector3& operator+=(const Vector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    friend Vector3 operator+(Vector3 lhs, const Vector3& rhs) {
        lhs += rhs;
        return lhs;
    }

    friend Vector3 operator*(Vector3 v, float scalar) {
        return {v.x * scalar, v.y * scalar, v.z * scalar};
    }

    friend Vector3 operator-(const Vector3& lhs, const Vector3& rhs) {
        return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
    }

    // Equality with a small epsilon tolerance to absorb floating point error
    friend bool operator==(const Vector3& lhs, const Vector3& rhs) {
        auto approx_equal = [](float a, float b) {
            float diff = a - b;
            if (diff < 0.0f) diff = -diff;
            return diff < Vector3::kEpsilon;
        };

        return approx_equal(lhs.x, rhs.x) &&
               approx_equal(lhs.y, rhs.y) &&
               approx_equal(lhs.z, rhs.z);
    }

    friend bool operator!=(const Vector3& lhs, const Vector3& rhs) {
        return !(lhs == rhs);
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector3& v) {
        return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    }
};

}  // namespace nathan

#endif  // GAME_ENGINE_MATH_VECTOR3_H_
