// Game unit at 0x8009210C-0x800938A4 (real file name unknown): the robots of the robot
// evolution experiment (named callbacks "RobotEvolutionExperiment_RobotReadyToLaunch",
// "..._RobotLanded", "..._RobotShowAnswer", "..._RobotIsLeft", "..._RobotTaunts",
// "..._RobotGameOver", "..._RobotIsCorrectRobot"...). Up to three robots are
// registered in a static table. Bounds come from the static initializers of this unit
// and the previous one.

#include "game/unk_vec3.mch"

#include "game/Entity.h"
#include "game/externs.h"
#include "game/unk_vec3.h"

class UnkRobot;

// Registered robots, by slot.
static UnkRobot* lbl_803C6A70[3];

// Reference counted object held by a robot (real type unknown).
struct UnkRobotRef {
    /* 0x00 */ u8 unk0[0x24];
    /* 0x24 */ int mRefs;
};

struct UnkRobotAnimOwner {
    /* 0x00 */ u8 unk0[0x48];
    /* 0x48 */ void* mSet;
};

// Object placed in the level, with its position at 0x8.
struct UnkLocator {
    /* 0x00 */ u8 unk0[0x8];
    /* 0x08 */ UnkVec3 mPos;
};

struct UnkRobotContext {
    /* 0x00 */ u8 unk0[0x10];
    /* 0x10 */ Entity* mOwner;
};

// Global manager created on first use (see unit_8010FA48.cpp).
class UnkManager {
public:
    UnkManager();
    static int fn_80209E70(u32 hash, int arg1, int arg2, int arg3, int arg4, int arg5);

    static UnkManager* Instance() {
        if (sInstance == 0) {
            sInstance = new UnkManager;
        }
        return sInstance;
    }

    static UnkManager* sInstance;
};

extern UnkHazardManager lbl_8047C760;

extern "C" {
void fn_801DC91C(const char* name, int type, void* func);
void fn_801903CC(void* obj, u32 hash);
void fn_80184028(UnkRobotRef* ref);
void fn_8020A398(int handle);
void fn_801ECFA0(UnkEntity94* model, int bone);
void fn_801EC8C4(UnkEntity94* model, int bone, u32 hash);
void fn_80191B68(void* obj, UnkRobotRef* ref);
void fn_8018FE5C(void* obj, int arg);
u32 fn_80143460();
int fn_801AC2FC(void* set, u32 id);
void fn_80069350(Entity* entity, int index, int arg2, int arg3);
}

class UnkRobot {
public:
    UnkRobot();
    ~UnkRobot();
    void fn_800923DC();

    void ReleaseRef() {
        UnkRobotRef* ref = mRef;
        int refs = --ref->mRefs;
        if (refs <= 0) {
            if (refs == 0) {
                fn_80184028(ref);
            }
        }
        mRef = 0;
    }
    void ClearRef(u32 hash) {
        if (mRef != 0) {
            fn_801903CC(mEffect, hash);
            ReleaseRef();
        }
    }
    void StopSound() {
        if (mSound != -1) {
            fn_8020A398(mSound);
            mSound = -1;
        }
    }
    void StopSound2() {
        if (mSound2 != -1) {
            fn_8020A398(mSound2);
            mSound2 = -1;
        }
    }
    void ResetBones() {
        if (mEntity != 0) {
            UnkEntity94* model = &mEntity->m94;
            fn_801ECFA0(model, model->fn_801EC880((void*)0x53B643B8));
            fn_801ECFA0(model, model->fn_801EC880((void*)0x37B4DA5B));
            fn_801ECFA0(model, model->fn_801EC880((void*)0x8B9BD519));
        }
    }
    bool IsDone() const { return mState == 4 || mState == 5; }

