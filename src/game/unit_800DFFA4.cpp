// Game unit at 0x800DFFA4-0x800E02A8 (real file name unknown): the CIPlayerOffset
// camera influence (RTTI). Bounds come from the static initializers of this unit
// and the previous one.

#include "game/unk_vec3.mch"
#include "game/CameraInfluence.h"
#include "game/unk_vec3.h"

// Ray query passed to fn_800DA8F0 (real type unknown).
struct UnkRayQuery {
    UnkRayQuery() : mEnd(0.0f, 0.0f, 0.0f) {
        mRadius = 0.0f;
        m1C = 0.0f;
        m20 = 1;
        m24 = 4;
        m28 = 0;
        m2C = 2;
    }

    void Set(const UnkVec3& start, const UnkVec3& end, f32 radius) {
        mStart = start;
        mEnd = end;
        mRadius = radius;
        m20 = 1;
        m1C = 0.0f;
    }

    /* 0x00 */ UnkVec3 mStart;
    /* 0x0C */ UnkVec3 mEnd;
    /* 0x18 */ f32 mRadius;
    /* 0x1C */ f32 m1C;
    /* 0x20 */ u8 m20;
    /* 0x24 */ int m24;
    /* 0x28 */ int m28;
    /* 0x2C */ int m2C;
};

// Result of fn_800DA8F0 (real type unknown).
struct UnkRayHit {
    UnkRayHit() : m0(0), mPos(0.0f, 0.0f, 0.0f) {
        m10 = UnkVec3Zero();
        UnkVec3 zero = UnkVec3Zero();
        m4 = zero;
    }

    /* 0x00 */ u8 m0;
    /* 0x04 */ UnkVec3 m4;
    /* 0x10 */ UnkVec3 m10;
    /* 0x1C */ UnkVec3 mPos;
};

extern "C" {
// Eye point of the camera controller, raised by height (real name unknown).
UnkVec3 fn_800DE28C(UnkCameraController* camera, f32 height);
void fn_800DA8F0(UnkRayQuery* query, UnkRayHit* hit);
}

void CIPlayerOffset::vf10() {}

bool CIPlayerOffset::vf0C(UnkVec3* pos, UnkVec3* target, f32 dt) {
    if (!CIDynamic::vf0C(pos, target, dt)) {
        return false;
    }
    UnkCameraController* camera = (UnkCameraController*)lbl_803C4298.m8;
    UnkVec3 eye = fn_800DE28C(camera, camera->mState.m48);
    mPos = eye + mOffset;
    if (m56) {
        UnkRayQuery query;
        UnkRayHit hit;
        query.Set(eye, mPos, 1.0f);
        fn_800DA8F0(&query, &hit);
        mPos = hit.mPos;
    }
    *pos = mPos;
    *target = eye - mPos;
    *target = target->Normalized();
    return true;
}
