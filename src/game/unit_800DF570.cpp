// Game unit at 0x800DF570-0x800DF630 (real file name unknown):
// CIFixedPlayerTracking_DJ1::vf0C, a fixed camera looking at the player. Bounds come
// from the static initializers of this unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/CameraInfluence.h"
#include "game/unk_vec3.h"

bool CIFixedPlayerTracking_DJ1::vf0C(UnkVec3* pos, UnkVec3* target, f32 dt) {
    lbl_805FB1A1 = 0;
    *pos = mPos;
    UnkVec3 eye = fn_801F5E50(1.3f);
    *target = eye - *pos;
    return true;
}
