#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

// Entity class family (names from RTTI): Entity -> Entity_Game -> Entity_Static ->
// Entity_Sensor -> Entity_Trigger_Script, and more. Entity's functions are at
// 0x801B4334-0x801B6E94 (unit not split yet); Entity_Game is in the game unit at
// 0x80062C80-0x8006307C. Real file names unknown; method names and most signatures
// unknown (vfXX = vtable offset, parameters follow the register use).
//
// Several Entity virtual functions are emitted in other units (0x8000C7A0 (vf44,
// defined in game/unit_8000C478.cpp),
// 0x800084A0, 0x80058B5C, 0x80015800, 0x80050588-0x800505A0, 0x8004B570-0x8004B57C):
// they are probably inline functions emitted where first used. They are declared
// out of line here until those units are decompiled.

#include "game/MotionRecognizer.h"
#include "game/unk_vec3.h"
#include "types.h"

class Entity;

// Pointer list (real name unknown): data at 0xC, count at 0x10.
struct UnkList {
    void* First() { return mCount == 0 ? 0 : mData[0]; }

    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ void** mData;
    /* 0x10 */ u32 mCount;
};

// Four-float rotation, probably a quaternion (the Entity constructor sets it to
// (0, 0, 0, 1)). Real name unknown.
struct UnkQuat {
    UnkQuat() {}
    UnkQuat(const UnkQuat& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}

    // Interpolation towards other (0x80145F9C; probably a slerp).
    UnkQuat fn_80145F9C(const UnkQuat* other, f32 t) const;

    f32 LengthSq() const { return x * x + y * y + z * z + w * w; }
    UnkQuat Normalized() const {
        f32 inv = UnkInvSqrtf(LengthSq());
        UnkQuat result;
        result.x = x * inv;
        result.y = y * inv;
        result.z = z * inv;
        result.w = w * inv;
        return result;
    }
    void Normalize() { *this = Normalized(); }

    f32 x, y, z, w;
};

// 3x4 transform: three basis vectors then the translation (real name unknown).
struct UnkMtx34 {
    UnkVec3 Transform(const UnkVec3& v) const {
        UnkVec3 result;
        result.x = m[0] * v.x + m[3] * v.y + m[6] * v.z + m[9];
        result.y = m[1] * v.x + m[4] * v.y + m[7] * v.z + m[10];
        result.z = m[2] * v.x + m[5] * v.y + m[8] * v.z + m[11];
        return result;
    }
    // Same product written through a pointer: no temporary, so the caller's stack
    // slots and scheduling differ (the cursor functions of the code entry screens).
    void Transform(const UnkVec3& v, UnkVec3* out) const {
        out->x = m[0] * v.x + m[3] * v.y + m[6] * v.z + m[9];
        out->y = m[1] * v.x + m[4] * v.y + m[7] * v.z + m[10];
        out->z = m[2] * v.x + m[5] * v.y + m[8] * v.z + m[11];
    }

    f32 m[12];
};

// Model data returned through Entity::vf10 (real names unknown).
struct UnkJoint {
    /* 0x00 */ u8 unk0[0x24];
    /* 0x24 */ UnkVec3 mPos;
};

struct UnkJointName {
    /* 0x0 */ u32 mHash;
    /* 0x4 */ u32 m4;
};

struct UnkModelData {
    int FindJoint(u32 hash) {
        for (int i = 0; i < mJointNameCount; i++) {
            if (mJointNames[i].mHash == hash) {
                return i;
            }
        }
        return -1;
    }

    /* 0x0 */ u32 m0;
    /* 0x4 */ UnkJoint* mJoints;
    /* 0x8 */ int mJointNameCount;
    /* 0xC */ UnkJointName* mJointNames;
};

struct UnkModel {
    int FindJoint(u32 hash) {
        for (int i = 0; i < mData->mJointNameCount; i++) {
            if (mData->mJointNames[i].mHash == hash) {
                return i;
            }
        }
        return -1;
    }

