//
// Created by jcksn on 7/12/2026.
//

#include "Vector.h"
#include <cmath>

namespace vect {
    float Dot(const Vector2& v1, const Vector2& v2) {
        return v1.x * v2.x + v1.y * v2.y;
    }
    float Cross(const Vector2& v1, const Vector2& v2) {
        return v1.x * v2.y - v1.y * v2.x;
    }
    float LengthSq(const Vector2& v1) {
        return Dot(v1, v1);
    }
    float Length(const Vector2& v1) {
        return std::sqrt(LengthSq(v1));
    }
    Vector2 Normalize (const Vector2& v1) {
        const float len = Length(v1);

        if (len == 0.0f) {
            return v1;
        }

        return {v1.x / len, v1.y / len};
    }
}