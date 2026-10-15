#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

// Entity class family (names from RTTI): Entity -> Entity_Game -> Entity_Static ->
// Entity_Sensor -> Entity_Trigger_Script, and more. Entity's functions are at
// 0x801B4334-0x801B6E94 (unit not split yet); Entity_Game is in the game unit at
// 0x80062C80-0x8006307C. Real file names unknown; method names and most signatures
// unknown (vfXX = vtable offset, parameters follow the register use).
//
// Several Entity virtual functions are emitted in other units (0x8000C7A0,
// 0x800084A0, 0x80058B5C, 0x80015800, 0x80050588-0x800505A0, 0x8004B570-0x8004B57C):
// they are probably inline functions emitted where first used. They are declared
// out of line here until those units are decompiled.

#include "types.h"

class Entity;

// Per-entity state block pointed to by Entity::mInfo (real name unknown).
struct EntityInfo {
    /* 0x00 */ f32 m0[7];
    /* 0x1C */ u32 mTypeMask;
    /* 0x20 */ u32 mFlags;
    /* 0x24 */ u8 unk24[0x34 - 0x24];
    /* 0x34 */ u8 mType;
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

class Entity {
public:
    Entity(int type, EntityInfo* info, int init);
    virtual ~Entity();
    virtual void* vf0C();
    virtual void* vf10();
    virtual void vf14(void* arg);
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20(f32 dt, int arg);
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C();
    virtual void vf30(void* arg1, void* arg2);
    virtual void vf34();
    virtual void vf38();
    virtual void vf3C();
    virtual void vf40();
    virtual void* vf44();
    virtual void vf48();
    virtual int vf4C();
    virtual void vf50();
    virtual void vf54(void* arg);
    virtual void vf58();
    virtual int vf5C();
    virtual int vf60();
    virtual int vf64();
    virtual int vf68();
    virtual int vf6C();
    virtual int vf70();
    virtual void vf74();
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

    void SetType(int type) {
        if (mInfo != 0) {
            mInfo->mTypeMask |= 1 << type;
            mInfo->mType = type;
        }
    }

    /* 0x000 */ // vtable
    /* 0x004 */ EntityInfo* mInfo;
    /* 0x008 */ u8 unk8[0x88 - 0x8];
    /* 0x088 */ void* m88;
    /* 0x08C */ u8 unk8C[0x14C - 0x8C];
};

class Entity_Game : public Entity {
public:
    Entity_Game(int type, EntityInfo* info, int init);
    virtual ~Entity_Game();
    virtual void vf14(void* arg);
    virtual void vf18();
    virtual void vf20(f32 dt, int arg);
    virtual void vf30(void* arg1, void* arg2);
    virtual void vf40();
    virtual void vf78(void* arg);
    virtual UnkEntitySlot* vfB8();
    virtual void vfBC() = 0;

    // Whether these fields belong to Entity or Entity_Game is unknown.
    /* 0x14C */ u32 m14C;
    /* 0x150 */ u8 m150;
    /* 0x154 */ s32 m154;
};

// The destructor is implicit: the original emits it in another unit (0x800630E0).
class Entity_Static : public Entity_Game {
public:
    Entity_Static(int type, EntityInfo* info, int init);
    virtual void vf20(f32 dt, int arg);
    virtual void vf40();
};

#endif // GAME_ENTITY_H
