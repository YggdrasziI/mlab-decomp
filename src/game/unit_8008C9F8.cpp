// Game unit at 0x8008C9F8-0x8008CBB0 (real file name unknown): the
// Entity_BrazierBowl class. Bounds come from the static initializers of this unit
// and the previous one.

#include "game/Entity.h"
#include "game/externs.h"
#include "game/unk_vec3.h"

Entity_BrazierBowl::Entity_BrazierBowl(int type, EntityInfo* info, int init) : Entity_Prop(type, info, init) {
    mState = 0;
    mValue = 0.0f;
}

Entity_BrazierBowl::~Entity_BrazierBowl() {}

void Entity_BrazierBowl::fn_8008CAA4() {
    mState = 0;
    mValue = 0.0f;
}

void Entity_BrazierBowl::fn_8008CAB8() {}

void Entity_BrazierBowl::fn_8008CABC() {}

void Entity_BrazierBowl::fn_8008CAC0(f32 value) {
    if (value != mValue) {
        mValue = value;
        fn_801BA190(lbl_805FB6C8, this, 0, 0xFCEB7453, 0, 0, 1, 0);
    }
}

void Entity_BrazierBowl::fn_8008CAFC(int state) {
    mState = state;
    fn_801BA190(lbl_805FB6C8, this, 0, 0xE3AB4F6A, 0, 0, 1, 0);
}

int Entity_BrazierBowl::fn_8008CB28() {
    if (mValue <= 15.0f) {
        return 0;
    }
    if (mValue <= 70.0f) {
        return 1;
    }
    return 2;
}

int Entity_BrazierBowl::fn_8008CB64() {
    if (mState <= 1) {
        return 0;
    }
    return mState <= 3 ? 1 : 2;
}
