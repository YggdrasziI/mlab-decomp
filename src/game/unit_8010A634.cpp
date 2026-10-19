// Game unit at 0x8010A634-0x8010A678 (real file name unknown). Bounds come from the
// static initializers of this unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/unk_vec3.h"

// One size (in bytes) per id; real name unknown.
u32 lbl_8036CA20[80] = {
    36, 20, 16, 4, 16, 28, 8, 8, 8, 8,
    8, 8, 4, 4, 4, 12, 4, 4, 20, 20,
    32, 4, 12, 12, 12, 16, 16, 12, 20, 12,
    8, 12, 4, 4, 8, 4, 16, 16, 16, 16,
    8, 20, 8, 4, 4, 8, 32, 8, 20, 36,
    372, 52, 52, 44, 16, 4, 4, 4, 8, 4,
    4, 8, 28, 116, 4, 4, 4, 36, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
};

// Used as a function pointer: the size for an id, 0 for an unknown id.
u32 fn_8010A634(u32 id) {
    if (id <= 79) {
        return lbl_8036CA20[id];
    }
    return 0;
}
