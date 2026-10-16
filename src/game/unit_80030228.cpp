// Game unit at 0x80030228-0x80030604 (real file name unknown): the SewerItem base
// class (RTTI). Bounds come from the static initializers of this unit and the
// previous one.

#include "game/SewerItem.h"

extern "C" {
u32 fn_80143460(void);
void fn_8019BC80(UnkActor* actor, int arg);
void fn_8018F618(void* obj, int arg);
void fn_80190658(void* obj, u32 hash);
}

SewerItem::SewerItem() {
    m10 = -1;
    m24 = 0;
    mActor = 0;
    mActive = 0;
    mNext = 0;
    mPrev = 0;
    mType = 0;
    mTimer = 0;
    mInterval = 0.0f;
    mRadiusSq = 4096.0f;
}

SewerItem::~SewerItem() {}

void SewerItem::fn_800302B0(UnkActor* actor) {
    mActor = actor;
    m24 = 0;
    mActive = 0;
    mNext = 0;
    mPrev = 0;
    mHome = actor->mPos;
    mTimer = 0;
    mInterval = fn_80143460() % 5 + 8;
}

void SewerItem::fn_80030348(SewerItem* other) {
    SewerItem* next = other->mNext;
    if (next != 0) {
        mNext = next;
        next->mPrev = this;
    }
    other->mNext = this;
    mPrev = other;
}

SewerItem* SewerItem::vf14() {
    SewerItem* next = mNext;
    if (next != 0) {
        next->mPrev = mPrev;
    }
    if (mPrev != 0) {
        mPrev->mNext = mNext;
    }
    mPrev = 0;
    mNext = 0;
    mActive = 0;
    fn_8019BC80(mActor, 0);
    mActor->mPos.x = 690.0f;
    mActor->vf2C(0x18);
    fn_8018F618(mActor->m0, 0);
    return next;
}

void SewerItem::fn_80030414(const UnkVec2* pos, f32 arg) {
    UnkVec3 position(pos->x, pos->y, 10.0f);
    mActor->mPos = position;
    fn_8019BC80(mActor, 0);
    u32 random = fn_80143460();
    m24 = random % 3;
    mActive = 1;
    mActor->vf2C(0x1A);
    vf1C(arg);
}

int SewerItem::vf0C(UnkVec3 pos) {
    UnkVec3 delta = mActor->mPos - pos;
    if (delta.x > 0.0f && delta.LengthSq() < mRadiusSq) {
        return 1;
    }
    return 0;
}

void SewerItem::vf1C(f32 arg) {
    fn_80190658(mActor->m0, 0x4FD5A6A5);
}

void SewerItem::vf18() {}

void SewerItem::vf10() {
    if (mTimer > mInterval) {
        vf18();
        mTimer = 0;
    } else {
        mTimer++;
    }
}
