#ifndef GAME_CAMERAINFLUENCE_H
#define GAME_CAMERAINFLUENCE_H

// Camera influence classes (names from RTTI): CameraInfluence -> CIDynamic ->
// CIStitching, and more. CameraInfluence is abstract (no vtable of its own in the
// DOL); its default virtual functions are emitted at 0x8009F594-0x8009F5AC and
// CIDynamic's implicit destructor at 0x8009E888 (weak copies). Method names unknown
// (vfXX = vtable offset).

#include "game/Entity.h"
#include "game/unk_803C4298.h"
#include "game/unk_vec3.h"
#include "types.h"

// Member of CIDynamic at 0xC (real type unknown; functions at 0x800DE5B4).
class UnkCameraTarget {
public:
    UnkCameraTarget();
    ~UnkCameraTarget();

    /* 0x00 */ u8 unk0[0x14];
};

class CameraInfluence {
public:
    virtual ~CameraInfluence() {}
    // Fills the camera position and target; returns whether it applies.
    virtual bool vf0C(UnkVec3* pos, UnkVec3* target, f32 dt) = 0;
    virtual void vf10();
    virtual f32 vf14() = 0;
    virtual f32 vf18() = 0;
    virtual f32 vf1C();
    virtual int vf20();

    /* 0x00 */ // vtable
    /* 0x04 */ u8 unk4[0x4];
    /* 0x08 */ int mType;
};

// Functions at 0x800DEC28-0x800DEF98 (unit not split yet); implicit destructor.
class CIDynamic : public CameraInfluence {
public:
    CIDynamic();
    CIDynamic(int type);
    virtual bool vf0C(UnkVec3* pos, UnkVec3* target, f32 dt);
    virtual f32 vf14();
    virtual f32 vf18();

    /* 0x0C */ UnkCameraTarget mTarget;
    /* 0x20 */ u8 unk20[0x44 - 0x20];
    /* 0x44 */ f32 mProgress;
    /* 0x48 */ u8 unk48[0x58 - 0x48];
};

// Quaternion helpers at 0x801452A8 and 0x801455E0 (real names unknown).
UnkVec3 fn_801452A8(const UnkQuat* rot);
UnkVec3 fn_801455E0(const UnkQuat* rot);

inline UnkQuat UnkQuatIdentity() {
    UnkQuat q;
    q.x = q.y = q.z = 0.0f;
    q.w = 1.0f;
    return q;
}

class CIStitching : public CIDynamic {
public:
    CIStitching();
    virtual ~CIStitching();
    virtual bool vf0C(UnkVec3* pos, UnkVec3* target, f32 dt);
    virtual f32 vf14();
    virtual f32 vf18();

    void fn_800D0174();
    void fn_800D01C0();

    void ReleaseCamera() { lbl_803C4298.mCamera->mFC = 0; }

    /* 0x58 */ UnkVec3 mPos;
    /* 0x64 */ UnkQuat mRot;
};

// Track data at lbl_803C4298.m8 (real type unknown).
struct UnkTrackData {
    /* 0x00 */ u8 unk0[0x80];
    /* 0x80 */ UnkVec3 mStart;
    /* 0x8C */ u8 unk8C[0x98 - 0x8C];
    /* 0x98 */ UnkVec3 mLookAt;
};

// Object followed by CIFixedTrackPlayer (real type unknown).
struct UnkTrackFollow {
    /* 0x00 */ u8 unk0[0x8];
    /* 0x08 */ UnkVec3 mPos;
};

class CIFixedTrackPlayer : public CIDynamic {
public:
    CIFixedTrackPlayer(int type);
    virtual bool vf0C(UnkVec3* pos, UnkVec3* target, f32 dt);
    virtual void vf10();

    /* 0x58 */ UnkTrackFollow* mFollow;
    /* 0x5C */ UnkVec3 mPos;
    /* 0x68 */ UnkVec3 mLookAt;
};

#endif // GAME_CAMERAINFLUENCE_H
