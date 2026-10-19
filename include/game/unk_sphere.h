#ifndef GAME_UNK_SPHERE_H
#define GAME_UNK_SPHERE_H

// Sphere sweep helpers shared by Entity_GrowJuiceHeart (unit_800B1498.cpp) and
// Entity_GrowJuiceHeartVine (unit_800B1EDC.cpp), whose sweep functions are identical
// in the DOL (real names unknown).

#include "game/Entity.h"
#include "game/unk_math.h"

// Moving sphere: the items of the entity's sphere groups and the object swept
// against them share this layout (real type unknown).
struct UnkSphere {
    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ int mC;
    /* 0x10 */ u8 unk10[0x14 - 0x10];
    /* 0x14 */ UnkVec3 mPos;
    /* 0x20 */ UnkVec3 mPrevPos;
    /* 0x2C */ u8 unk2C[0x3C - 0x2C];
    /* 0x3C */ f32 mRadius;
};

struct UnkSphereGroup {
    /* 0x00 */ u8 unk0[0x18];
    /* 0x18 */ UnkList mItems;
};

inline void* UnkListAt(UnkList* list, u32 i) {
    return i >= list->mCount ? 0 : list->mData[i];
}

// Same steps as UnkSqrtf, rounded to float before the caller uses it.
inline f32 UnkSqrtfRounded(f32 x) {
    if (x <= 0.0f) {
        return 0.0f;
    }
    double guess = __frsqrte(x);
    guess = 0.5 * (guess * (3.0 - x * guess * guess));
    guess = 0.5 * (guess * (3.0 - x * guess * guess));
    return (f32)(guess * x);
}

// Sweeps sphere A against sphere B, each moving by its velocity; t is the hit time
// in units of the velocities. used by the
// sphere sweeps of unit_800B1498 and unit_800B1EDC (real name unknown).
inline int SweepSpheres(const UnkVec3& posA, const UnkVec3& velA, f32 radiusA, const UnkVec3& posB,
                        const UnkVec3& velB, f32 radiusB, f32& t) {
    UnkVec3 rel = posB - posA;
    UnkVec3 dir = velB - velA;
    f32 radius = radiusB + radiusA;
    f32 c = rel.LengthSq() - radius * radius;
    if (c < 0.0f) {
        return 1;
    }
    f32 a = dir.LengthSq();
    if (a < 0.0001f) {
        return 0;
    }
    f32 b = dir.x * rel.x + dir.y * rel.y + dir.z * rel.z;
    if (b >= 0.0f) {
        return 0;
    }
    f32 disc = b * b; // b * b - a * c swaps two float registers
    disc -= a * c;
    if (disc < 0.0f) {
        return 0;
    }
    t = (-b - UnkSqrtfRounded(disc)) / a;
    return 1;
}

#endif // GAME_UNK_SPHERE_H
