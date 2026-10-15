// Game unit at 0x800E2FE8-0x800E3300 (real file name unknown): the
// CIFixedTrackPlayer camera influence. Bounds come from the static initializers of
// this unit and the previous one.

#include "game/CameraInfluence.h"
#include "game/unk_math.h"
#include "game/unk_vec3.h"

CIFixedTrackPlayer::CIFixedTrackPlayer(int type) {
    mType = type;
    mProgress = 0.0f;
}

void CIFixedTrackPlayer::vf10() {}

bool CIFixedTrackPlayer::vf0C(UnkVec3* pos, UnkVec3* target, f32 dt) {
    if (!CIDynamic::vf0C(pos, target, dt)) {
        return false;
    }
    UnkCameraState* track = &((UnkCameraController*)lbl_803C4298.m8)->mState;
    f32 progress = mProgress;
    f32 angle = 1.5707964f * (2.0f * progress);
    UnkVec3 start = track->mStart;
    UnkVec3 lookAt = track->mLookAt;
    f32 ease = 0.5f * (1.0f + cosf(angle));
    f32 weight = UnkLerp(1.0f, 0.0f, ease);
    mPos = (mFollow->mPos - start) * weight + start;
    mLookAt = lookAt;
    *pos = mPos;
    *target = mLookAt - mPos;
    *target = target->Normalized();
    return true;
}
