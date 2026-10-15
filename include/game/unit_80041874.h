#ifndef GAME_UNIT_80041874_H
#define GAME_UNIT_80041874_H

// Declarations for the game unit at 0x80041874-0x800418E0 (real file name unknown).

#include "types.h"

// Small class without a vtable: the constructor clears one byte and the destructor
// is empty. Real name unknown.
class Unk80041874 {
public:
    Unk80041874();
    ~Unk80041874();

    /* 0x0 */ u8 m0;
};

#endif // GAME_UNIT_80041874_H
