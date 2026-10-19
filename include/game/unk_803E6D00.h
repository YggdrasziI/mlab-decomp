#ifndef GAME_UNK_803E6D00_H
#define GAME_UNK_803E6D00_H

// Global block in .bss at 0x803E6D00 (0x3298 bytes, real type unknown). Only the
// fields used by decompiled code are known.

#include "game/Entity.h"
#include "types.h"

// Element of UnkGlobals803E6D00Sub::m1A4; the array stride is the global
// lbl_805FB94C (real type unknown).
struct UnkWireGroup {
    /* 0x000 */ u8 unk0[0x94];
    /* 0x094 */ int mEntityCount2;
    /* 0x098 */ int mEntityCount;
    /* 0x09C */ u8 unk9C[0xA0 - 0x9C];
    /* 0x0A0 */ UnkWire* mWires;
    /* 0x0A4 */ u8 mWireCount;
    /* 0x0A5 */ u8 unkA5[0x120 - 0xA5];
    /* 0x120 */ Entity** mEntities2;
    /* 0x124 */ u8 unk124[0x12C - 0x124];
    /* 0x12C */ Entity** mEntities;
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
    /* 0x1B4 */ u8 unk1B4[0x1E0 - 0x1B4];
    /* 0x1E0 */ int m1E0;
    /* 0x1E4 */ u8 unk1E4[0x1E8 - 0x1E4];
    /* 0x1E8 */ u32 m1E8;
    /* 0x1EC */ u8 unk1EC[0x1F4 - 0x1EC];
    /* 0x1F4 */ struct UnkStateStack* m1F4;
};

struct UnkGlobals803E6D00 {
    /* 0x0 */ UnkGlobals803E6D00Sub* m0;
    /* 0x4 */ u8 unk4[0xC - 0x4];
    /* 0xC */ f32 mDeltaTime;
    /* 0x10 */ u8 unk10[0xE0 - 0x10];
    /* 0xE0 */ struct UnkZoneRef* mE0; // current zone (zone id at 0x18)
};

extern UnkGlobals803E6D00 lbl_803E6D00;

#endif // GAME_UNK_803E6D00_H