    /* 0x00 */ int mSlot;
    /* 0x04 */ Entity* mEntity;
    /* 0x08 */ int mState;
    /* 0x0C */ UnkRobotRef* mRef;
    /* 0x10 */ void* mEffect;
    /* 0x14 */ u8 mCorrect;
    /* 0x18 */ UnkLocator* m18;
    /* 0x1C */ UnkLocator* m1C;
    /* 0x20 */ UnkLocator* m20;
    /* 0x24 */ int mSound;
    /* 0x28 */ int mSound2;
    /* 0x2C */ int m2C;
    /* 0x30 */ u32 m30;
    /* 0x34 */ u8 mTimerOn;
    /* 0x38 */ f32 mTimer;
    /* 0x3C */ u8 mTaunts;
    /* 0x40 */ int mFlee;
    /* 0x44 */ int mRandom;
};

// Last random answer (-1 when none).
int lbl_805F8A38 = -1;

UnkRobot::UnkRobot() {
    mSlot = -1;
    mEntity = 0;
    mState = -1;
    mRef = 0;
    mEffect = 0;
    mCorrect = 0;
    m18 = 0;
    m1C = 0;
    m20 = 0;
    mSound = -1;
    mSound2 = -1;
    m2C = 0;
    m30 = 0;
    mTimerOn = 0;
    mTimer = 0.0f;
    mRandom = -1;
}

UnkRobot::~UnkRobot() {
    fn_800923DC();
}

extern "C" {

void fn_80092E40(const UnkRobotContext* ctx);
void fn_80092EA0(const UnkRobotContext* ctx);
void fn_80092F50(const UnkRobotContext* ctx);
void fn_80092FD4(const UnkRobotContext* ctx);
void fn_80093168(const UnkRobotContext* ctx);
int fn_80093278(const UnkRobotContext* ctx);
int fn_800932E0(const UnkRobotContext* ctx);
int fn_80093350(const UnkRobotContext* ctx);
int fn_800933C0(const UnkRobotContext* ctx);
int fn_80093430(const UnkRobotContext* ctx);
int fn_800934A0(const UnkRobotContext* ctx);
int fn_80093510(const UnkRobotContext* ctx);
int fn_80093580(const UnkRobotContext* ctx);
int fn_80093608(const UnkRobotContext* ctx);
int fn_80093678(const UnkRobotContext* ctx);
int fn_800936E8(const UnkRobotContext* ctx);

void fn_800921B0(void) {
    fn_801DC91C("RobotEvolutionExperiment_RobotReadyToLaunch", 78, (void*)fn_80092E40);
    fn_801DC91C("RobotEvolutionExperiment_RobotLanded", 78, (void*)fn_80092EA0);
    fn_801DC91C("RobotEvolutionExperiment_RobotReadyToPlay", 78, (void*)fn_80092F50);
    fn_801DC91C("RobotEvolutionExperiment_RobotShowAnswer", 78, (void*)fn_80092FD4);
    fn_801DC91C("RobotEvolutionExperiment_RobotStartFleeCycle", 78, (void*)fn_80093168);
    fn_801DC91C("RobotEvolutionExperiment_RobotGetLastRandom", 29, (void*)fn_80093278);
    fn_801DC91C("RobotEvolutionExperiment_RobotIsLeft", 29, (void*)fn_800932E0);
    fn_801DC91C("RobotEvolutionExperiment_RobotIsCenter", 29, (void*)fn_80093350);
    fn_801DC91C("RobotEvolutionExperiment_RobotIsRight", 29, (void*)fn_800933C0);
    fn_801DC91C("RobotEvolutionExperiment_RobotTaunts", 29, (void*)fn_80093430);
    fn_801DC91C("RobotEvolutionExperiment_RobotFleesLeft", 29, (void*)fn_800934A0);
    fn_801DC91C("RobotEvolutionExperiment_RobotFleesRight", 29, (void*)fn_80093510);
    fn_801DC91C("RobotEvolutionExperiment_RobotGameOver", 29, (void*)fn_80093580);
    fn_801DC91C("RobotEvolutionExperiment_RobotGameWonCheer", 29, (void*)fn_80093608);
    fn_801DC91C("RobotEvolutionExperiment_RobotGameLostFlee", 29, (void*)fn_80093678);
    fn_801DC91C("RobotEvolutionExperiment_RobotIsCorrectRobot", 29, (void*)fn_800936E8);
}

void fn_8009231C(UnkRobot* robot, int slot, Entity* entity, void* effect, UnkLocator* locator18,
                 UnkLocator* locator1C, UnkLocator* locator20) {
    lbl_803C6A70[slot] = robot;
    robot->mSlot = slot;
    robot->mEntity = entity;
    robot->mState = 0;
    robot->ClearRef(0xB729E3E6);
    robot->mEffect = effect;
    robot->m18 = locator18;
    robot->m1C = locator1C;
    robot->m20 = locator20;
    robot->mTimerOn = 0;
    robot->mTimer = 0.0f;
}

}

