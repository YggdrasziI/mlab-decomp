// Game unit at 0x800E391C-0x800E3CF4 (real file name unknown): the CITrackTarget
// camera influence, which turns the camera towards a target. Bounds come from the
// static initializers of this unit and the previous one.

#include "game/CameraInfluence.h"
#include "game/unk_math.h"
#include "game/unk_vec3.h"

void CITrackTarget::vf10() {}

// NONMATCHING: 99.4 %; the DOL reads the yaw speed with lfsu (address update) and
// keeps fabs and its rounding in one register.
bool CITrackTarget::vf0C(UnkVec3* pos, UnkVec3* target, f32 dt) {
    if (!CIDynamic::vf0C(pos, target, dt)) {
        return false;
    }
    if (mActive) {
        UnkCameraState* state = &((UnkCameraController*)lbl_803C4298.m8)->mState;
        UnkVec3 dir = mTracked.fn_801ED788() - state->mLookAt;
        dir = dir.Normalized();
        f32 yaw = UnkWrapAngle(fn_801F60F8(dir.y, dir.x));
        UnkCameraController* camera = (UnkCameraController*)lbl_803C4298.m8;
        f32 delta = UnkWrapAngle(yaw - camera->mState.mYaw);
        f32 absDelta = fabsf(delta);
        if (mDeadZone > 0.0f && absDelta > 0.0001f) {
            if (absDelta < mDeadZone) {
                delta = 0.0f;
            } else if (delta < 0.0f) {
                delta += mDeadZone;
            } else {
                delta -= mDeadZone;
            }
        }
        delta *= 100.0f * mTurnRate * mProgress;
        f32 speed = UnkClamp(delta * dt, -10.0f, 10.0f);
        bool apply = false;
        if (speed > 0.0f) {
            if (camera->mState.mYawSpeed < 3.0f * speed) {
                apply = true;
            }
        } else {
            if (camera->mState.mYawSpeed > 3.0f * speed) {
                apply = true;
            }
        }
        if (apply) {
            camera->mState.mYawSpeed += speed;
        }
        ((UnkCameraController*)lbl_803C4298.m8)->mState.fn_800E2E48(mPitch, mProgress);
    }
    return false;
}

f32 CITrackTarget::vf14() {
    return 0.0f;
}

f32 CITrackTarget::vf1C() {
    return mDeadZone;
}
