// Game unit at 0x8006CFA4-0x8006D2D0 (real file name unknown): the
// Entity_Challenge_Projector class. Bounds come from the static initializers of this
// unit and the previous one.

#include "game/Entity.h"
#include "game/externs.h"
#include "game/unk_803B53A0.h"
#include "game/unk_math.h"
#include "game/unk_vec3.h"

Entity_Challenge_Projector::Entity_Challenge_Projector(int type, EntityInfo* info, int init)
    : Entity_Prop(type, info, init) {
    SetType(16);
}

int Entity_Challenge_Projector::vf30(void* arg1, void* arg2) {
    Entity_Prop::vf30(arg1, arg2);
    mInfo->mFlags |= 0x2000000;
    return 1;
}

// NONMATCHING: 99.8 %; the DOL keeps m268 in r4 (and mInfo in r5) where this
// compiles to r0 (and r4). Local variables and member types tried did not change it.
void Entity_Challenge_Projector::vf20(f32 dt, int arg) {
    Entity_Prop::vf20(dt, arg);
    if (m268 != 0) {
        UnkVec3 delta = mInfo->mPos - lbl_803B53A0.mD614->mInfo->mPos;
        if (!mNear) {
            if (delta.Length() < 15.0f && fn_800D5924(fn_800D39BC(lbl_805FB180))) {
                mNear = 1;
                fn_801BA190(lbl_805FB6C8, this, 0, 0x59CFBC0F, 0, 0, 0, 0);
            }
        } else {
            if (delta.Length() >= 15.0f || !fn_800D5924(fn_800D39BC(lbl_805FB180))) {
                mNear = 0;
                fn_801BA190(lbl_805FB6C8, this, 0, 0x0957DD45, 0, 0, 0, 0);
            }
        }
    }
}

void Entity_Challenge_Projector::fn_8006D248(void* target) {
    m268 = target;
}
