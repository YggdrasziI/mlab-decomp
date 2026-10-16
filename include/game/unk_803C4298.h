#ifndef GAME_UNK_803C4298_H
#define GAME_UNK_803C4298_H

// Global object at 0x803C4298 (0x168 bytes, real type unknown). Only the fields used
// by decompiled code are known.

#include "game/unk_vec3.h"
#include "types.h"

// Object at lbl_803C4298.m4, probably the camera (real type unknown).
class UnkCamera {
public:
    virtual void vf08();
    // Sets the camera mode (5, 6 and 7 used by script callbacks).
    virtual void vf0C(int mode);
    // Clears the camera mode.
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C(const UnkVec3* pos);
    // Returns to the fixed camera.
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    // Places the camera at eye looking at target.
    virtual void vf2C(const UnkVec3* eye, const UnkVec3* target);
    // Sets the interpolation time.
    virtual void vf30(f32 time);

    /* 0x00 */ // vtable
    /* 0x04 */ UnkVec3* mPos;
    /* 0x08 */ u8 unk8[0xF0 - 0x8];
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
