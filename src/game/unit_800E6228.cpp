// Game unit at 0x800E6228-0x800E6360 (real file name unknown): flag helpers on the
// entity at lbl_803E6D00.m0->m8. Bounds come from the static initializers of this
// unit and the previous one.

#include "game/unk_803E6D00.h"
#include "game/unk_vec3.h"

// Returns the bit mask for value (real name unknown; 0x801B94F8).
extern "C" u32 fn_801B94F8(u32* flags, int value);

static inline u32* UnkGetFlags() {
    Entity* entity = lbl_803E6D00.m0->m8;
    if (entity == 0) {
        return 0;
    }
    if (entity->mA4 != 0) {
        return (u32*)entity->mA4[1];
    }
    return 0;
}

extern "C" {

u32* fn_800E6228(void) {
    return UnkGetFlags();
}

void fn_800E6260(int value) {
    u32* flags = UnkGetFlags();
    *flags |= fn_801B94F8(flags, value);
}

void fn_800E62D0(int value) {
    u32* flags = UnkGetFlags();
    *flags &= ~fn_801B94F8(flags, value);
}

} // extern "C"
