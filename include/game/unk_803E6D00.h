#ifndef GAME_UNK_803E6D00_H
#define GAME_UNK_803E6D00_H

// Global block in .bss at 0x803E6D00 (0x3298 bytes, real type unknown). Only the
// fields used by decompiled code are known.

#include "game/Entity.h"
#include "types.h"

// Object pointed to by lbl_803E6D00.m0 (real type unknown).
struct UnkGlobals803E6D00Sub {
    /* 0x0 */ u8 unk0[0x8];
    /* 0x8 */ Entity* m8;
};

struct UnkGlobals803E6D00 {
    /* 0x0 */ UnkGlobals803E6D00Sub* m0;
    /* 0x4 */ u8 unk4[0xC - 0x4];
    /* 0xC */ f32 mDeltaTime;
};

extern UnkGlobals803E6D00 lbl_803E6D00;

#endif // GAME_UNK_803E6D00_H
