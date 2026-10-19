// Game unit at 0x801091B4-0x801091EC (real file name unknown). Bounds: the previous
// unit's inline destructors end at 0x801091B4 (its static initializer registers
// them), and this unit's static initializer ends at 0x801091EC.

#include "game/unk_vec3.mch"
#include "game/unk_vec3.h"

// Global block at 0x803C2A20 (real type unknown).
struct UnkGlobals803C2A20 {
    /* 0x000 */ u8 unk0[0xDC0];
    /* 0xDC0 */ void* mDC0;
};
extern UnkGlobals803C2A20 lbl_803C2A20;

// Copy of lbl_803C2A20.mDC0 (real name unknown).
void* lbl_805FB218;

// The path argument (for example "ui\ScreenSets\ScreenSets.csv") is unused.
extern "C" int fn_801091B4(const char* path) {
    lbl_805FB218 = lbl_803C2A20.mDC0;
    return 0;
}
