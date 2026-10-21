// Game unit at 0x800D4C64-0x800D50C4 (real file name unknown): the constructor and
// copy of a path point and the copy of a path, both kept in arrays of 255 by
// Exploration::PathAPI (game/unit_800D3428.cpp; types in game/PathAPI.h). The end
// is the static initializer at 0x800D50A4; the functions use its 0.0 constant, so
// they belong to this unit and not to the one at 0x800D3428 (whose static
// initializer at 0x800D4C44 has its own constants).
// NonMatching: Unk800D5008::operator= differs (two loop registers); the other
// functions match.

#include "game/unk_vec3.mch"
#include "game/PathAPI.h"
#include "game/unk_vec3.h"

extern "C" void* memset(void* dst, int value, u32 size);

Unk800D4C64::Unk800D4C64() {
    m0 = 0;
    for (int i = 0; i < 4; i++) {
        mPaths[i] = 0;
        mPathIds[i] = 0;
        mLocks[i] = 0;
    }
    m34 = UnkVec3Zero();
    m40 = 1;
    memset(m44, 0, sizeof(m44));
    m50 = 0;
    m54 = 0;
    m58 = 0;
    m5C = 0;
    m60 = 1;
    m64 = 0;
    memset(m68, 0, sizeof(m68));
    m90 = 0;
    m94 = 0;
    m98 = 0;
    m9C = 1;
    mA0 = 0;
    for (int i = 0; i < 64; i++) {
        mA4[i] = 0;
    }
    m1A4 = 0;
    m1A8 = 0;
}

// Straight-line copies up to m64 let the compiler load them all before the first
// store, as in the DOL; m44 and m68 are loops.
Unk800D4C64& Unk800D4C64::operator=(const Unk800D4C64& other) {
    m0 = other.m0;
    mPaths[0] = other.mPaths[0];
    mPathIds[0] = other.mPathIds[0];
    mLocks[0] = other.mLocks[0];
    mPaths[1] = other.mPaths[1];
    mPathIds[1] = other.mPathIds[1];
    mLocks[1] = other.mLocks[1];
    mPaths[2] = other.mPaths[2];
    mPathIds[2] = other.mPathIds[2];
    mLocks[2] = other.mLocks[2];
    mPaths[3] = other.mPaths[3];
    mPathIds[3] = other.mPathIds[3];
    mLocks[3] = other.mLocks[3];
    m34.x = other.m34.x;
    m34.y = other.m34.y;
    m34.z = other.m34.z;
    m40 = other.m40;
    for (int i = 0; i < 3; i++) {
        m44[i] = other.m44[i];
    }
    m50 = other.m50;
    m54 = other.m54;
    m58 = other.m58;
    m5C = other.m5C;
    m60 = other.m60;
    m64 = other.m64;
    for (int i = 0; i < 10; i++) {
        m68[i] = other.m68[i];
    }
    m90 = other.m90;
    m94 = other.m94;
    m98 = other.m98;
    m9C = other.m9C;
    mA0 = other.mA0;
    for (int i = 0; i < 64; i++) {
        mA4[i] = other.mA4[i];
    }
    m1A4 = other.m1A4;
    m1A8 = other.m1A8;
    return *this;
}

// 98.6 %: the DOL keeps the loop counter in r6 and the source pointer in r7, this
// build the other way round (pointer loop, while loop, inline accessors, by-value
// point and member-by-member copies tried).
Unk800D5008& Unk800D5008::operator=(const Unk800D5008& other) {
    mCount = 0;
    for (int i = 0; i < other.mCount; i++) {
        Add(other.mPoints[i]);
    }
    for (int i = 0; i < 2; i++) {
        m4[i] = other.m4[i];
        mC[i] = other.mC[i];
    }
    m0 = other.m0;
    m14 = other.m14;
    return *this;
}
