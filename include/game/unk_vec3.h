#ifndef GAME_UNK_VEC3_H
#define GAME_UNK_VEC3_H

// Shared header included by most game units (real name unknown).

#include "types.h"

// Three-float vector with an inline constructor. Each game unit that includes this
// header gets its own (0, 1, 0) copy in .bss, built by the unit's static initializer.
// Real type and object names unknown.
struct UnkVec3 {
    f32 x, y, z;
    UnkVec3(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

static UnkVec3 sUnkVec3(0.0f, 1.0f, 0.0f);

#endif // GAME_UNK_VEC3_H
