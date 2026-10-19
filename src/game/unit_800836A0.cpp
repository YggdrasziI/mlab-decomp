// Game unit at 0x800836A0-0x80083A6C (real file name unknown): the
// Entity_BrainTossTarget class. Bounds come from the static initializers of this
// unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/Entity.h"
#include "game/unk_803E6D00.h"
#include "game/unk_math.h"
#include "game/unk_vec3.h"


// Three distance ratios {0.3, 0.6, 0.9} in .rodata, defined in another unit.
extern const f32 lbl_803464E4[3];

// Returns an object with a 2D position at 0x8 (real name and signature unknown).
struct UnkBrainTossSpot {
    /* 0x0 */ u8 unk0[0x8];
    /* 0x8 */ f32 mX;
    /* 0xC */ f32 mY;
};
extern "C" UnkBrainTossSpot* fn_80081A34(Entity_BrainTossTarget* target, UnkVec3* pos);

inline int IsNearZero(f32 x) {
    return x > -0.0001f && x < 0.0001f;
}

inline f32 UnkSqrtf2(f32 x) {
    if (x <= 0.0f) {
        return 0.0f;
    }
    double guess = __frsqrte(x);
    guess = 0.5 * (guess * (3.0 - x * guess * guess));
    guess = 0.5 * (guess * (3.0 - x * guess * guess));
    return (f32)(guess * x);
}

Entity_BrainTossTarget::Entity_BrainTossTarget(int type, EntityInfo* info, int init)
    : Entity_Prop(type, info, init) {
    mThrower = 0;
    mTime = 0.0f;
    mDuration = 0.0f;
    m270 = false;
    mTarget = UnkVec3Zero();
    m280 = 0;
}

Entity_BrainTossTarget::~Entity_BrainTossTarget() {}

void Entity_BrainTossTarget::vfBC(UnkBrainTossThrower* thrower) {
    mThrower = thrower;
    m15C |= 0x400000;
    mTime = 0.0f;
}

void Entity_BrainTossTarget::vfC0() {
    mTime = 0.0f;
}

int Entity_BrainTossTarget::fn_80083798() {
    return vfC4();
}

void Entity_BrainTossTarget::fn_800837A8() {
    f32 dt = lbl_803E6D00.mDeltaTime;
    mTime += dt;
}

int Entity_BrainTossTarget::fn_800837C4() {
    int done;
    if (mTime >= mDuration) {
        mTime = 0.0f;
        done = 1;
    } else {
        done = 0;
    }
    return done;
}

void Entity_BrainTossTarget::fn_800837F4(f32 time) {
    mTime = time;
}

int Entity_BrainTossTarget::fn_800837FC() {
    return vfC8();
}

void Entity_BrainTossTarget::fn_8008380C(int enable) {
    m270 = enable;
}

int Entity_BrainTossTarget::fn_80083820() {
    return m280;
}

void Entity_BrainTossTarget::fn_80083828(int value) {
    m280 = value;
}

bool Entity_BrainTossTarget::fn_80083830() {
    UnkVec3 delta = mTarget - mInfo->mPos;
    f32 distSq = delta.LengthSq();
    if (IsNearZero(distSq)) {
        mInfo->mPos = mTarget;
        mInfo->mFlags |= 0x8000;
        mInfo->mFlags |= 0x10000000;
        mInfo->mFlags |= 0x4000;
        mInfo->mFlags |= 0x10000000;
    }
    return IsNearZero(distSq);
}

int Entity_BrainTossTarget::fn_80083930(UnkVec3* pos) {
    UnkBrainTossSpot* spot = fn_80081A34(this, pos);
    UnkVec3 delta;
    delta.x = pos->x - spot->mX;
    delta.y = pos->y - spot->mY;
    delta.z = 0.0f;
    f32 ratio = UnkSqrtf2(delta.LengthSq()) / mThrower->mRange;
    if (fabsf(lbl_803464E4[0] - ratio) < 0.05) {
        return 0;
    }
    if (fabsf(lbl_803464E4[2] - ratio) < 0.05) {
        return 2;
    }
    return 1;
}
