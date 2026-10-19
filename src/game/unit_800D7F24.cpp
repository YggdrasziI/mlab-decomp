// Game unit at 0x800D7F24-0x800D887C (real file name unknown): the object at
// lbl_803B53A0.mD5F0, which keeps one entity per mode and drives a state stack
// whose states are named "%s_questgiver_fsm" (string at 0x8035FC30). Bounds come from
// the static initializers of this unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/externs.h"
#include "game/unk_803B53A0.h"
#include "game/unk_803C4298.h"
#include "game/unk_803E6D00.h"
#include "game/unk_vec3.h"

// Real names unknown.
struct UnkState;

struct UnkStateInfo {
    /* 0x00 */ u8 unk0[0x30];
    /* 0x30 */ u32 mId;
};

struct UnkStateStack {
    /* 0x000 */ u32 mDepth;
    /* 0x004 */ UnkState* mStack[8];
    /* 0x024 */ UnkState** mCurrent;
    /* 0x028 */ UnkState* mStates[0x100];
    /* 0x428 */ u32 mCount;
    /* 0x42C */ UnkState* mPending;
    /* 0x430 */ u8 m430;
    /* 0x434 */ UnkState* m434; // cleared after fn_8001AFB4(stack, 0)
};

// Entry of the table at 0x80369D60, indexed by lbl_803C4298.m1C.
struct UnkQuestGiverName {
    /* 0x00 */ const char* mName;
    /* 0x04 */ u8 unk4[0x20 - 0x4];
};

struct UnkCombatState {
    /* 0x000 */ u8 unk0[0x138];
    /* 0x138 */ int m138;
    /* 0x13C */ u8 unk13C[0xA1C - 0x13C];
    /* 0xA1C */ int mA1C;
};

struct UnkChallengeState {
    /* 0x00 */ u8 unk0[0x6C];
    /* 0x6C */ u8 m6C;
};

struct UnkModeEntity {
    /* 0x000 */ u8 unk0[0x2C];
    /* 0x02C */ u32 mId;
    /* 0x030 */ u8 unk30[0xA00 - 0x30];
    /* 0xA00 */ int mA00;
};

// m28 indexes the per-mode entities and takes lbl_803E6D00.m0->m1E0 (meaning unknown).
struct UnkQuestGiverFsm {
    UnkQuestGiverFsm();
    ~UnkQuestGiverFsm();

    /* 0x00 */ u32 m0;
    /* 0x04 */ UnkModeEntity* mModeEntities[8];
    /* 0x24 */ UnkModeEntity* mEntity;
    /* 0x28 */ int mMode;
    /* 0x2C */ int mPrevMode;
    /* 0x30 */ u32 m30;
    /* 0x34 */ u32 m34;
    /* 0x38 */ u32 m38;
    /* 0x3C */ int m3C;
    /* 0x40 */ int m40;
    /* 0x44 */ UnkStateStack* mStack;
};

extern UnkQuestGiverName lbl_80369D60[];
extern UnkCombatState* lbl_805FBBE0;
extern UnkChallengeState* lbl_805FBBC8;
extern int lbl_805FB94C;
extern f32 lbl_805FB918;

extern "C" {
void fn_8001AFB4(UnkStateStack* stack, UnkState** current);
UnkState* fn_8001B044(UnkStateStack* stack, u32 id);
UnkState* fn_8001B094(UnkStateStack* stack);
void fn_8001B09C(UnkStateStack* stack, UnkState* state);
void fn_8001B0B8(UnkStateStack* stack, u32 id);
void fn_8001B128(UnkStateStack* stack);
void fn_8001B164(UnkStateStack* stack);
void fn_801BB5BC(UnkState* state, f32 dt);
void fn_801BBB68(UnkState* state, u32 id, int arg2, int arg3);
u32 fn_801EB704(const char* text);
void fn_8000C314(void* obj);
void fn_80051940(f32 dt);
void fn_80012B5C(void* obj, f32 dt);
void fn_80012CB0(void* obj, f32 dt);
UnkModeEntity* fn_800D8724(UnkQuestGiverFsm* fsm, u32 id);
}

UnkQuestGiverFsm::UnkQuestGiverFsm() {
    mStack = 0;
    m3C = -1;
    m40 = -1;
    if (mStack != 0) {
        fn_8001AFB4(mStack, 0);
        if (mStack->m434 != 0) {
            mStack->m434 = 0;
        }
    }
    mModeEntities[0] = 0;
    mModeEntities[1] = 0;
    mModeEntities[2] = 0;
    mModeEntities[3] = 0;
    mModeEntities[4] = 0;
    mModeEntities[5] = 0;
    mModeEntities[6] = 0;
    mModeEntities[7] = 0;
    mMode = 0;
    mPrevMode = 0;
    mEntity = 0;
    m30 = 0;
    m0 = 0;
    m34 = 0;
    m38 = 0;
}

UnkQuestGiverFsm::~UnkQuestGiverFsm() {}

