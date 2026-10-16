#ifndef GAME_UNK_803E6D00_H
#define GAME_UNK_803E6D00_H

// Global block in .bss at 0x803E6D00 (0x3298 bytes, real type unknown). Only the
// fields used by decompiled code are known.

#include "game/Entity.h"
#include "types.h"

// Element of UnkGlobals803E6D00Sub::m1A4; the array stride is the global
// lbl_805FB94C (real type unknown).
struct UnkWireGroup {
    /* 0x00 */ u8 unk0[0xA0];
    /* 0xA0 */ UnkWire* mWires;
    /* 0xA4 */ u8 mWireCount;
};

// Object pointed to by lbl_803E6D00.m0 (real type unknown).
struct UnkGlobals803E6D00Sub {
    /* 0x000 */ u8 unk0[0x4];
    /* 0x004 */ int m4;
    /* 0x008 */ Entity* m8;
    /* 0x00C */ u8 unkC[0x1A4 - 0xC];
    /* 0x1A4 */ u8* mWireGroups;
    /* 0x1A8 */ u8 unk1A8[0x1B0 - 0x1A8];
    /* 0x1B0 */ struct UnkZoneData** mZones;
};

struct UnkGlobals803E6D00 {
    /* 0x0 */ UnkGlobals803E6D00Sub* m0;
    /* 0x4 */ u8 unk4[0xC - 0x4];
    /* 0xC */ f32 mDeltaTime;
};

extern UnkGlobals803E6D00 lbl_803E6D00;

#endif // GAME_UNK_803E6D00_H
