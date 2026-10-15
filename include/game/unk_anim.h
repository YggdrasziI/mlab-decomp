#ifndef GAME_UNK_ANIM_H
#define GAME_UNK_ANIM_H

// Object embedded in several game classes (0x484 bytes, functions at
// 0x801F8238-0x801F9530, unit not split yet), registered with the manager returned
// by fn_801B3308. Probably an animation player. Real names unknown.

#include "types.h"

// Three-byte argument of UnkAnimPlayer::fn_801F8628 (real type unknown).
struct UnkAnimParam {
    UnkAnimParam(s8 a, u8 b, u8 c) : m0(a), m1(b), m2(c) {}

    s8 m0;
    u8 m1;
    u8 m2;
};

class UnkAnimPlayer {
public:
    UnkAnimPlayer(void* resource);
    ~UnkAnimPlayer();
    void fn_801F8628(UnkAnimParam* param, int arg);
    void fn_801F8F34(f32 value);
    void fn_801F8FA0(f32 value);

    /* 0x00 */ u8 unk0[0x8];
    /* 0x08 */ f32 m8;
    /* 0x0C */ u8 unkC[0x90 - 0xC];
    /* 0x90 */ u8 m90;
    /* 0x94 */ f32 m94;
    /* 0x98 */ u32 m98;
    /* 0x9C */ u8 unk9C[0x484 - 0x9C];
};

class UnkAnimManager {
public:
    void fn_801B3310(UnkAnimPlayer* player);
    void fn_801B3378(UnkAnimPlayer* player);
};

UnkAnimManager* fn_801B3308();

// Global object at 0x803C4298 (0x168 bytes, real type unknown).
struct UnkGlobals803C4298 {
    /* 0x00 */ u8 unk0[0x8];
    /* 0x08 */ void* m8;
    /* 0x0C */ u8 unkC[0x38 - 0xC];
    /* 0x38 */ void* m38;
    /* 0x3C */ u8 unk3C[0x168 - 0x3C];
};

extern UnkGlobals803C4298 lbl_803C4298;

#endif // GAME_UNK_ANIM_H
