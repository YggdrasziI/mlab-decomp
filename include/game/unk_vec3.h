#ifndef GAME_UNK_VEC3_H
#define GAME_UNK_VEC3_H

// Shared header included by most game units (real name unknown).

#include "types.h"

// Three-float vector with an inline constructor. Each game unit that includes this
// header gets its own (0, 1, 0) copy in .bss, built by the unit's static initializer.
// Real type and object names unknown.
struct UnkVec3 {
    f32 x, y, z;
    UnkVec3() {}
    UnkVec3(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3(const UnkVec3& other) : x(other.x), y(other.y), z(other.z) {}

    UnkVec3 operator-(const UnkVec3& other) const {
        UnkVec3 result;
        result.x = x - other.x;
        result.y = y - other.y;
        result.z = z - other.z;
        return result;
    }
    UnkVec3 operator+(const UnkVec3& other) const {
        UnkVec3 result;
        result.x = x + other.x;
        result.y = y + other.y;
        result.z = z + other.z;
        return result;
    }
    UnkVec3 operator*(f32 s) const {
        UnkVec3 result;
        result.x = x * s;
        result.y = y * s;
        result.z = z * s;
        return result;
    }
    UnkVec3 Normalized() const;
    bool operator==(const UnkVec3& other) const { return x == other.x && y == other.y && z == other.z; }
    f32 LengthSq() const { return x * x + y * y + z * z; }
    f32 Length() const;
};

#include "game/unk_math.h"

inline UnkVec3 UnkVec3Zero() {
    UnkVec3 v;
    v.x = v.y = v.z = 0.0f;
    return v;
}

// 1 / sqrt(x) with two Newton steps on frsqrte (x > 0).
inline f32 UnkInvSqrtf(f32 x) {
    double guess = __frsqrte(x);
    guess = 0.5 * (guess * (3.0 - x * guess * guess));
    guess = 0.5 * (guess * (3.0 - x * guess * guess));
    return guess;
}

inline UnkVec3 UnkVec3::Normalized() const {
    f32 lengthSq = LengthSq();
    if (lengthSq == 0.0f) {
        return UnkVec3Zero();
    }
    return *this * UnkInvSqrtf(lengthSq);
}

inline f32 UnkVec3::Length() const {
    return UnkSqrtf(LengthSq());
}

static UnkVec3 sUnkVec3(0.0f, 1.0f, 0.0f);

#endif // GAME_UNK_VEC3_H
