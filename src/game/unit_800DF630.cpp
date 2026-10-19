// Game unit at 0x800DF630-0x800DFFA4 (real file name unknown): the CIFree camera
// influence. Bounds come from the static initializers of this unit and the previous
// one. NOT FINISHED: vf0C (0x800DFC44) and fn_800DF7C4 are not written yet.

#include "game/unk_vec3.mch"
#include "game/CameraInfluence.h"
#include "game/unk_vec3.h"
#include <math.h>

// Square root returning a float directly (0 for x <= 0).
static inline f32 UnkSqrtfDirect(f32 x) {
    if (x <= 0.0f) {
        return 0.0f;
    }
    double guess = __frsqrte(x);
    guess = 0.5 * (guess * (3.0 - x * guess * guess));
    guess = 0.5 * (guess * (3.0 - x * guess * guess));
    return guess * x;
}

void CIFree::vf24(const UnkVec3* pos, const UnkQuat* rot) {
    UnkVec3 forward = fn_801452A8(rot);
    mYaw = atan2f(forward.y, forward.x);
    f32 length = UnkSqrtfDirect(forward.LengthSq());
    if (length < 0.001f) {
        mPitch = 0.0f;
        mYaw = 0.0f;
    } else {
        f32 sine = forward.z / length;
        if (sine < -1.0f) {
            mPitch = -3.1415927f;
        }
        if (sine > 1.0f) {
            mPitch = 3.1415927f;
        } else {
            mPitch = asin(sine);
        }
    }
    mPos = *pos;
    f32 scale = 0.8f;
    mPos += forward * scale;
    m20 = UnkVec3Zero();
}
