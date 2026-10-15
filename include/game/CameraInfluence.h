#ifndef GAME_CAMERAINFLUENCE_H
#define GAME_CAMERAINFLUENCE_H

// Camera influence classes (names from RTTI): CameraInfluence -> CIDynamic ->
// CIStitching, and more. CameraInfluence is abstract (no vtable of its own in the
// DOL); its default virtual functions are emitted at 0x8009F594-0x8009F5AC and
// 0x800DC504-0x800DC510, and CIDynamic's implicit destructor at 0x8009E888 (weak
// copies). Method names unknown
// (vfXX = vtable offset).

#include "game/Entity.h"
#include "game/unk_803C4298.h"
#include "game/unk_math.h"
#include "game/unk_vec3.h"
#include "types.h"

// Result of a UnkCameraTarget query (real type unknown). m4 is the best distance
// found so far.
struct UnkCameraTargetHit {
    /* 0x00 */ f32 m0;
    /* 0x04 */ f32 m4;
    /* 0x08 */ u8 unk8[0x14 - 0x8];
};

// Member of CIDynamic at 0xC (real type unknown; functions at 0x800DE5B4).
class UnkCameraTarget {
public:
    UnkCameraTarget();
    ~UnkCameraTarget();

    bool fn_800DE678(UnkVec3 pos, UnkCameraTargetHit* hit, f32 radius);

    /* 0x00 */ u8 unk0[0x14];
};

// Object the camera follows (real type unknown), at UnkCameraController::m2C0.
struct UnkCameraSubject {
    /* 0x00 */ u8 unk0[0x4];
    /* 0x04 */ UnkVec3* mPos;
    /* 0x08 */ u8 unk8[0x14 - 0x8];
    /* 0x14 */ f32 mRadius;
};

// Set to stop CIDynamic from holding its blend (real name unknown).
extern u8 lbl_805FAF64;

class CameraInfluence {
public:
    virtual ~CameraInfluence() {}
    // Fills the camera position and target; returns whether it applies.
    virtual bool vf0C(UnkVec3* pos, UnkVec3* target, f32 dt) = 0;
    virtual void vf10();
    virtual f32 vf14();
    virtual f32 vf18();
    virtual f32 vf1C();
    virtual int vf20();

    /* 0x00 */ // vtable
    /* 0x04 */ u8 unk4[0x4];
    /* 0x08 */ int mType;
};

// Unit 0x800DEC28 (game/unit_800DEC28.cpp); implicit destructor.
class CIDynamic : public CameraInfluence {
public:
    CIDynamic();
    CIDynamic(int type);
    virtual bool vf0C(UnkVec3* pos, UnkVec3* target, f32 dt);
    // Inline; a copy is emitted at 0x8009F5A8.
    virtual f32 vf14() { return mProgress; }
    virtual f32 vf18();

    /* 0x0C */ UnkCameraTarget mGraph;
    /* 0x20 */ UnkCameraTargetHit mHit;
    /* 0x34 */ f32 mAngle;
    /* 0x38 */ u8 unk38[0x4];
    /* 0x3C */ f32 mTimer;
    /* 0x40 */ u8 unk40[0x4];
    /* 0x44 */ f32 mProgress;
    /* 0x48 */ f32 m48;
    /* 0x4C */ f32 mBlendTime;
    /* 0x50 */ f32 mBlend;
    /* 0x54 */ u8 m54;
    /* 0x55 */ u8 mActive;
    /* 0x56 */ u8 unk56[0x58 - 0x56];
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

// Camera state blended from the influences (real type unknown; functions at
// 0x800E2DF4-0x800E2E9C). The distance and pitch are weighted averages.
struct UnkCameraState {
    void fn_800E2DF4(f32 distance, f32 weight);
    void fn_800E2E48(f32 pitch, f32 weight);

    /* 0x00 */ u8 unk0[0x18];
    /* 0x18 */ UnkVec3 mStart;
    /* 0x24 */ u8 unk24[0x30 - 0x24];
    /* 0x30 */ UnkVec3 mLookAt;
    /* 0x3C */ u8 unk3C[0x4C - 0x3C];
    /* 0x4C */ f32 mYaw;
    /* 0x50 */ f32 mYawSpeed;
    /* 0x54 */ u8 unk54[0xC4 - 0x54];
    /* 0xC4 */ f32 mDistance;
    /* 0xC8 */ f32 mDistanceWeight;
    /* 0xCC */ f32 mPitch;
    /* 0xD0 */ f32 mPitchWeight;
};

// Object at lbl_803C4298.m8, probably the camera controller (real type unknown).
struct UnkCameraController {
    /* 0x000 */ u8 unk0[0x68];
    /* 0x068 */ UnkCameraState mState;
    /* 0x13C */ u8 unk13C[0x2C0 - 0x13C];
    /* 0x2C0 */ UnkCameraSubject* mSubject;
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

// Implicit destructor (emitted at the end of its unit).
class CIDist : public CIDynamic {
public:
    virtual bool vf0C(UnkVec3* pos, UnkVec3* target, f32 dt);
    virtual void vf10();
    virtual f32 vf14();

