// Game unit at 0x8006CCC4-0x8006CFA4 (real file name unknown): the
// Entity_Hazard_Prop class. Bounds come from the static initializers of this unit
// and the previous one.

#include "game/Entity.h"
#include "game/externs.h"
#include "game/unk_vec3.h"

Entity_Hazard_Prop::Entity_Hazard_Prop(int type, EntityInfo* info, int init) : Entity_Prop(type, info, init) {
    SetType(15);
}

int Entity_Hazard_Prop::vf30(void* arg1, void* arg2) {
    Entity_Prop::vf30(arg1, arg2);
    m264 = 0.0f;
    mInfo->mFlags |= 0x2000000;
    return 1;
}

void Entity_Hazard_Prop::vf20(f32 dt, int arg) {
    Entity_Prop::vf20(dt, arg);
}

void Entity_Hazard_Prop::fn_8006CD70(void* arg) {
    if (mA8 != 0) {
        ReleaseHandle();
    }
    UnkVec4 pos(mInfo->mPos, 0.0f);
    lbl_8047C760.fn_801C80E0(arg, &pos, &mA8);
}

void Entity_Hazard_Prop::fn_8006CE08() {
    ReleaseHandle();
}

void Entity_Hazard_Prop::vf14(void* arg) {
    if (mA4 != 0 && *mA4 != 0) {
        fn_801BB478(*mA4, 1);
    }
    m268 = 0;
    m264 = 0.0f;
    Entity_Prop::vf14(arg);
}

void Entity_Hazard_Prop::vf18(void* arg) {
    if (mA8 != 0) {
        lbl_8047C760.fn_801C83AC(mA8);
        mA8 = 0;
    }
    Entity_Prop::vf18(arg);
}