static inline void SelectMode(UnkQuestGiverFsm* fsm) {
    int mode = lbl_803E6D00.m0->m1E0;
    fsm->mEntity = fsm->mModeEntities[mode];
    fsm->mPrevMode = fsm->mMode;
    fsm->mMode = mode;
    UnkModeEntity* entity = fn_800D8724(fsm, lbl_803E6D00.m0->m1E8);
    if (entity != 0) {
        fsm->mEntity = entity;
        fsm->mModeEntities[lbl_803E6D00.m0->m1E0] = entity;
    }
}

extern "C" {

void fn_800D7FF0(UnkQuestGiverFsm* fsm, int keep) {
    fsm->m3C = -1;
    fsm->m40 = -1;
    if (keep != 0 && fsm->mStack != 0) {
        fn_8001AFB4(fsm->mStack, 0);
        if (fsm->mStack->m434 != 0) {
            fsm->mStack->m434 = 0;
        }
    }
    fsm->mModeEntities[0] = 0;
    fsm->mModeEntities[1] = 0;
    fsm->mModeEntities[2] = 0;
    fsm->mModeEntities[3] = 0;
    fsm->mModeEntities[4] = 0;
    fsm->mModeEntities[5] = 0;
    fsm->mModeEntities[6] = 0;
    fsm->mModeEntities[7] = 0;
    fsm->mMode = 0;
    fsm->mPrevMode = 0;
    fsm->mEntity = 0;
    fsm->m30 = 0;
    fsm->m0 = 0;
    fsm->m34 = 0;
    fsm->m38 = 0;
    if (keep == 0) {
        SelectMode(fsm);
    }
}

void fn_800D8100(UnkQuestGiverFsm* fsm) {
    fsm->mStack = lbl_803E6D00.m0->m1F4;
}

void fn_800D8114(UnkQuestGiverFsm* fsm) {
    if (fsm->mStack != 0) {
        fn_8001AFB4(fsm->mStack, 0);
        if (fsm->mStack->m434 != 0) {
            fsm->mStack->m434 = 0;
        }
    }
}

void fn_800D816C(UnkQuestGiverFsm* fsm, u32 id) {
    if (fsm->mStack->mPending != 0 || lbl_805FBBE0->mA1C != 0 || fsm->mMode == 4) {
        fsm->m38 = id;
    } else {
        fn_8001B0B8(fsm->mStack, id);
    }
}

void fn_800D81AC(UnkQuestGiverFsm* fsm) {
    u32 mode = lbl_803E6D00.m0->m1E0;
    if (mode == 1 || mode == 2) {
        if (fsm->m0 != 0 && fsm->m30 == 0) {
            fsm->m30 = fsm->m0;
            fsm->m0 = 0;
        }
        if (fsm->m30 != 0) {
            if (lbl_803C4298.m18 != 5) {
                if (fsm->mStack->mPending != 0) {
                    fsm->m0 = fsm->m30;
                    fsm->m30 = 0;
                } else {
                    fn_8001B0B8(fsm->mStack, fsm->m30);
                }
            } else if (fn_8001B044(fsm->mStack, fsm->m30) != 0) {
                fn_8001B0B8(fsm->mStack, fsm->m30);
                fn_8001B128(fsm->mStack);
            }
        }
    }
}

void fn_800D8280(UnkQuestGiverFsm* fsm) {
    u32 mode = lbl_803E6D00.m0->m1E0;
    if (mode == 1 || mode == 2) {
        u8 flag;
        char name[0x100];
        fn_80142EF8(name, "%s_questgiver_fsm", lbl_80369D60[lbl_803C4298.m1C].mName);
        u32 id = fn_801EB704(name);
        if (fn_8001B044(fsm->mStack, id) != 0 && lbl_803C4298.m18 == 5) {
            UnkState* pending = fn_8001B094(fsm->mStack);
            flag = fsm->mStack->m430;
            fn_8001B0B8(fsm->mStack, id);
            fn_8001B128(fsm->mStack);
            fn_801BB5BC(fsm->mStack->m434, 1.0f);
            fsm->mStack->m430 = 1;
            fn_8001B164(fsm->mStack);
            if (pending != 0) {
                fn_8001B09C(fsm->mStack, pending);
            }
            if (flag != 0) {
                fsm->mStack->m430 = 1;
            }
        }
    }
}

int fn_800D83A8(UnkQuestGiverFsm* fsm) {
    return fsm->mMode;
}

int fn_800D83B0(UnkQuestGiverFsm* fsm) {
    return fsm->mPrevMode;
}

void fn_800D83B8(UnkQuestGiverFsm* fsm, int mode) {
    fsm->mMode = mode;
    fn_8000C314(&lbl_803C4298);
}

void fn_800D83C8(UnkQuestGiverFsm* fsm, f32 dt) {
    UnkModeEntity* player = 0;
    if (fsm->mMode == 2) {
        player = (UnkModeEntity*)lbl_803B53A0.mD614;
    }
    if (lbl_803C4298.m18 == 5) {
        if ((player != 0 && player->mA00 != 0) || player == 0) {
            if (fsm->mStack->m430 != 0) {
                fn_8001B164(fsm->mStack);
                fn_801BA190(lbl_805FB6C8, fsm->mStack->m434, 0, 0x307B3C9F, 0, 0, 0, 0);
                if (fsm->m0 != 0 && fsm->mStack->mDepth == 0) {
                    fn_8001B0B8(fsm->mStack, fsm->m0);
                    fsm->m30 = fsm->m0;
                    fsm->m0 = 0;
                }
            }
            if (fsm->mStack->mPending != 0) {
                fn_8001B128(fsm->mStack);
            }
            if (fsm->m38 != 0 && fsm->mMode != 4 && lbl_805FBBE0->mA1C == 0) {
                fn_8001B0B8(fsm->mStack, fsm->m38);
                fsm->m38 = 0;
            }
            if (fsm->mStack->m434 != 0) {
                fn_801BB5BC(fsm->mStack->m434, dt);
                if (fsm->m34 != 0) {
                    UnkStateStack* stack = fsm->mStack;
                    if (*stack->mCurrent == fn_8001B044(stack, fsm->m30)) {
                        fn_801BBB68(stack->m434, fsm->m34, 0, 0);
                        fsm->m34 = 0;
                    }
                }
            }
            fn_80051940(dt);
            fn_80012B5C(lbl_803B53A0.m5C8, dt);
            fn_80012CB0(lbl_803B53A0.m5C8, dt);
            if (lbl_803B53A0.m7CC > 0.0f) {
                lbl_803B53A0.m7CC -= lbl_805FB918;
                if (lbl_803B53A0.m7CC <= 0.0f) {
                    lbl_803B53A0.m7CC = 0.0f;
                }
            }
        }
    }
}

void fn_800D85CC(UnkQuestGiverFsm* fsm, int all) {
    if (all != 0) {
        fsm->mModeEntities[0] = 0;
        fsm->mModeEntities[1] = 0;
        fsm->mModeEntities[2] = 0;
        fsm->mModeEntities[3] = 0;
        fsm->mModeEntities[4] = 0;
        fsm->mModeEntities[5] = 0;
        fsm->mModeEntities[6] = 0;
        fsm->mModeEntities[7] = 0;
    }
    fsm->mEntity = 0;
}

void fn_800D8604(UnkQuestGiverFsm* fsm) {
    SelectMode(fsm);
    if (fsm->mMode == 1 && fsm->mPrevMode == 6) {
        fsm->m34 = 0xD5030FA4;
    } else if (fsm->mMode == 2 && fsm->mPrevMode == 6 && lbl_805FBBE0->m138 == 2) {
        fsm->m34 = 0xF700C412;
    } else if (fsm->mMode == 1 && fsm->mPrevMode == 3) {
        if (lbl_805FBBC8->m6C != 0) {
            fsm->m34 = 0xACA6B62C;
        } else {
            fsm->m34 = 0xADCA036F;
        }
    }
}

static inline UnkWireGroup* GetWireGroup(int index) {
    return (UnkWireGroup*)(lbl_803E6D00.m0->mWireGroups + index * lbl_805FB94C);
}

// NonMatching (49 %): our build turns both loops into ctr loops with the count loaded
// once and folds the group index 0; the DOL reloads the count every iteration (as in
// ExWorldWireTile::vfBC), keeps the index in a register (li 0 / mullw) and recomputes
// the group address in the returns. A volatile count inside a one-pass outer loop
// reaches 79 % but is unlikely to be the original source.
UnkModeEntity* fn_800D8724(UnkQuestGiverFsm* fsm, u32 id) {
    int index = 0;
    UnkWireGroup* group = GetWireGroup(index);
    for (int i = 0; i < group->mEntityCount2; i++) {
        if (((UnkModeEntity*)group->mEntities2[i])->mId == id) {
            return (UnkModeEntity*)GetWireGroup(index)->mEntities2[i];
        }
    }
    for (int i = 0; i < group->mEntityCount; i++) {
        if (((UnkModeEntity*)group->mEntities[i])->mId == id) {
            return (UnkModeEntity*)GetWireGroup(index)->mEntities[i];
        }
    }
    return 0;
}

void fn_800D87EC(UnkQuestGiverFsm* fsm, u32 id, int arg2, int arg3, int direct) {
    u32 current = fsm->m30;
    if (current == 0) {
        current = fsm->m0;
    }
    if (current == 0) {
        return;
    }
    UnkState* state = fsm->mStack->m434;
    if (state == 0) {
        return;
    }
    if (direct != 0) {
        fn_801BBB68(state, id, arg2, arg3);
    } else {
        fn_801BA190(lbl_805FB6C8, state, 0, id, arg2, arg3, 0, 0);
    }
}

void fn_800D8854(UnkQuestGiverFsm* fsm, u32 id) {
    fsm->m34 = id;
}

}
