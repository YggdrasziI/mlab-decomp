#ifndef GAME_UNK_803B53A0_H
#define GAME_UNK_803B53A0_H

// Large game object in .bss at 0x803B53A0 (0xD638 bytes, defined in another unit).
// Only the fields used by decompiled code are known. Real names unknown.

#include "types.h"

class Entity;

struct UnkGameState {
    /* 0x0000 */ u8 unk0[0x140];
    /* 0x0140 */ Entity* m140[32];
    /* 0x01C0 */ u8 mEntityCount;
    /* 0x01C4 */ Entity* mEntities[255];
    /* 0x05C0 */ u8 unk5C0[0x5C8 - 0x5C0];
    /* 0x05C8 */ u8 m5C8[0x7CC - 0x5C8]; // object passed to fn_80012B5C / fn_80012CB0
    /* 0x07CC */ f32 m7CC;
    /* 0x07D0 */ u8 unk7D0[0xD5F0 - 0x7D0];
    /* 0xD5F0 */ u8 mD5F0[0xD614 - 0xD5F0]; // object passed to fn_800D83A8 / fn_800D87EC
    /* 0xD614 */ Entity* mD614;
    /* 0xD618 */ u8 unkD618[0xD638 - 0xD618];
};

extern UnkGameState lbl_803B53A0;

#endif // GAME_UNK_803B53A0_H