    /* 0x0 */ UnkModelData* mData;
};

// Per-entity state block pointed to by Entity::mInfo (real name unknown).
struct EntityInfo {
    /* 0x00 */ UnkVec3 mPos;
    /* 0x0C */ UnkQuat mRot;
    /* 0x1C */ u32 mTypeMask;
    /* 0x20 */ u32 mFlags;
    /* 0x24 */ u8 unk24[0x30 - 0x24];
    /* 0x30 */ u16 m30;
    /* 0x32 */ u16 m32;
    /* 0x34 */ u8 mType;
    /* 0x35 */ u8 unk35[0x3A - 0x35];
    /* 0x3A */ u8 m3A;
};

// 0x20-byte record kept twice in UnkEntitySlot (real name unknown).
struct UnkEntitySlotData {
    /* 0x00 */ u32 m0[8];
};

// Slot of an entity in the active entity table (see unit_80122A10; real name
// unknown). Entity_Game::vfB8 returns it.
class UnkEntitySlot {
public:
    void Clear() {
        mCur.m0[0] = mCur.m0[1] = mCur.m0[2] = mCur.m0[3] = mCur.m0[4] = mCur.m0[5] =
            mCur.m0[6] = mCur.m0[7] = mPrev.m0[0] = mPrev.m0[1] = mPrev.m0[2] = mPrev.m0[3] =
                mPrev.m0[4] = mPrev.m0[5] = mPrev.m0[6] = mPrev.m0[7] = 0;
    }
    void Reset() {
        Clear();
        mSlot = 255;
        m41 = 0;
    }

    void fn_80122A10(Entity* entity);
    void fn_80122B00(Entity* entity);

    /* 0x00 */ UnkEntitySlotData mCur;
    /* 0x20 */ UnkEntitySlotData mPrev;
    /* 0x40 */ u8 mSlot;
    /* 0x41 */ u8 m41;
};

// Animation objects at Entity::m9C and Entity::mA0 (real types unknown).
struct UnkEntityAnim {
    /* 0x00 */ u8 unk0[0x8];
    /* 0x08 */ u32 m8; // hash of the sensor type
    /* 0x0C */ u8 unkC[0xB4 - 0xC];
    /* 0x0B4 */ void* mB4;
};

struct UnkEntityAnimState {
    /* 0x00 */ u8 unk0[0xF0];
    /* 0xF0 */ u32 mFlags;
};

// Bone of an entity model (0xC8 bytes, real name unknown).
struct UnkBone {
    /* 0x00 */ u32 m0;
    /* 0x04 */ u8 unk4[0x5C - 0x4];
    /* 0x5C */ UnkVec3 mPos;
    /* 0x68 */ u8 unk68[0xC8 - 0x68];
};

// Member of Entity at 0x94, probably the model or skeleton (real type unknown; the
// Entity constructor clears its first two words). Its size is unknown.
class UnkEntity94 {
public:
    void fn_801EC080(Entity* entity);
    // Index of a bone (the argument is probably its name).
    int fn_801EC880(void* name);

    /* 0x0 */ u32 m0;
    /* 0x4 */ UnkBone* mBones;
};

// Node of the entity list at Entity::mB0 (real name unknown).
struct UnkEntityNode {
    /* 0x0 */ u32 m0;
    /* 0x4 */ Entity* mEntity;
    /* 0x8 */ UnkEntityNode* mNext;
};

