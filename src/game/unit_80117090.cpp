// Game unit at 0x80117090-0x80117AE4 (real file name unknown): the Creatures section of
// the InventoryBook (RTTI). Pages through the player's creatures, shows the current
// one on the stands model and gives its name and statistics. Bounds come from the
// static initializers of this unit and the previous one.

#include "game/unk_vec3.mch"

#include "game/InventoryBook.h"
#include "game/unk_803E6D00.h"
#include "game/unk_actor.h"
#include "game/unk_vec3.h"

// Stands model showing creatures (real type unknown; see unit_8013CA74.cpp).
struct UnkStands {
    /* 0x000 */ u8 unk0[0xC50];
    /* 0xC50 */ UnkActor* mModels[5];
    /* 0xC64 */ u8 unkC64[0xC6C - 0xC64];
    /* 0xC6C */ u32 mPending[5];
};

// Creature record (real type unknown).
struct UnkCreatureRecord {
    /* 0x0 */ u8 unk0[0x4];
    /* 0x4 */ void* mParts[5];
};

// List of the player's creatures (real type unknown).
struct UnkCreatureList {
    int IndexOf(UnkCreatureRecord* creature) {
        for (int i = 0; i < mCount; i++) {
            if (mItems[i] == creature) {
                return i;
            }
        }
        return -1;
    }

    /* 0x000 */ UnkCreatureRecord* mItems[0x1AE];
    /* 0x6B8 */ int mCount;
};

// Player data (real type unknown).
struct UnkPlayerData {
    /* 0x0000 */ u8 unk0[0xA30];
    /* 0x0A30 */ UnkCreatureList mCreatures;
};

extern "C" {
extern UnkPlayerData* lbl_805FBBE0;
extern void* lbl_805FB654;

// Pending creature requests and their count (shared with other units).
extern u32 lbl_803C4E28[];
extern int lbl_805FB034;
extern int lbl_805FB038;

void fn_8001CDE4(int index);
int fn_8001E430(void);
void fn_8003DCE0(UnkActor* model);
void fn_8004C718(UnkStands* stands, int index, void* model);
void fn_8004D2B8(UnkStands* stands, int arg);
void fn_8004D760(UnkStands* stands, void* creature);
s32 fn_8004DAF8(UnkStands* stands, int arg1, int arg2);
void fn_8004DCEC(UnkStands* stands, int index);
int fn_8004E300(void* obj, int arg);
void fn_8004EDD0(UnkActor* model, int arg1, int arg2);
void* fn_8004FFB4(u32 pending, void* part);
UnkStands* fn_800E876C(u32 id);
void fn_80130418(UnkGlobals803E6D00* globals);
void* fn_8018D1E4(void* obj);
UnkActor* fn_80198F18(void* screens, u32 hash);
void fn_801A3010(void* actor, void* arg);
void* fn_801B7198(void* obj, int arg);
void fn_8021C968(UnkCreatureRecord* creature, char* buf, int size);
f32 fn_8021CB98(UnkCreatureRecord* creature);
f32 fn_8021CCC0(UnkCreatureRecord* creature);
f32 fn_8021CDC0(UnkCreatureRecord* creature);
f32 fn_8021CEC0(UnkCreatureRecord* creature);
f32 fn_8021CF40(UnkCreatureRecord* creature);
f32 fn_8021D0B8(UnkCreatureRecord* creature);
UnkCreatureRecord* fn_8022E170(UnkPlayerData* player);
}

// Offset of the cursor on some targets (.rodata, unsplit; see unit_8013C600.cpp).
extern const UnkVec3 lbl_8034602C;

inline UnkCreatureRecord* UnkCurrent(Creatures* self) {
    return (UnkCreatureRecord*)self->mCreature;
}

inline void UnkReleaseModel(UnkStands* stands, int index) {
    UnkActor* model = stands->mModels[index];
    fn_8004C718(stands, index, 0);
    fn_8003DCE0(model);
}

Creatures::Creatures() {
    mCreature = 0;
}

Creatures::~Creatures() {}

void Creatures::vf0C() {
    mActor = 0;
    mStands = 0;
    mShown = 0;
    mOpened = 0;
    mTimer = 0;
    mCreature = fn_8022E170(lbl_805FBBE0);
    int index = lbl_805FBBE0->mCreatures.IndexOf((UnkCreatureRecord*)mCreature);
    mIndex = index;
    mShownIndex = index;
    mCount = lbl_805FBBE0->mCreatures.mCount;
    m8 = mCount;
    m4 = index + 1;
}

void Creatures::vf10() {
    if (mStands != 0) {
        for (int i = 0; i < 5; i++) {
            UnkReleaseModel(mStands, i);
        }
    }
    mCreature = 0;
    mStands = 0;
}

void Creatures::vf14() {}

void Creatures::vf18() {}

inline void UnkSelect(Creatures* self) {
    self->mCreature = lbl_805FBBE0->mCreatures.mItems[self->mIndex];
    self->m4 = self->mIndex + 1;
    if (fn_8001E430() > 0) {
        fn_8001CDE4(self->mIndex);
    }
    self->mShown = 0;
    self->mOpened = 0;
}

void Creatures::fn_8011726C(int delta) {
    int old = mIndex;
    mIndex = old + delta;
    if (mIndex >= mCount) {
        mIndex %= mCount;
    }
    while (mIndex < 0) {
        mIndex += mCount;
    }
    if (mIndex != old) {
        UnkSelect(this);
    }
}

void Creatures::fn_80117320(int index) {
    mIndex = index;
    if (index >= mCount) {
        mIndex = 0;
    }
    UnkSelect(this);
}

