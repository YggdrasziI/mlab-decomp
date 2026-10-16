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
    /* 0x05C0 */ u8 unk5C0[0xD614 - 0x5C0];
    /* 0xD614 */ Entity* mD614;
    /* 0xD618 */ u8 unkD618[0xD638 - 0xD618];
};

extern UnkGameState lbl_803B53A0;

#endif // GAME_UNK_803B53A0_H
