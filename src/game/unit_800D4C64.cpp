// Game unit at 0x800D4C64-0x800D50C4 (real file name unknown): the constructor and
// copy of a 0x1AC-byte record and the copy of a 0xDC-byte record, both kept in
// arrays of 255 by the unit at 0x800D3428 (not split; real names unknown). The end
// is the static initializer at 0x800D50A4; the functions use its 0.0 constant, so
// they belong to this unit and not to the one at 0x800D3428 (whose static
// initializer at 0x800D4C44 has its own constants).
// NonMatching: Unk800D5008::operator= differs (two loop registers); the other
// functions match.

#include "game/unk_vec3.mch"
#include "game/unk_vec3.h"

extern "C" void* memset(void* dst, int value, u32 size);

class Unk800D4C64 {
public:
    Unk800D4C64();
    Unk800D4C64& operator=(const Unk800D4C64& other);

    /* 0x000 */ u32 m0;
    // Set and copied column by column in the DOL (the order of the stores).
    /* 0x004 */ u32 m4[3][4];
    /* 0x034 */ UnkVec3 m34;
    /* 0x040 */ int m40;
    /* 0x044 */ u32 m44[3];
    /* 0x050 */ u32 m50;
    /* 0x054 */ u32 m54;
    /* 0x058 */ u32 m58;
    /* 0x05C */ u32 m5C;
    /* 0x060 */ int m60;
    /* 0x064 */ u8 m64;
    /* 0x068 */ u32 m68[10];
    /* 0x090 */ u32 m90;
    /* 0x094 */ u32 m94;
    /* 0x098 */ u32 m98;
    /* 0x09C */ u8 m9C;
    /* 0x0A0 */ u32 mA0;
    // A flat word array: the 64-word loops unroll into the DOL's 2 iterations of
    // 32 stores (constructor) and 4 iterations of 16 copies (operator=).
    /* 0x0A4 */ u32 mA4[64];
    /* 0x1A4 */ u32 m1A4;
    /* 0x1A8 */ u32 m1A8;
};

// Point with a flag, 0x10 bytes (real name unknown).
struct Unk800D5008Point {
    /* 0x0 */ UnkVec3 mPos;
    /* 0xC */ u8 mFlag;
};

class Unk800D5008 {
public:
    Unk800D5008& operator=(const Unk800D5008& other);

    void Add(const Unk800D5008Point& point) {
        if (mCount < 12) {
            Unk800D5008Point& dst = mPoints[mCount];
            dst.mPos.x = point.mPos.x;
            dst.mPos.y = point.mPos.y;
            dst.mPos.z = point.mPos.z;
            dst.mFlag = point.mFlag;
            mCount++;
        }
    }

    /* 0x00 */ u32 m0;
    /* 0x04 */ u32 m4;
    /* 0x08 */ u32 m8;
    /* 0x0C */ u32 mC;
    /* 0x10 */ u32 m10;
    /* 0x14 */ u32 m14;
    /* 0x18 */ Unk800D5008Point mPoints[12];
    /* 0xD8 */ int mCount;
};

Unk800D4C64::Unk800D4C64() {
    m0 = 0;
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 3; i++) {
            m4[i][j] = 0;
        }
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
    m4[0][0] = other.m4[0][0];
    m4[1][0] = other.m4[1][0];
    m4[2][0] = other.m4[2][0];
    m4[0][1] = other.m4[0][1];
    m4[1][1] = other.m4[1][1];
    m4[2][1] = other.m4[2][1];
    m4[0][2] = other.m4[0][2];
    m4[1][2] = other.m4[1][2];
    m4[2][2] = other.m4[2][2];
    m4[0][3] = other.m4[0][3];
    m4[1][3] = other.m4[1][3];
    m4[2][3] = other.m4[2][3];
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
// point and member-by-member copies tried). The six words are copied in the DOL's
// load order.
Unk800D5008& Unk800D5008::operator=(const Unk800D5008& other) {
    mCount = 0;
    for (int i = 0; i < other.mCount; i++) {
        Add(other.mPoints[i]);
    }
    m4 = other.m4;
    mC = other.mC;
    m8 = other.m8;
    m10 = other.m10;
    m0 = other.m0;
    m14 = other.m14;
    return *this;
}
