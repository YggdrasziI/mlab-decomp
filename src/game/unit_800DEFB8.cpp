// Game unit at 0x800DEFB8-0x800DF204 (real file name unknown): CIFixed_DJ1::vf0C,
// the camera transition from a start position and rotation to a fixed one. Bounds
// come from the static initializers of this unit and the previous one.

#include "game/CameraInfluence.h"
#include "game/unk_math.h"
#include "game/unk_vec3.h"

bool CIFixed_DJ1::vf0C(UnkVec3* pos, UnkVec3* target, f32 dt) {
    if (lbl_805FB1A1) {
        mTimer -= dt;
        if (mTimer > 0.0f) {
            f32 ease = 0.5f * (1.0f + cosf(1.5707964f * (2.0f * (mTimer / mDuration))));
            f32 weight = UnkLerp(0.0f, 1.0f, ease);
            *pos = mStartPos * (1.0f - weight) + mPos * weight;
            UnkQuat rot = mStartRot.fn_80145F9C(&mRot, weight);
            rot.Normalize();
            *target = fn_801452A8(&rot);
        } else {
            lbl_805FB1A1 = 0;
        }
    }
    if (!lbl_805FB1A1) {
        *pos = mPos;
        *target = mTarget;
    }
    return true;
}
