// Game unit at 0x800DEC28-0x800DEFB8 (real file name unknown): the CIDynamic camera
// influence, base of most others. Bounds come from the static initializer of this
// unit and the start of the CIFixed_DJ1 unit.

#include "game/CameraInfluence.h"
#include "game/unk_vec3.h"

CIDynamic::CIDynamic() {
    mTimer = mAngle = mBlend = 0.0f;
}

CIDynamic::CIDynamic(int type) {
    mType = type;
    mTimer = mAngle = mBlend = 0.0f;
}

// Types 4, 5, 9 and 14 never hold the blend (meaning unknown).
static inline bool UnkTypeCanHold(int type) {
    return type != 4 && type != 5 && type != 9 && type != 14;
}

static inline bool UnkIsNearZero(f32 x) {
    return x > -0.0001f && x < 0.0001f;
}

// Clamp to [low, high], written as a conditional expression (the UnkClamp form
// allocates registers differently here).
static inline f32 UnkClampExpr(f32 value, f32 low, f32 high) {
    return value < low ? low : (value > high ? high : value);
}

// Clamp between two bounds given in any order.
static inline f32 UnkClampBetween(const f32& value, f32 a, const f32& b) {
    return a < b ? (value < a ? a : (value > b ? b : value))
                 : (value < b ? b : (value > a ? a : value));
}

bool CIDynamic::vf0C(UnkVec3* pos, UnkVec3* target, f32 dt) {
    UnkCameraSubject* subject = ((UnkCameraController*)lbl_803C4298.m8)->mSubject;
    mActive = mGraph.fn_800DE678(*subject->mPos, &mHit, subject->mRadius);

    bool hold = false;
    bool timed = mTimer > 0.0f && !lbl_805FAF64;
    if (timed) {
        if (UnkTypeCanHold(mType)) {
            hold = true;
        }
    }

    mAngle = UnkWrapAngle(mAngle);
    mTimer -= dt;

    if (mBlendTime > 0.0f) {
        if (m54 && mActive && !hold) {
            mBlend += dt;
        } else {
            mBlend -= dt;
        }
        mBlend = UnkClampBetween(mBlend, 0.0f, mBlendTime);
        mProgress = mBlend / mBlendTime;
    } else if (m54 && mActive && !hold) {
        mProgress = UnkClampExpr(mHit.m0 / m48, 0.0f, 1.0f);
    } else {
        mProgress = 0.0f;
    }

    return UnkIsNearZero(mProgress) == false;
}

f32 CIDynamic::vf18() {
    return 1.0f;
}
