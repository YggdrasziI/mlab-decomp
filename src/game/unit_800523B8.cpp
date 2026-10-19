// Game unit at 0x800523B8-0x80052504 (real file name unknown): a 32-bit key for an
// id. Bounds come from the static initializers of this unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/unk_vec3.h"

// 41 keys in another unit's data (0x803881B4); real name unknown.
extern u32 lbl_803881B4[];

// Called with the object at lbl_805FBBC0, which is unused here (real name and
// signature unknown). Modes 2 and 3 use fixed key pairs for ids 30 to 38.
u32 fn_800523B8(void* obj, int id, int mode) {
    if (mode != 3 && mode != 2) {
        if (id != -1 && id < 40) {
            return lbl_803881B4[id];
        }
        return 0;
    }

    switch (id) {
    case 30:
        return mode == 3 ? 0x163C6F3E : 0xCE4CC037;
    case 31:
    case 37:
    case 38:
        return mode == 3 ? 0xF913A97B : 0xFBDEF319;
    case 32:
        return mode == 3 ? 0x16DBEE1F : 0xD55155FF;
    case 33:
        return mode == 3 ? 0x1C85365D : 0xC0F0855F;
    case 34:
        return mode == 3 ? 0xA7D952F2 : 0x21EC130F;
    case 35:
        return mode == 3 ? 0x1972162F : 0xBACCDB01;
    case 36:
        return mode == 3 ? 0xCF16E957 : 0x0C9C52B7;
    }
    return 0;
}