void UnkRobot::fn_800923DC() {
    ResetBones();
    lbl_803C6A70[mSlot] = 0;
    mSlot = -1;
    mEntity = 0;
    mState = -1;
    ClearRef(0x3D242667);
    mEffect = 0;
    m18 = 0;
    m1C = 0;
    m20 = 0;
    StopSound();
    StopSound2();
    m2C = 0;
    m30 = 0;
    mTimerOn = 0;
    mTimer = 0.0f;
    mRandom = -1;
}

extern "C" {

void fn_8009253C(UnkRobot* robot, f32 dt) {
    if (robot->mTimerOn) {
        robot->mTimer -= dt;
        if (robot->mState == 0 && robot->mTimer <= 0.0f) {
            robot->mTimerOn = 0;
            fn_801BA190(lbl_805FB6C8, robot->mEntity, 0, 0xDC155FBA, 0, 0, 1, 0);
        }
    }
}

void fn_800925A4(UnkRobot* robot) {
    robot->mRandom = -1;
}

void fn_800925B0(UnkRobot* robot, f32 time) {
    robot->mTimer = time;
    robot->mTimerOn = 1;
}

void fn_800925C0(UnkRobot* robot, UnkRobotRef* ref, int arg, u8 correct) {
    if (robot->mState != 2) {
        if (robot->mRef != 0) {
            robot->ReleaseRef();
        }
        robot->mRef = ref;
        ref->mRefs++;
        robot->mCorrect = correct;
        robot->mTimerOn = 1;
        fn_80191B68(robot->mEffect, robot->mRef);
        fn_8018FE5C(robot->mEffect, arg);
    }
}

void fn_80092678(UnkRobot* robot) {
    robot->mState = 2;
    fn_801BA190(lbl_805FB6C8, robot->mEntity, 0, 0xD7A63AA2, 0, 0, 1, 0);
    robot->ClearRef(0x3D242667);
    robot->StopSound();
    robot->StopSound2();
    robot->m2C = 0;
    robot->m30 = 0;
    robot->ResetBones();
}

void fn_800927B0(UnkRobot* robot) {
    fn_801903CC(robot->mEffect, 0x15726E29);
    UnkManager::Instance()->fn_80209E70(0xF6127B57, 0, 0, 0, 0, 0);
    robot->mSound = UnkManager::Instance()->fn_80209E70(0x0007E9A2, 0, 0, 0, 0, 0);
    if (robot->mEntity != 0) {
        UnkEntity94* model = &robot->mEntity->m94;
        fn_801EC8C4(model, model->fn_801EC880((void*)0x53B643B8), 0x1833D84C);
        fn_801EC8C4(model, model->fn_801EC880((void*)0x37B4DA5B), 0x1833D84C);
        fn_801EC8C4(model, model->fn_801EC880((void*)0x8B9BD519), 0xA80D43C2);
    }
}

void fn_800928F4(UnkRobot* robot) {
    robot->ClearRef(0xB729E3E6);
    robot->StopSound();
    robot->ResetBones();
}

void fn_800929D8(UnkRobot* robot) {
    if (robot->mRef != 0) {
        robot->ReleaseRef();
    }
}

void fn_80092A30(UnkRobot* robot, int won) {
    robot->ClearRef(0xB729E3E6);
    robot->StopSound();
    robot->StopSound2();
    robot->m2C = 0;
    robot->m30 = 0;
    robot->ResetBones();
    if (won) {
        fn_801BA190(lbl_805FB6C8, robot->mEntity, 0, 0xB91BDDB3, 0, 0, 1, 0);
        if (robot->mState != 2) {
            UnkVec4 pos(robot->m1C->mPos, 0.0f);
            lbl_8047C760.fn_801C80E0((void*)0xFE6B32D9, &pos, 0);
            UnkVec4 pos2(robot->m20->mPos, 0.0f);
            lbl_8047C760.fn_801C80E0((void*)0xFC2DB371, &pos2, 0);
        }
        robot->mState = 3;
    } else {
        fn_801BA190(lbl_805FB6C8, robot->mEntity, 0, 0xF78001C0, 0, 0, 1, 0);
        robot->mState = 3;
    }
}

void fn_80092C40(UnkRobot* robot, int won) {
    if (robot->mState == 1) {
        fn_801BA190(lbl_805FB6C8, robot->mEntity, 0, 0xE495E9F9, 0, 0, 1, 0);
    }
    if (won) {
        robot->mState = 4;
    } else {
        robot->mState = 5;
    }
}

UnkRobot* fn_80092CC4(Entity* entity) {
    for (int i = 0; i < 3; i++) {
        if (lbl_803C6A70[i]->mEntity == entity) {
            return lbl_803C6A70[i];
        }
    }
    return 0;
}

void fn_80092D10() {
    lbl_803C6A70[0]->mFlee = 0;
    if (lbl_803C6A70[0]->mState == 2) {
        lbl_803C6A70[1]->mFlee = 2;
    } else {
        lbl_803C6A70[1]->mFlee = 0;
    }
    lbl_803C6A70[2]->mFlee = 2;
    int count = 0;
    for (int i = 0; i < 3; i++) {
        if (lbl_803C6A70[i]->mState != 2) {
            count++;
        }
    }
    lbl_803C6A70[0]->mTaunts = 0;
    lbl_803C6A70[1]->mTaunts = 0;
    lbl_803C6A70[2]->mTaunts = 0;
    if (count != 1) {
        if (lbl_803C6A70[1]->mState != 2) {
            lbl_803C6A70[1]->mTaunts = 1;
        } else if (fn_80143460() & 0x10000) {
            lbl_803C6A70[0]->mTaunts = 1;
        } else {
            lbl_803C6A70[2]->mTaunts = 1;
        }
    }
}

void fn_80092E40(const UnkRobotContext* ctx) {
    UnkRobot* robot = fn_80092CC4(ctx->mOwner);
    if (robot != 0) {
        robot->mState = 0;
    }
}

void fn_80092EA0(const UnkRobotContext* ctx) {
    UnkRobot* robot = fn_80092CC4(ctx->mOwner);
    if (robot != 0) {
        UnkVec4 pos(robot->m18->mPos, 0.0f);
        lbl_8047C760.fn_801C80E0((void*)0xF5BB8364, &pos, 0);
    }
}

void fn_80092F50(const UnkRobotContext* ctx) {
    UnkRobot* robot = fn_80092CC4(ctx->mOwner);
    if (robot != 0 && !robot->IsDone()) {
        robot->mState = 1;
    }
}

void fn_80092FD4(const UnkRobotContext* ctx) {
    UnkRobot* robot = fn_80092CC4(ctx->mOwner);
    if (robot != 0) {
        fn_800927B0(robot);
    }
}

void fn_80093168(const UnkRobotContext* ctx) {
    UnkRobot* robot = fn_80092CC4(ctx->mOwner);
    if (robot != 0) {
        robot->mState = 3;
    }
}

void fn_800931C8(int max) {
    if (max >= 0) {
        u32 retry = 1;
        while (retry == 1) {
            int value = fn_80143460() % (max + 1);
            lbl_805F8A38 = value;
            retry = 0;
            for (int i = 0; i < 3; i++) {
                if (value == lbl_803C6A70[i]->mRandom) {
                    retry = 1;
                }
            }
        }
    } else {
        lbl_805F8A38 = -1;
    }
}

int fn_80093278(const UnkRobotContext* ctx) {
    UnkRobot* robot = fn_80092CC4(ctx->mOwner);
    if (robot != 0) {
        return robot->mRandom = lbl_805F8A38;
    }
    return -1;
}

int fn_800932E0(const UnkRobotContext* ctx) {
    UnkRobot* robot = fn_80092CC4(ctx->mOwner);
    if (robot != 0 && robot->mSlot == 0) {
        return 1;
    }
    return 0;
}

int fn_80093350(const UnkRobotContext* ctx) {
    UnkRobot* robot = fn_80092CC4(ctx->mOwner);
    if (robot != 0 && robot->mSlot == 1) {
        return 1;
    }
    return 0;
}

int fn_800933C0(const UnkRobotContext* ctx) {
    UnkRobot* robot = fn_80092CC4(ctx->mOwner);
    if (robot != 0 && robot->mSlot == 2) {
        return 1;
    }
    return 0;
}

int fn_80093430(const UnkRobotContext* ctx) {
    UnkRobot* robot = fn_80092CC4(ctx->mOwner);
    if (robot != 0 && robot->mTaunts) {
        return 1;
    }
    return 0;
}

int fn_800934A0(const UnkRobotContext* ctx) {
    UnkRobot* robot = fn_80092CC4(ctx->mOwner);
    if (robot != 0 && robot->mFlee == 0) {
        return 1;
    }
    return 0;
}

int fn_80093510(const UnkRobotContext* ctx) {
    UnkRobot* robot = fn_80092CC4(ctx->mOwner);
    if (robot != 0 && robot->mFlee == 2) {
        return 1;
    }
    return 0;
}

int fn_80093580(const UnkRobotContext* ctx) {
    UnkRobot* robot = fn_80092CC4(ctx->mOwner);
    if (robot != 0 && robot->IsDone()) {
        return 1;
    }
    return 0;
}

int fn_80093608(const UnkRobotContext* ctx) {
    UnkRobot* robot = fn_80092CC4(ctx->mOwner);
    if (robot != 0 && robot->mState == 4) {
        return 1;
    }
    return 0;
}

int fn_80093678(const UnkRobotContext* ctx) {
    UnkRobot* robot = fn_80092CC4(ctx->mOwner);
    if (robot != 0 && robot->mState == 5) {
        return 1;
    }
    return 0;
}

int fn_800936E8(const UnkRobotContext* ctx) {
    UnkRobot* robot = fn_80092CC4(ctx->mOwner);
    if (robot != 0 && robot->mCorrect) {
        return 1;
    }
    return 0;
}

void fn_80093758(UnkRobotContext* ctx, u32 hash, int arg) {
    UnkRobot* robot = fn_80092CC4(ctx->mOwner);
    if (robot != 0) {
        robot->mSound2 = -1;
        robot->m2C = arg;
        robot->m30 = hash;
    }
}

void fn_800937C0(UnkRobotContext* ctx) {
    UnkRobot* robot = fn_80092CC4(ctx->mOwner);
    if (robot != 0 && robot->m30 != 0 && robot->m30 != 0x9F93FF74) {
        UnkRobotAnimOwner* owner = (UnkRobotAnimOwner*)robot->mEntity->m9C;
        if (owner != 0 && owner->mSet != 0) {
            fn_80069350(robot->mEntity, fn_801AC2FC(owner->mSet, robot->m30), 0, 1);
        }
    }
}

}