f32 Creatures::fn_801173A0() {
    if (mCreature != 0) {
        return fn_8021CCC0(UnkCurrent(this));
    }
    return 0.0f;
}

f32 Creatures::fn_801173B8() {
    if (mCreature != 0) {
        return fn_8021CDC0(UnkCurrent(this));
    }
    return 0.0f;
}

f32 Creatures::fn_801173D0() {
    if (mCreature != 0) {
        return fn_8021CEC0(UnkCurrent(this));
    }
    return 0.0f;
}

f32 Creatures::fn_801173E8() {
    if (mCreature != 0) {
        return fn_8021CB98(UnkCurrent(this));
    }
    return 0.0f;
}

f32 Creatures::fn_80117400() {
    if (mCreature != 0) {
        return fn_8021CF40(UnkCurrent(this));
    }
    return 0.0f;
}

f32 Creatures::fn_80117418() {
    if (mCreature != 0) {
        return fn_8021D0B8(UnkCurrent(this));
    }
    return 0.0f;
}

void Creatures::fn_80117430() {
    if (mShown == 0 && mOpened == 0) {
        int ready = 1;
        for (int i = 0; i < lbl_805FB034; i++) {
            if (lbl_803C4E28[i] == 0) {
                ready = 0;
            }
        }
        if (ready) {
            for (int i = 0; i < 5; i++) {
                u32 pending = mStands->mPending[i];
                if (pending != 0 && pending < 1000) {
                    ready = 0;
                }
            }
        }
        if (ready) {
            if (mActor != 0) {
                fn_801A3010(mActor, 0);
                mActor->vf30(0x1A);
            }
            for (int i = 0; i < 5; i++) {
                UnkReleaseModel(mStands, i);
            }
            mCreature = lbl_805FBBE0->mCreatures.mItems[mIndex];
            if (mCreature != 0) {
                fn_8004D760(mStands, mCreature);
                fn_8004DAF8(mStands, 1, 0);
                lbl_805FB038 = 5;
            }
            mShownIndex = mIndex;
        }
    }
    if (mActor == 0) {
        return;
    }
    fn_80130418(&lbl_803E6D00);
    for (int i = 0; i < lbl_805FB034; i++) {
        if (lbl_803C4E28[i] == 0) {
            return;
        }
    }
    for (int i = 0; i < 5; i++) {
        fn_8004DCEC(mStands, i);
        if (mStands->mPending[i] != 0) {
            void* model = fn_8004FFB4(mStands->mPending[i], UnkCurrent(this)->mParts[i]);
            if (model != 0) {
                fn_8004C718(mStands, i, 0);
                fn_8004C718(mStands, i, model);
                if (mStands->mModels[i] != 0) {
                    fn_8004EDD0(mStands->mModels[i], 0, 0);
                    *(u32*)((u8*)mStands->mModels[i] + 0x15C) |= 0x400000;
                }
            }
            mStands->mPending[i] = 0;
        }
    }
    if (mShown == 0) {
        if (mActor != 0) {
            fn_801A3010(mActor, mStands);
            mActor->vf2C(0x18);
            mTimer = 3;
        }
        fn_8004D2B8(mStands, 1);
    }
    if (mTimer > 0 && --mTimer == 0) {
        mActor->vf2C(2);
    }
    for (int i = 0; i < lbl_805FB034; i++) {
        lbl_803C4E28[i] = 0;
    }
    lbl_805FB034 = 0;
    if (mIndex == mShownIndex) {
        mShown = 1;
        lbl_805FB038 = 0;
    }
}

void Creatures::fn_80117844() {
    mActor = fn_80198F18(fn_8018D1E4(lbl_805FB654), 0x60B15D91);
    lbl_803C4E28[0] = 0;
    lbl_803C4E28[1] = 0;
    lbl_803C4E28[2] = 0;
    lbl_803C4E28[3] = 0;
    lbl_803C4E28[4] = 0;
    lbl_805FB038 = 5;
    lbl_805FB034 = 0;
    if (mStands == 0) {
        mStands = fn_800E876C(0xA9638202);
        if (mStands != 0) {
            if (mCreature != 0) {
                fn_8004D760(mStands, mCreature);
                fn_8004DAF8(mStands, 1, 0);
                lbl_805FB038 = 5;
            }
            if (mActor != 0) {
                fn_801A3010(mActor, 0);
                mActor->vf30(0x1A);
            }
        }
        mOpened = 1;
    }
    if (fn_8001E430() > 0) {
        fn_8001CDE4(mIndex);
    }
}

int Creatures::fn_80117948() {
    if (lbl_805FBBE0 != 0 && lbl_805FBBE0->mCreatures.mCount > 0) {
        return 1;
    }
    return 0;
}

const char* Creatures::fn_80117970() {
    if (mCreature != 0) {
        fn_8021C968(UnkCurrent(this), mName, 0x10);
        return mName;
    }
    return 0;
}

int Creatures::fn_801179C0() {
    return mShown;
}

void Creatures::fn_801179C8() {
    if (mActor != 0) {
        UnkVec3 pos = *(UnkVec3*)((u8*)mActor + 0x14);
        void* target = fn_801B7198((u8*)mActor + 0x140, 0);
        if (target != 0 && fn_8004E300(target, 1) == 13) {
            pos += UnkVec3(lbl_8034602C.x, lbl_8034602C.y, lbl_8034602C.z);
        }
        mActor->mPos = UnkVec3(pos);
        mActor->vf2C(0x18);
    }
}

int Creatures::fn_80117ABC() {
    return mIndex;
}
