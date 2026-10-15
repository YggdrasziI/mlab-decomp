// Game unit at 0x80122A10-0x80122B5C (real file name unknown): reserving and
// releasing a slot in the active entity id table. Bounds come from the static
// initializers of this unit and the previous one.

#include "game/Entity.h"
#include "game/externs.h"
#include "game/unk_803B53A0.h"
#include "game/unk_vec3.h"

void UnkEntitySlot::fn_80122A10(Entity* entity) {
    char buf[256];
    u8 count = lbl_803B53A0.mEntityCount;
    u8 slot;
    u8 i;

    for (i = 0; i < count; i++) {
        Entity* entry = lbl_803B53A0.mEntities[i];
        if (entry == entity) {
            slot = i;
            goto done;
        }
    }
    for (i = 0; i < count; i++) {
        if (lbl_803B53A0.mEntities[i] == 0) {
            lbl_803B53A0.mEntities[i] = entity;
            slot = i;
            goto done;
        }
    }
    if (i == count) {
        if (count < 255) {
            lbl_803B53A0.mEntities[lbl_803B53A0.mEntityCount++] = entity;
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
    lbl_803B53A0.mEntities[slot] = 0;
    if (slot == lbl_803B53A0.mEntityCount) {
        lbl_803B53A0.mEntityCount--;
    }
    mSlot = 255;
}
