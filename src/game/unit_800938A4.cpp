// Game unit at 0x800938A4-0x800940BC (real file name unknown): the smasher of the
// robot evolution experiment (named callbacks "RobotEvolutionExperiment_SmasherIdling",
// "..._SmasherDropping", "..._SmasherRaising", "..._SmasherHit", "..._SmasherMiss",
// "..._SmasherCheckImpactFrame" and "..._SmasherHitSomething"). Up to three smashers
// are registered in a static table. Bounds come from the static initializers of
// this unit and the previous one.

#include "game/Entity.h"
#include "game/externs.h"

class UnkSmasher;

// Registered smashers, by slot.
static UnkSmasher* lbl_803C6A88[3];

#include "game/unk_vec3.h"

// Real names unknown.
struct UnkSmasherAnimState;

struct UnkSmasherAnimOwner {
    /* 0x00 */ u8 unk0[0x48];
    /* 0x48 */ void* mSet;
};

struct UnkSmasherOwner {
    /* 0x00 */ u8 unk0[0x9C];
    /* 0x9C */ UnkSmasherAnimOwner* m9C;
    /* 0xA0 */ UnkSmasherAnimState* mA0;
};

struct UnkSmasherAnimEntry {
    /* 0x00 */ u8 unk0[0x28];
    /* 0x28 */ int mFrame;
};

// Object placed in the level, with its position at 0x8.
struct UnkLocator {
    /* 0x00 */ u8 unk0[0x8];
    /* 0x08 */ UnkVec3 mPos;
};

struct UnkSmasherContext {
    /* 0x00 */ u8 unk0[0x10];
    /* 0x10 */ UnkSmasherOwner* mOwner;
};


extern "C" {
void fn_801DC91C(const char* name, int type, void* func);
int fn_801AC2FC(void* set, u32 id);
UnkSmasherAnimEntry* fn_801AC164(void* set, int index);
f32 fn_8018C014(UnkSmasherAnimState* state, f32 rate);
}

class UnkSmasher {
public:
    UnkSmasher();
    ~UnkSmasher();
    void fn_80093A88();

    /* 0x00 */ int mSlot;
    /* 0x04 */ UnkSmasherOwner* mOwner;
    /* 0x08 */ int mState;
    /* 0x0C */ UnkLocator* mHitLocator;
    /* 0x10 */ UnkLocator* mHazardLocator;
    /* 0x14 */ int mHazard;
    /* 0x18 */ int mHazard2;
    /* 0x1C */ u8 mImpact;
    /* 0x1D */ bool mHitSomething;
    /* 0x1E */ u8 mArmed;
    /* 0x20 */ int mImpactFrame;
    /* 0x24 */ u8 mHazardOn;
};

static inline UnkSmasher* FindSmasher(UnkSmasherOwner* owner) {
    for (int i = 0; i < 3; i++) {
        if (lbl_803C6A88[i]->mOwner == owner) {
            return lbl_803C6A88[i];
        }
    }
    return 0;
}

UnkSmasher::UnkSmasher() {
    mSlot = -1;
    mOwner = 0;
    mState = -1;
    mHitLocator = 0;
    mHazardLocator = 0;
    mHazard = 0;
    mHazard2 = 0;
    mImpact = 0;
    mHitSomething = 0;
    mArmed = 1;
    mHazardOn = 0;
}

UnkSmasher::~UnkSmasher() {
    fn_80093A88();
}

extern "C" {

void fn_80093D3C(UnkSmasherContext* ctx);
void fn_80093D9C(UnkSmasherContext* ctx);
void fn_80093DFC(UnkSmasherContext* ctx);
void fn_80093E5C(UnkSmasherContext* ctx);
void fn_80093F0C(UnkSmasherContext* ctx);
int fn_80093FBC(UnkSmasherContext* ctx);
int fn_8009402C(UnkSmasherContext* ctx);

void fn_80093934(void) {
    fn_801DC91C("RobotEvolutionExperiment_SmasherIdling", 78, (void*)fn_80093D3C);
    fn_801DC91C("RobotEvolutionExperiment_SmasherDropping", 78, (void*)fn_80093D9C);
    fn_801DC91C("RobotEvolutionExperiment_SmasherRaising", 78, (void*)fn_80093DFC);
    fn_801DC91C("RobotEvolutionExperiment_SmasherHit", 78, (void*)fn_80093E5C);
    fn_801DC91C("RobotEvolutionExperiment_SmasherMiss", 78, (void*)fn_80093F0C);
    fn_801DC91C("RobotEvolutionExperiment_SmasherCheckImpactFrame", 29, (void*)fn_80093FBC);
    fn_801DC91C("RobotEvolutionExperiment_SmasherHitSomething", 29, (void*)fn_8009402C);
}

void fn_800939EC(UnkSmasher* smasher, int slot, UnkSmasherOwner* owner, UnkLocator* hitLocator,
                 UnkLocator* hazardLocator) {
    lbl_803C6A88[slot] = smasher;
    smasher->mSlot = slot;
    smasher->mOwner = owner;
    smasher->mState = 0;
    smasher->mHitLocator = hitLocator;
    smasher->mHazardLocator = hazardLocator;
    smasher->mHazardOn = 0;
    UnkSmasherAnimOwner* anim = owner->m9C;
    if (anim != 0 && anim->mSet != 0) {
        int index = fn_801AC2FC(anim->mSet, 0x51E28E95);
        if (index >= 0) {
            smasher->mImpactFrame = fn_801AC164(anim->mSet, index)->mFrame;
        }
    }
}

}

