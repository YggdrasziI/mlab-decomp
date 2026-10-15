// Game unit at 0x800E374C-0x800E3834 (real file name unknown): the CIDist camera influence.
// Bounds come from the static initializers of this unit and the previous one.

#include "game/CameraInfluence.h"
#include "game/unk_vec3.h"

void CIDist::vf10() {}

bool CIDist::vf0C(UnkVec3* pos, UnkVec3* target, f32 dt) {
    if (!CIDynamic::vf0C(pos, target, dt)) {
        return false;
    }
    ((UnkCameraController*)lbl_803C4298.m8)->mState.fn_800E2DF4(mDistance, mProgress);
    return false;
}

f32 CIDist::vf14() {
    return 0.0f;
}