class Entity {
public:
    Entity(int type, EntityInfo* info, int init);
    virtual ~Entity();
    virtual void* vf0C();
    virtual UnkModel* vf10();
    virtual void vf14(void* arg);
    virtual void vf18(void* arg);
    virtual void vf1C();
    virtual void vf20(f32 dt, int arg);
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C();
    virtual int vf30(void* arg1, void* arg2);
    virtual void vf34();
    virtual void vf38();
    virtual void vf3C();
    virtual void vf40();
    // Point of this entity seen from another one (the base returns m18; Entity_Prop
    // ignores the arguments too).
    virtual UnkVec3* vf44(const UnkVec3* from, Entity* fromEntity);
    virtual void vf48();
    virtual int vf4C();
    virtual void vf50(Entity* source, int arg2, int arg3, f32 arg4, UnkVec3* pos);
    virtual void vf54(void* arg);
    virtual void vf58();
    virtual int vf5C();
    virtual int vf60();
    virtual int vf64();
    virtual int vf68();
    virtual int vf6C();
    virtual int vf70();
    virtual void vf74(f32 dt);
    virtual void vf78(void* arg);
    virtual void vf7C();
    virtual void vf80();
    virtual void vf84();
    virtual void vf88();
    virtual void vf8C();
    virtual void vf90();
    virtual void vf94();
    virtual void vf98();
    virtual void vf9C();
    virtual void vfA0();
    virtual void vfA4();
    virtual void vfA8();
    virtual void vfAC();
    virtual void vfB0();
    virtual int vfB4();

    void fn_801B50B0(Entity* child);
    UnkMtx34* fn_801B50FC();

    void SetPosition(const UnkVec3& pos) {
        mInfo->mPos = pos;
        mInfo->mFlags |= 0x8000;
        mInfo->mFlags |= 0x10000000;
        mInfo->mFlags |= 0x4000;
        mInfo->mFlags |= 0x10000000;
    }
    void SetRotation(const UnkQuat& rot) {
        mInfo->mRot = rot;
        mInfo->mFlags |= 0x8000;
        mInfo->mFlags |= 0x10000000;
        mInfo->mFlags |= 0x4000;
        mInfo->mFlags |= 0x10000000;
    }
    UnkQuat GetWorldRotation() {
        fn_801B50FC();
        return mInfo->mRot;
    }
    u32 fn_801B5420(Entity* child, int arg, UnkVec3 target, UnkVec3 offset, int arg5);

    void SetType(int type) {
        if (mInfo != 0) {
            mInfo->mTypeMask |= 1 << type;
            mInfo->mType = type;
        }
    }

    /* 0x000 */ // vtable
    /* 0x004 */ EntityInfo* mInfo;
    /* 0x008 */ u8 unk8[0x18 - 0x8];
    /* 0x018 */ UnkVec3 m18; // returned by vf44
    /* 0x024 */ f32 m24; // the frame time, added to timers
    /* 0x028 */ f32 m28; // the current time (timers store m28 + delay)
    /* 0x02C */ u32 m2C;
    /* 0x030 */ u8 unk30[0x88 - 0x30];
    /* 0x088 */ void* m88;
    /* 0x08C */ UnkList* m8C;
    /* 0x090 */ void* m90;
    /* 0x094 */ UnkEntity94 m94;
    /* 0x09C */ UnkEntityAnim* m9C;
    /* 0x0A0 */ UnkEntityAnimState* mA0;
    /* 0x0A4 */ void** mA4;
    /* 0x0A8 */ int mA8;
    /* 0x0AC */ u8 unkAC[0xB0 - 0xAC];
    /* 0x0B0 */ UnkEntityNode* mB0;
    /* 0x0B4 */ u8 unkB4[0x14C - 0xB4];
};

class Entity_Game : public Entity {
public:
    Entity_Game(int type, EntityInfo* info, int init);
    virtual ~Entity_Game();
    virtual void vf14(void* arg);
    virtual void vf18(void* arg);
    virtual void vf20(f32 dt, int arg);
    virtual int vf30(void* arg1, void* arg2);
    virtual void vf40();
    virtual void vf78(void* arg);
    virtual UnkEntitySlot* vfB8();

    // Whether these fields belong to Entity or Entity_Game is unknown.
    /* 0x14C */ u32 m14C;
    /* 0x150 */ u8 m150;
    /* 0x154 */ s32 m154;
};

