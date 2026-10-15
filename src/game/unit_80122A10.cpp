// Game unit at 0x80122A10-0x80122B5C (real file name unknown): reserving and
// releasing a slot in the active entity id table. Bounds come from the static
// initializers of this unit and the previous one.

#include "game/Entity.h"
#include "game/externs.h"
#include "game/unk_vec3.h"

// Large game object in .bss (0x803B53A0, 0xD638 bytes, defined in another unit);
// only the active entity table is used here. Real names unknown.
struct UnkEntityTable {
    /* 0x000 */ u8 unk0[0x1C0];
    /* 0x1C0 */ u8 mCount;
    /* 0x1C4 */ Entity* mIds[255];
};

extern UnkEntityTable lbl_803B53A0;

void UnkEntitySlot::fn_80122A10(Entity* entity) {
    char buf[256];
    u8 count = lbl_803B53A0.mCount;
    u8 slot;
    u8 i;

    for (i = 0; i < count; i++) {
        Entity* entry = lbl_803B53A0.mIds[i];
        if (entry == entity) {
            slot = i;
            goto done;
        }
    }
    for (i = 0; i < count; i++) {
        if (lbl_803B53A0.mIds[i] == 0) {
            lbl_803B53A0.mIds[i] = entity;
            slot = i;
            goto done;
        }
    }
    if (i == count) {
        if (count < 255) {
            lbl_803B53A0.mIds[lbl_803B53A0.mCount++] = entity;
            slot = i;
            goto done;
        }
        fn_80142EF8(buf, "Holy toledo, we have more than %d active entities in this buffer.", 255);
    }
    slot = 255;
done:
    mSlot = slot;
}

void UnkEntitySlot::fn_80122B00(Entity* entity) {
    u8 slot = mSlot;
    lbl_803B53A0.mIds[slot] = 0;
    if (slot == lbl_803B53A0.mCount) {
        lbl_803B53A0.mCount--;
    }
    mSlot = 255;
}
