// Game unit at 0x8001425C-0x80014328 (real file name unknown): the build region and
// the build string. Bounds come from the static initializers of this unit and the
// previous one.

#include "game/unk_vec3.mch"
#include "game/unit_80014230.h"
#include "game/externs.h"
#include "game/unk_vec3.h"
#include "version.h"

#if REGION_USA
UnkBuildInfo lbl_80354868 = {0xA1A1A1A1, 0xA2A2A2A2, 0xA3A3A3A3, 0};
#else
UnkBuildInfo lbl_80354868 = {0xA1A1A1A1, 0xA2A2A2A2, 0xA3A3A3A3, 1};
#endif

s32 fn_8001425C() {
    return lbl_80354868.mRegion;
}

void fn_8001426C(char* buf) {
    const char* region;
    switch (lbl_80354868.mRegion) {
    default:
        region = "??";
        break;
    case 0:
        region = "NA";
        break;
    case 1:
        region = "EU";
        break;
    }
    const char* type = "REL";
    fn_80142EF8(buf, "%s %d: %s@%d (%s)", region, lbl_805FC720, fn_80014230(), lbl_805FC724, type);
}