// The destructor is implicit: the original emits it in the Entity_Light unit
// (0x800630E0).
class Entity_Static : public Entity_Game {
public:
    Entity_Static(int type, EntityInfo* info, int init);
    virtual void vf20(f32 dt, int arg);
    virtual void vf40();
};

// Unit 0x8006307C (game/unit_8006307C.cpp); implicit destructor (emitted at the end
// of its unit).
class Entity_Light : public Entity_Static {
public:
    Entity_Light(int type, EntityInfo* info, int init);
    virtual void vf14(void* arg);
    virtual void vf18(void* arg);
    virtual void vf20(f32 dt, int arg);
    virtual void vf40() { Entity::vf40(); }
};

class Entity_Dynamic : public Entity_Game {
public:
    Entity_Dynamic(int type, EntityInfo* info, int init);
    virtual ~Entity_Dynamic();
    virtual void vf20(f32 dt, int arg);
    virtual void vf40();
};

// Functions at 0x8006D2D0-0x8006DD00 (unit not split yet).
class Entity_Sensor : public Entity_Static {
public:
    Entity_Sensor(int type, EntityInfo* info, int init);
    virtual ~Entity_Sensor();
    virtual void vf18(void* arg);
    virtual void vf1C();
    virtual void vf20(f32 dt, int arg);
    virtual int vf30(void* arg1, void* arg2);
    virtual void vf40();
    // New virtual function in this class.
    virtual void vfBC();
};

class Entity_Trigger_Script : public Entity_Sensor {
public:
    Entity_Trigger_Script(int type, EntityInfo* info, int init);
    virtual ~Entity_Trigger_Script();
};

// Functions at 0x80069028-0x8006C000 (unit not split yet). The destructor is
// implicit (derived destructors inline it). vfB0 and vfB8 are emitted at
// 0x800505B0 and 0x800505A8, probably inline functions (vfB8 returns &mSlot).
class Entity_Prop : public Entity_Static {
public:
    Entity_Prop(int type, EntityInfo* info, int init);
    virtual void vf14(void* arg);
    virtual void vf18(void* arg);
    virtual void vf1C();
    virtual void vf20(f32 dt, int arg);
    virtual void vf28();
    virtual int vf30(void* arg1, void* arg2);
    virtual void vf40();
    virtual UnkVec3* vf44(const UnkVec3* from, Entity* fromEntity);
    virtual int vf4C();
    virtual void vf50(Entity* source, int arg2, int arg3, f32 arg4, UnkVec3* pos);
    virtual void vf54(void* arg);
    virtual void vf58();
    virtual void vf74(f32 dt);
    virtual void vf80();
    virtual void vf84();
    virtual void vf88();
    virtual void vf90();
    virtual void vfB0();
    virtual UnkEntitySlot* vfB8();

    // Script parameters: their meaning depends on the callbacks that use them.
    /* 0x158 */ f32 m158;
    /* 0x15C */ union {
        u32 m15C;
        f32 m15CFloat;
    };
    /* 0x160 */ f32 m160;
    /* 0x164 */ f32 m164; // death count (callbacks "reset_deathcount", "increment_deathcount")
    /* 0x168 */ int m168;
    /* 0x16C */ int m16C;
    /* 0x170 */ int m170;
    /* 0x174 */ int m174;
    /* 0x178 */ u8 unk178[0x184 - 0x178];
    /* 0x184 */ u32 m184;
    /* 0x188 */ u32 m188;
    /* 0x18C */ u8 unk18C[0x1AC - 0x18C];
    /* 0x1AC */ int m1AC;
    /* 0x1B0 */ u8 unk1B0[0x1C8 - 0x1B0];
    /* 0x1C8 */ UnkEntitySlot mSlot;
    /* 0x20C */ u8 unk20C[0x264 - 0x20C];
};

class Entity_ArmWrestleBolt : public Entity_Prop {
public:
    Entity_ArmWrestleBolt(int type, EntityInfo* info, int init);
    virtual ~Entity_ArmWrestleBolt();

