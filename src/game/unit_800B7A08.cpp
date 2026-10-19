// Game unit at 0x800B7A08-0x800B7E34 (real file name unknown): the
// Entity_ArmWrestleHand class. Bounds come from the static initializers of this unit
// and the previous one.

#include "game/unk_vec3.mch"
#include "game/Entity.h"
#include "game/unk_vec3.h"

Entity_ArmWrestleHand::Entity_ArmWrestleHand(int type, EntityInfo* info, int init) : Entity_Prop(type, info, init) {
    m264 = 0.0f;
    mOffset = UnkVec3Zero();
}

Entity_ArmWrestleHand::~Entity_ArmWrestleHand() {
    Detach();
}

void Entity_ArmWrestleHand::fn_800B7B08(int arg, void* boneName) {
    mBone = m94.fn_801EC880(boneName);
    m270 = arg;
}

void Entity_ArmWrestleHand::fn_800B7B50() {
    Detach();
}

void Entity_ArmWrestleHand::vf20(f32 dt, int arg) {
    UnkEntityNode* node;
    for (node = mB0; node != 0; node = node->mNext) {
        if (node->mEntity != 0) {
            node->mEntity->mInfo->mFlags |= 1;
        }
    }
    Entity_Prop::vf20(dt, 0);
    vf74(dt);
    for (node = mB0; node != 0; node = node->mNext) {
        if (node->mEntity != 0) {
            node->mEntity->mInfo->mFlags &= ~1;
        }
    }
}

void Entity_ArmWrestleHand::fn_800B7C64(f32 value) {
    m264 += value;
}

int Entity_ArmWrestleHand::fn_800B7C74(UnkArmWrestler* holder, void* boneName) {
    mHolder = holder;
    int bone = holder->m94.fn_801EC880(boneName);
    if (mOffset == UnkVec3Zero()) {
        mOffset = mHolder->m94.mBones[bone].mPos;
    }
    UnkVec3 target = mOffset - mHolder->mInfo->mPos;
    f32 z = target.z;
    target.z = z - mHolder->fn_800B7464();
    UnkVec3 offset = m94.mBones[mBone].mPos - mInfo->mPos;
    if (mHolder->fn_801B5420(this, m270, target, offset, 1) == 1) {
        mInfo->mFlags &= ~1;
        return 1;
    }
    return 0;
}
