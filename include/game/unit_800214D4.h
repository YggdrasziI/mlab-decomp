#ifndef GAME_UNIT_800214D4_H
#define GAME_UNIT_800214D4_H

// Declarations for the game unit at 0x800214D4-0x80021594 (real file name unknown).

#include "game/unit_8001FCA8.h"

// Holds every UnkInputSequence: eight fixed ones, then a table of sequences picked
// through an index table (no vtable, no RTTI; real name unknown).
class UnkInputSequenceSet {
public:
    /* 0x0000 */ UnkInputSequence mFixed[8];
    /* 0x0240 */ UnkInputSequence mSequences[100];
    /* 0x1E60 */ s32 mIndices[34];
    /* 0x1EE8 */ s32 mCount;

    void fn_800214D4();
    UnkInputSequence* fn_80021550(int id);
};

#endif // GAME_UNIT_800214D4_H