    void fn_800B79D8(f32 value);

    /* 0x264 */ f32 m264;
};

class ExWorldTrackItem : public Entity_Prop {
public:
    ExWorldTrackItem(int type, EntityInfo* info, int init);
    virtual ~ExWorldTrackItem();
    virtual void vfBC();
    virtual void vfC0();
    virtual void vfC4();
    virtual void vfC8();

    /* 0x264 */ f32 m264;
    /* 0x268 */ u8 m268;
    /* 0x26C */ u32 m26C;
    /* 0x270 */ u8 m270;
};

// Functions at 0x800C13D8-0x800C1DC0 (unit not split yet).
class ExWorldTile : public Entity_Prop {
public:
    ExWorldTile(int type, EntityInfo* info, int init);
    virtual ~ExWorldTile();
    virtual void vfBC();
    virtual void vfC0();
    virtual void vfC4();
    virtual void vfC8();
    virtual void vfCC();
    virtual f32 vfD0() = 0;
};

// Functions at 0x800C4530-0x800C5100 (unit not split yet).
class ExWorldCurvedTile : public ExWorldTile {
public:
    ExWorldCurvedTile(int type, EntityInfo* info, int init);
    virtual ~ExWorldCurvedTile();
    virtual void vfBC();
    virtual f32 vfD0();
    virtual void vfD4();
    virtual UnkVec3 vfD8(f32 t);
};

// Wire segment of a level element (real type unknown; 0x4C bytes).
// Point of a wire (0x28 bytes, real name unknown).
struct UnkWirePoint {
    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ UnkVec3 mPos;
    /* 0x18 */ u8 unk18[0x28 - 0x18];
};

struct UnkWire {
    /* 0x00 */ UnkWirePoint* mPoints;
    /* 0x04 */ u8 unk4[0x24 - 0x4];
    /* 0x24 */ int mPointCount;
    /* 0x28 */ u8 unk28[0x2C - 0x28];
    /* 0x2C */ u32 mId;
    /* 0x30 */ u32 mFlags;
    /* 0x34 */ u8 unk34[0x4C - 0x34];
};

// Game unit at 0x800C407C-0x800C42B8.
class ExWorldWireTile : public ExWorldTile {
public:
    ExWorldWireTile(int type, EntityInfo* info, int init);
    virtual ~ExWorldWireTile();
    virtual void vfBC();
    virtual void vfC0();
    virtual f32 vfD0();

    /* 0x264 */ u8 unk264[0x2A0 - 0x264]; // owner class unknown
    /* 0x2A0 */ UnkWire* mWire;
};

class Entity_AstralTile : public ExWorldCurvedTile {
public:
    Entity_AstralTile(int type, EntityInfo* info, int init);
    virtual ~Entity_AstralTile();
    virtual void vfBC();
    virtual void vfC0();
    virtual void vfC4();
    virtual void vfC8();
    virtual void vfCC();
};

class Entity_BrazierBowl : public Entity_Prop {
public:
    Entity_BrazierBowl(int type, EntityInfo* info, int init);
    virtual ~Entity_BrazierBowl();

    void fn_8008CAA4();
    void fn_8008CAB8();
    void fn_8008CABC();
    void fn_8008CAC0(f32 value);
    void fn_8008CAFC(int state);
    int fn_8008CB28();
    int fn_8008CB64();

    /* 0x264 */ int mState;
    /* 0x268 */ f32 mValue;
};

// Straight piece of an UnkPath (0x1C bytes, real name unknown).
struct UnkPathSegment {
    /* 0x00 */ UnkVec3 mStart;
    /* 0x0C */ UnkVec3 mDir;
    /* 0x18 */ f32 mLength;
};