    /* 0x58 */ f32 mDistance;
};

// Implicit destructor (emitted at the end of its unit).
class CIPitch : public CIDynamic {
public:
    virtual bool vf0C(UnkVec3* pos, UnkVec3* target, f32 dt);
    virtual void vf10();
    virtual f32 vf14();

    /* 0x58 */ f32 mPitch;
};

// Waypoint path camera influence. The two static setters are named by the strings
// they are registered under ("CIWaypointPath::StaticSetPosition" and
// "CIWaypointPath::StaticSetOrientation"); their parameter types, the base class and
// the other members are unknown.
class CIWaypointPath {
public:
    static void StaticSetPosition(const UnkVec3* value, CIWaypointPath* path);
    static void StaticSetOrientation(const UnkQuat* value, CIWaypointPath* path);

    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ UnkVec3 mPosition;
    /* 0x18 */ UnkQuat mOrientation;
};

// Set while a CIFixed_DJ1 transition runs (real name unknown).
extern u8 lbl_805FB1A1;

// Its destructor is inline (first copy emitted at 0x800DBBE4).
class CIFixed_DJ1 : public CameraInfluence {
public:
    virtual ~CIFixed_DJ1() {}
    virtual bool vf0C(UnkVec3* pos, UnkVec3* target, f32 dt);

    /* 0x0C */ UnkVec3 mPos;
    /* 0x18 */ UnkVec3 mTarget;
    /* 0x24 */ UnkQuat mRot;
    /* 0x34 */ UnkVec3 mStartPos;
    /* 0x40 */ u8 unk40[0x4C - 0x40];
    /* 0x4C */ UnkQuat mStartRot;
    /* 0x5C */ f32 mTimer;
    /* 0x60 */ f32 mDuration;
};

// Player eye position: the player position raised by height (0x801F5E50; real name
// unknown).
UnkVec3 fn_801F5E50(f32 height);

// Its destructor is inline (first copy emitted at 0x800DBC24).
class CIFixedPlayerTracking_DJ1 : public CameraInfluence {
public:
    virtual ~CIFixedPlayerTracking_DJ1() {}
    virtual bool vf0C(UnkVec3* pos, UnkVec3* target, f32 dt);

    /* 0x0C */ UnkVec3 mPos;
};

// Implicit destructor (emitted at the end of its unit).
class CIFixedEyeLook : public CIDynamic {
public:
    virtual bool vf0C(UnkVec3* pos, UnkVec3* target, f32 dt);
    virtual void vf10();

    /* 0x58 */ UnkTrackFollow* mEye;
    /* 0x5C */ UnkTrackFollow* mLook;
    /* 0x60 */ UnkVec3 mPos;
    /* 0x6C */ UnkVec3 mLookAt;
};

// Angle of (x, y), atan2-like (0x801F60F8; real name unknown).
f32 fn_801F60F8(f32 y, f32 x);

// Wraps an angle to [-pi, pi] (inline helper; real name unknown).
inline f32 UnkWrapAngle(f32 angle) {
    angle = UnkClamp(angle, -6.2831855f, 6.2831855f);
    if (angle < -3.1415927f) {
        angle += 6.2831855f;
    } else if (angle > 3.1415927f) {
        angle -= 6.2831855f;
    }
    return UnkClamp(angle, -3.1415927f, 3.1415927f);
}

// Target followed by CITrackTarget (real type unknown); fn_801ED788 returns its
// position.
struct UnkTrackedTarget {
    UnkVec3 fn_801ED788() const;

    /* 0x0 */ u8 unk0[0x4];
};

// Implicit destructor (emitted at the end of its unit).
class CITrackTarget : public CIDynamic {
public:
    virtual bool vf0C(UnkVec3* pos, UnkVec3* target, f32 dt);
    virtual void vf10();
    virtual f32 vf14();
    virtual f32 vf1C();

    /* 0x58 */ f32 mDeadZone;
    /* 0x5C */ f32 mTurnRate;
    /* 0x60 */ f32 mPitch;
    /* 0x64 */ u8 unk64[0x4];
    /* 0x68 */ UnkTrackedTarget mTracked;
};

#endif // GAME_CAMERAINFLUENCE_H
