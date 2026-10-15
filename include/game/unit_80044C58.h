#ifndef GAME_UNIT_80044C58_H
#define GAME_UNIT_80044C58_H

// Declarations for the game unit at 0x80044C58-0x80044CDC (real file name unknown).

#include "types.h"

// 0x148-byte class without a vtable; fn_80040824 builds an array of two with
// __construct_array. Only the fields the constructor sets are known. Real name
// unknown.
class Unk80044C58 {
public:
    Unk80044C58();
    ~Unk80044C58();

    /* 0x000 */ u8 unk0[0x13C];
    /* 0x13C */ u8 m13C;
    /* 0x13D */ u8 m13D;
    /* 0x13E */ u8 m13E;
    /* 0x13F */ u8 m13F;
    /* 0x140 */ u8 m140;
    /* 0x141 */ u8 m141;
    /* 0x144 */ u32 m144;
};

#endif // GAME_UNIT_80044C58_H
