// Game unit at 0x800E3300-0x800E374C (real file name unknown): the CIDirectionGuide
// camera influence. Bounds come from the static initializers of this unit and the
// previous one.

#include "game/unk_vec3.mch"
#include "game/CameraInfluence.h"
#include "game/unk_math.h"
#include "game/unk_vec3.h"

static inline f32 UnkDot(const UnkVec3& a, const UnkVec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline UnkVec3 UnkNeg(const UnkVec3& v) {
    return UnkVec3(-v.x, -v.y, -v.z);
}

// Square root returning a float directly (0 for x <= 0); UnkSqrtf rounds twice.
static inline f32 UnkSqrtfDirect(f32 x) {
    if (x <= 0.0f) {
        return 0.0f;
    }
    double guess = __frsqrte(x);
    guess = 0.5 * (guess * (3.0 - x * guess * guess));
    guess = 0.5 * (guess * (3.0 - x * guess * guess));
    return guess * x;
}

void CIDirectionGuide::vf10() {}

// NONMATCHING: 98.4 %; same instructions, but the float registers differ from the
// negated direction on (and the operand order of the final products follows).
bool CIDirectionGuide::vf0C(UnkVec3* pos, UnkVec3* target, f32 dt) {
    if (!CIDynamic::vf0C(pos, target, dt)) {
        return false;
    }
    if (mActive) {
        UnkCameraController* camera = (UnkCameraController*)lbl_803C4298.m8;
        UnkVec3 start = camera->mState.mStart;
        UnkVec3 lookAt = camera->mState.mLookAt;
        *pos = start;
        *target = lookAt - start;
        *target = target->Normalized();

        UnkVec3 axis(mHit.mAxisX, mHit.mAxisY, mHit.mAxisZ);
        f32 same = UnkDot(axis, *target);
        f32 opposite = UnkDot(axis, UnkNeg(*target));
        if (opposite > same) {
            axis = UnkNeg(axis);
        }
        f32 yaw = UnkWrapAngle(fn_801F60F8(axis.y, axis.x));
        camera = (UnkCameraController*)lbl_803C4298.m8;
        f32 delta = UnkWrapAngle(yaw - camera->mState.mYaw);
        f32 speed = UnkSqrtfDirect(camera->mSubject->m770.LengthSq());
        camera->mState.mYawSpeed += delta * (300.0f * mTurnRate * mProgress * speed) * dt;
    }
    return false;
}

f32 CIDirectionGuide::vf14() {
    return 0.0f;
}
