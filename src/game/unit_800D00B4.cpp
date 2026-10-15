// Game unit at 0x800D00B4-0x800D02C0 (real file name unknown): the CIStitching
// camera influence. Bounds come from the static initializers of this unit and the
// previous one.

#include "game/CameraInfluence.h"
#include "game/unk_vec3.h"

CIStitching::CIStitching() : CIDynamic(18) {}

CIStitching::~CIStitching() {
    ReleaseCamera();
}

void CIStitching::fn_800D0174() {
    mPos = UnkVec3Zero();
    mRot = UnkQuatIdentity();
}

void CIStitching::fn_800D01C0() {
    ReleaseCamera();
}

bool CIStitching::vf0C(UnkVec3* pos, UnkVec3* target, f32 dt) {
    *pos = mPos;
    *target = fn_801452A8(&mRot);
    lbl_803C4298.mCamera->mFC = 1;
    lbl_803C4298.mCamera->mUp = fn_801455E0(&mRot);
    return true;
}

f32 CIStitching::vf18() {
    return 0.0f;
}

f32 CIStitching::vf14() {
    return 1.0f;
}
