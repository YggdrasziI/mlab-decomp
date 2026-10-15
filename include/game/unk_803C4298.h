#ifndef GAME_UNK_803C4298_H
#define GAME_UNK_803C4298_H

// Global object at 0x803C4298 (0x168 bytes, real type unknown). Only the fields used
// by decompiled code are known.

#include "game/unk_vec3.h"
#include "types.h"

// Object at lbl_803C4298.m4, probably the camera (real type unknown).
struct UnkCamera {
    /* 0x00 */ u8 unk0[0xF0];
    /* 0xF0 */ UnkVec3 mUp;
    /* 0xFC */ u8 mFC;
};

struct UnkGlobals803C4298 {
    /* 0x00 */ u8 unk0[0x4];
    /* 0x04 */ UnkCamera* mCamera;
    /* 0x08 */ void* m8;
    /* 0x0C */ u8 unkC[0x18 - 0xC];
    /* 0x18 */ int m18; // a mode (5 and 7 tested)
    /* 0x1C */ u8 unk1C[0x38 - 0x1C];
    /* 0x38 */ void* m38;
    /* 0x3C */ u8 unk3C[0x168 - 0x3C];
};

extern UnkGlobals803C4298 lbl_803C4298;

#endif // GAME_UNK_803C4298_H
