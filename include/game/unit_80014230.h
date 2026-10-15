#ifndef GAME_UNIT_80014230_H
#define GAME_UNIT_80014230_H

// Declarations for the build information units at 0x80014230-0x80014350 (real file
// names unknown).

#include "types.h"

// Build numbers printed by fn_8001426C (519 and 142835); the constants are loaded
// from memory there, so they are defined in another unit. Real names unknown.
extern const int lbl_805FC720;
extern const int lbl_805FC724;

const char* fn_80014230();

// Region of the build (0 = "NA", 1 = "EU"); the three other words are 0xA1A1A1A1,
// 0xA2A2A2A2 and 0xA3A3A3A3, perhaps patched after the build. Real names unknown.
struct UnkBuildInfo {
    /* 0x0 */ u32 unk0;
    /* 0x4 */ u32 unk4;
    /* 0x8 */ u32 unk8;
    /* 0xC */ s32 mRegion;
};

extern UnkBuildInfo lbl_80354868;

s32 fn_8001425C();
void fn_8001426C(char* buf);

int fn_80014328();

#endif // GAME_UNIT_80014230_H
