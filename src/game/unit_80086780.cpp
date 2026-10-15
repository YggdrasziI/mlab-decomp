// Game unit at 0x80086780-0x80086BE8 (real file name unknown): the
// Entity_BrainTossHand class. Bounds come from the static initializers of this unit
// and the previous one.

#include "game/Entity.h"
#include "game/unk_vec3.h"

Entity_BrainTossHand::Entity_BrainTossHand(int type, EntityInfo* info, int init) : Entity_Prop(type, info, init) {
    mHeld = 0;
    m268 = -1;
}

Entity_BrainTossHand::~Entity_BrainTossHand() {}

int Entity_BrainTossHand::fn_8008682C() {
    return m268;
}

void Entity_BrainTossHand::fn_80086834(Entity* held) {
    mHeld = held;
    m268 = -1;
}

Entity* Entity_BrainTossHand::fn_80086844() {
    return mHeld;
}

UnkVec3 Entity_BrainTossHand::fn_8008684C() {
    return GetHandPosition();
}

// NONMATCHING: 99.9 %; the hand position and the transform temporary swap their
// stack slots (0x8 and 0x28 in the DOL).
void Entity_BrainTossHand::fn_800869A0() {
    if (mHeld != 0) {
        mHeld->SetPosition(GetHandPos());
        mHeld->SetRotation(GetWorldRotation());
    }
}