// Polyline built from a wire, followed by Entity_SpiritToken (real name unknown;
// unit at 0x8010B16C).
class UnkPath {
public:
    UnkPath();
    ~UnkPath();
    // Position at parameter t (0 to 1).
    UnkVec3 fn_8010B200(f32 t);
    // Builds the segments from the points of a wire.
    void fn_8010B2D8(UnkWire* wire);

    void Clear() {
        mSegments = 0;
        mCount = 0;
        mLength = 0.0f;
    }

    /* 0x0 */ UnkPathSegment* mSegments;
    /* 0x4 */ u32 mCount;
    /* 0x8 */ f32 mLength;
};

class Entity_SpiritToken : public Entity_Prop {
public:
    Entity_SpiritToken(int type, EntityInfo* info, int init);
    virtual ~Entity_SpiritToken();

    void fn_80076B5C(f32 duration, UnkPath* path);
    u8 fn_80076B9C();
    void fn_80076BA4(f32 dt);

    /* 0x264 */ f32 mDuration;
    /* 0x268 */ f32 mTime;
    /* 0x26C */ u8 mMoving;
    /* 0x270 */ UnkPath* mPath;
};

struct UnkVec4 {
    UnkVec4(const UnkVec3& v, f32 w_) : x(v.x), y(v.y), z(v.z), w(w_) {}

    f32 x, y, z, w;
};

// Global object at 0x8047C760 (0x30 bytes) that hands out the handles kept in
// Entity::mA8 (real name unknown).
class UnkHazardManager {
public:
    void fn_801C80E0(void* arg, UnkVec4* pos, int* handle);
    void fn_801C83AC(int handle);
    void fn_801C849C(int handle);

    /* 0x00 */ u8 unk0[0x30];
};

extern UnkHazardManager lbl_8047C760;

// Its destructor is implicit (emitted at the end of its unit).
class Entity_Hazard_Prop : public Entity_Prop {
public:
    Entity_Hazard_Prop(int type, EntityInfo* info, int init);
    virtual int vf30(void* arg1, void* arg2);
    virtual void vf20(f32 dt, int arg);
    virtual void vf14(void* arg);
    virtual void vf18(void* arg);

    void fn_8006CD70(void* arg);
    void fn_8006CE08();

    void ReleaseHandle() {
        if (mA8 != 0) {
            lbl_8047C760.fn_801C849C(mA8);
            mA8 = 0;
        }
    }

    /* 0x264 */ f32 m264; // state timer
    /* 0x268 */ u32 m268; // 0 idle, 1 attack
    /* 0x26C */ u8 unk26C[0x274 - 0x26C];
    /* 0x274 */ f32 m274; // idle duration
    /* 0x278 */ f32 m278; // attack duration
};

// Its destructor is implicit (emitted at the end of its unit).
class Entity_Challenge_Projector : public Entity_Prop {
public:
    Entity_Challenge_Projector(int type, EntityInfo* info, int init);
    virtual int vf30(void* arg1, void* arg2);
    virtual void vf20(f32 dt, int arg);

    void fn_8006D248(void* target);

    /* 0x264 */ u8 mNear;
    /* 0x268 */ void* m268;
};

class Entity_MysticBrewVortex : public Entity_Prop {
public:
    Entity_MysticBrewVortex(int type, EntityInfo* info, int init);
    virtual ~Entity_MysticBrewVortex();

    void fn_800BF6FC();
    void fn_800BF700();
    void fn_800BF710(f32 delta);
    void* fn_800BF7F0();
    f32 fn_800BF870();
    f32 fn_800BF878(f32 delta);

    /* 0x264 */ f32 mLevel;
    /* 0x268 */ f32 mPrevLevel;
};

// Object given to Entity_BrainTossTarget::vfBC (real type unknown).
struct UnkBrainTossThrower {
    /* 0x00 */ u8 unk0[0xB4];
    /* 0xB4 */ f32 mRange;
};

