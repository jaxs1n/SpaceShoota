#ifndef PHYSICS_ENGINE_LIBRARY_H
#define PHYSICS_ENGINE_LIBRARY_H

namespace vect {
    struct Vector2 {
        float x{};
        float y{};

        constexpr Vector2() = default;
        constexpr Vector2(const float x, const float y) : x{x}, y{y} {};

        inline Vector2& operator *= (const float scalar) {
            x *= scalar;
            y *= scalar;
            return *this;
        }
        inline Vector2& operator /= (const float scalar) {
            x /= scalar;
            y /= scalar;
            return *this;
        }
        inline Vector2& operator += (const Vector2& v) {
            x += v.x;
            y += v.y;
            return *this;
        }
        inline Vector2& operator -= (const Vector2& v) {
            x -= v.x;
            y -= v.y;
            return *this;
        }

        Vector2 operator + (const Vector2& v) const {
            return {v.x + x, v.y + y};
        }
        Vector2 operator - (const Vector2& v) const {
            return {x - v.x, y - v.y};
        }
    };

    float Dot (const Vector2& v1, const Vector2& v2);
    float Cross (const Vector2& v1, const Vector2& v2);
    float Length (const Vector2& v1);
    float LengthSq (const Vector2& v1);

    Vector2 Normalize (const Vector2& v1);
    Vector2 Inverse (const Vector2& v1);
}

#endif // PHYSICS_ENGINE_LIBRARY_H