void UnkSmasher::fn_80093A88() {
    lbl_803C6A88[mSlot] = 0;
    mSlot = -1;
    mOwner = 0;
    mState = -1;
    mHitLocator = 0;
    mHazardLocator = 0;
    if (mHazard != 0) {
        lbl_8047C760.fn_801C83AC(mHazard);
        mHazard = 0;
    }
    if (mHazard2 != 0) {
        lbl_8047C760.fn_801C83AC(mHazard2);
        mHazard2 = 0;
    }
    mHazardOn = 0;
    mImpact = 0;
    mHitSomething = 0;
    mArmed = 1;
}

extern "C" {

void fn_80093B3C(UnkSmasher* smasher, int hitNothing) {
    smasher->mImpact = 0;
    smasher->mHitSomething = 0;
    if (smasher->mState == 1 && smasher->mArmed == 1) {
        if (fn_8018C014(smasher->mOwner->mA0, 30.0f) >= smasher->mImpactFrame) {
            smasher->mImpact = 1;
            smasher->mHitSomething = hitNothing == 0 ? 1 : 0;
            smasher->mArmed = 0;
        }
    }
}

void fn_80093BF0(UnkSmasher* smasher) {
    smasher->mArmed = 1;
    fn_801BA190(lbl_805FB6C8, smasher->mOwner, 0, 0xD7A63AA2, 0, 0, 1, 0);
}

void fn_80093C20(UnkSmasher* smasher, u8 on) {
    if (smasher->mHazardOn != on) {
        if (on) {
            UnkVec4 pos(smasher->mHazardLocator->mPos, 0.0f);
            lbl_8047C760.fn_801C80E0((void*)0x040EE80C, &pos, &smasher->mHazard);
        } else {
            if (smasher->mHazard != 0) {
                lbl_8047C760.fn_801C83AC(smasher->mHazard);
                smasher->mHazard = 0;
            }
            if (smasher->mHazard2 != 0) {
                lbl_8047C760.fn_801C83AC(smasher->mHazard2);
                smasher->mHazard2 = 0;
            }
        }
        smasher->mHazardOn = on;
    }
}

UnkSmasher* fn_80093CF0(UnkSmasherOwner* owner) {
    for (int i = 0; i < 3; i++) {
        if (lbl_803C6A88[i]->mOwner == owner) {
            return lbl_803C6A88[i];
        }
    }
    return 0;
}

void fn_80093D3C(UnkSmasherContext* ctx) {
    UnkSmasher* smasher = FindSmasher(ctx->mOwner);
    if (smasher != 0) {
        smasher->mState = 0;
    }
}

void fn_80093D9C(UnkSmasherContext* ctx) {
    UnkSmasher* smasher = FindSmasher(ctx->mOwner);
    if (smasher != 0) {
        smasher->mState = 1;
    }
}

void fn_80093DFC(UnkSmasherContext* ctx) {
    UnkSmasher* smasher = FindSmasher(ctx->mOwner);
    if (smasher != 0) {
        smasher->mState = 2;
    }
}

// NonMatching (97.3 %, like fn_80093F0C): the DOL schedules the ctx->mOwner load
// before `i = 0`; same instructions otherwise.
void fn_80093E5C(UnkSmasherContext* ctx) {
    UnkSmasher* smasher = FindSmasher(ctx->mOwner);
    if (smasher != 0) {
        UnkLocator* locator = smasher->mHitLocator;
        UnkVec4 pos(locator->mPos, 0.0f);
        lbl_8047C760.fn_801C80E0((void*)0xF7A1EDD6, &pos, 0);
    }
}

void fn_80093F0C(UnkSmasherContext* ctx) {
    UnkSmasher* smasher = FindSmasher(ctx->mOwner);
    if (smasher != 0) {
        UnkVec4 pos(smasher->mHitLocator->mPos, 0.0f);
        lbl_8047C760.fn_801C80E0((void*)0x1166D3AF, &pos, 0);
    }
}

int fn_80093FBC(UnkSmasherContext* ctx) {
    UnkSmasher* smasher = FindSmasher(ctx->mOwner);
    if (smasher != 0 && smasher->mImpact) {
        return 1;
    }
    return 0;
}

int fn_8009402C(UnkSmasherContext* ctx) {
    UnkSmasher* smasher = FindSmasher(ctx->mOwner);
    if (smasher != 0 && smasher->mHitSomething) {
        return 1;
    }
    return 0;
}

}