class Entity_BrainTossTarget : public Entity_Prop {
public:
    Entity_BrainTossTarget(int type, EntityInfo* info, int init);
    virtual ~Entity_BrainTossTarget();
    virtual void vfBC(UnkBrainTossThrower* thrower);
    virtual void vfC0();
    virtual int vfC4() = 0;
    virtual int vfC8() = 0;

    int fn_80083798();
    void fn_800837A8();
    int fn_800837C4();
    void fn_800837F4(f32 time);
    int fn_800837FC();
    void fn_8008380C(int enable);
    int fn_80083820();
    void fn_80083828(int value);
    bool fn_80083830();
    int fn_80083930(UnkVec3* pos);

    /* 0x264 */ UnkBrainTossThrower* mThrower;
    /* 0x268 */ f32 mTime;
    /* 0x26C */ f32 mDuration;
    /* 0x270 */ bool m270;
    /* 0x274 */ UnkVec3 mTarget;
    /* 0x280 */ int m280;
};

// Entity carrying the hand (real class unknown; fn_800B7464 returns the float at
// 0x2F4).
class UnkArmWrestler : public Entity {
public:
    f32 fn_800B7464();
};

class Entity_ArmWrestleHand : public Entity_Prop {
public:
    Entity_ArmWrestleHand(int type, EntityInfo* info, int init);
    virtual ~Entity_ArmWrestleHand();
    virtual void vf20(f32 dt, int arg);

    void Detach() {
        if (mHolder != 0) {
            mHolder->fn_801B50B0(this);
            mInfo->mFlags |= 1;
            mHolder = 0;
        }
    }

    void fn_800B7B08(int arg, void* boneName);
    void fn_800B7B50();
    void fn_800B7C64(f32 value);
    int fn_800B7C74(UnkArmWrestler* holder, void* boneName);

    /* 0x264 */ f32 m264;
    /* 0x268 */ UnkArmWrestler* mHolder;
    /* 0x26C */ int mBone;
    /* 0x270 */ int m270;
    /* 0x274 */ UnkVec3 mOffset;
};

class Entity_BrainTossHand : public Entity_Prop {
public:
    Entity_BrainTossHand(int type, EntityInfo* info, int init);
    virtual ~Entity_BrainTossHand();

    int fn_8008682C();
    void fn_80086834(Entity* held);
    Entity* fn_80086844();
    UnkVec3 fn_8008684C();
    void fn_800869A0();

    // Two inline variants: fn_8008684C and fn_800869A0 allocate the joint lookup
    // registers differently, so they do not share one inline function.
    UnkVec3 GetHandPosition() {
        UnkModel* model = vf10();
        UnkVec3 pos = model->mData->mJoints[model->FindJoint(0x1DC9D499)].mPos;
        pos = fn_801B50FC()->Transform(pos);
        return pos;
    }

    UnkVec3 GetHandPos() {
        UnkModelData* data = vf10()->mData;
        UnkVec3 pos = data->mJoints[data->FindJoint(0x1DC9D499)].mPos;
        pos = fn_801B50FC()->Transform(pos);
        return pos;
    }

    /* 0x264 */ Entity* mHeld;
    /* 0x268 */ int m268;
};

// Abstract base of the reactor leak tools (name from RTTI; implicit constructor and
// destructor).
class Entity_ReactorLeakManipulator : public Entity_Prop {
public:
    Entity_ReactorLeakManipulator(int type, EntityInfo* info, int init) : Entity_Prop(type, info, init) {}
    virtual void vfBC() = 0;
    virtual void vfC0() = 0;
    virtual u8 vfC4() = 0;
};

class Entity_ReactorLeakHammer : public Entity_ReactorLeakManipulator {
public:
    Entity_ReactorLeakHammer(int type, EntityInfo* info, int init);
    virtual ~Entity_ReactorLeakHammer();
    virtual void vfBC();
    virtual void vfC0();
    virtual u8 vfC4();

    /* 0x264 */ u32 m264;
    /* 0x268 */ DirectionalSpikeRecognizer mRecognizer;
};

#endif // GAME_ENTITY_H
