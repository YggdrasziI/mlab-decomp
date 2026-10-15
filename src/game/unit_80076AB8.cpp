// Game unit at 0x80076AB8-0x80076D38 (real file name unknown): the
// Entity_SpiritToken class. Bounds come from the static initializers of this unit
// and the previous one.

#include "game/Entity.h"
#include "game/externs.h"
#include "game/unk_vec3.h"

Entity_SpiritToken::Entity_SpiritToken(int type, EntityInfo* info, int init) : Entity_Prop(type, info, init) {
    mMoving = 0;
}

Entity_SpiritToken::~Entity_SpiritToken() {}

void Entity_SpiritToken::fn_80076B5C(f32 duration, UnkPath* path) {
    mPath = path;
    mDuration = duration;
    mTime = 0.0f;
    mMoving = 1;
    fn_801BA190(lbl_805FB6C8, this, 0, 0x4608BB44, 0, 0, 1, 0);
}

u8 Entity_SpiritToken::fn_80076B9C() {
    return mMoving;
}

void Entity_SpiritToken::fn_80076BA4(f32 dt) {
    mTime += dt;
    f32 t = mTime / mDuration;
    if (t > 1.0f) {
        t = 1.0f;
    }
    mInfo->mPos = mPath->fn_8010B200(t);
    mInfo->mFlags |= 0x8000;
    mInfo->mFlags |= 0x10000000;
    mInfo->mFlags |= 0x4000;
    mInfo->mFlags |= 0x10000000;
    if (t >= 1.0f) {
        mMoving = 0;
        fn_801BA190(lbl_805FB6C8, this, 0, 0x3B46FE41, 0, 0, 1, 0);
        mInfo->mPos = mPath->fn_8010B200(0.0f);
        mInfo->mFlags |= 0x8000;
        mInfo->mFlags |= 0x10000000;
        mInfo->mFlags |= 0x4000;
        mInfo->mFlags |= 0x10000000;
    }
}
