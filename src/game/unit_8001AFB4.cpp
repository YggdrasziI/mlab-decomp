// Game unit at 0x8001AFB4-0x8001B20C (real file name unknown): a stack of states.
// The current state lives in a slot owned by someone else; a pending state can be
// chosen by id from a table, then pushed; popping restores the previous one. Bounds
// come from the static initializers of this unit and the previous one.

#include "game/externs.h"
#include "game/unk_vec3.h"

// Real names unknown.
struct UnkStateInfo {
    /* 0x00 */ u8 unk0[0x30];
    /* 0x30 */ u32 mId;
};

struct UnkState {
    /* 0x00 */ UnkStateInfo* mInfo;
    /* 0x04 */ u8 unk4[0x28 - 0x4];
    /* 0x28 */ u32 mFlags;
};

struct UnkStateStack {
    UnkState* Find(u32 id) {
        for (u32 i = 0; i < mCount; i++) {
            if (mStates[i]->mInfo->mId == id) {
                return mStates[i];
            }
        }
        return 0;
    }

    void SetPending(UnkState* state) {
        (*mCurrent)->mFlags |= 0x200;
        mPending = state;
    }

    /* 0x000 */ u32 mDepth;
    /* 0x004 */ UnkState* mStack[8];
    /* 0x024 */ UnkState** mCurrent;
    /* 0x028 */ UnkState* mStates[0x100];
    /* 0x428 */ u32 mCount;
    /* 0x42C */ UnkState* mPending;
    /* 0x430 */ u8 m430;
};

extern "C" {

void fn_801BC574(UnkStateInfo* info);

void fn_8001AFB4(UnkStateStack* stack, UnkState** current) {
    stack->mCurrent = current;
    stack->mDepth = 0;
    stack->mPending = 0;
    stack->m430 = 0;
}

void fn_8001AFCC(UnkStateStack* stack) {
    for (u32 i = 0; i < stack->mCount; i++) {
        UnkState* state = stack->mStates[i];
        if (state != 0 && state->mInfo != 0) {
            if (state->mInfo != 0) {
                fn_801BC574(state->mInfo);
            }
        }
    }
}

UnkState* fn_8001B044(UnkStateStack* stack, u32 id) {
    for (u32 i = 0; i < stack->mCount; i++) {
        if (stack->mStates[i]->mInfo->mId == id) {
            return stack->mStates[i];
        }
    }
    return 0;
}

UnkState* fn_8001B094(UnkStateStack* stack) {
    return stack->mPending;
}

void fn_8001B09C(UnkStateStack* stack, UnkState* state) {
    stack->SetPending(state);
}

void fn_8001B0B8(UnkStateStack* stack, u32 id) {
    UnkState* state = stack->Find(id);
    if (state != 0) {
        stack->SetPending(state);
    }
}

void fn_8001B128(UnkStateStack* stack) {
    stack->mStack[stack->mDepth] = *stack->mCurrent;
    *stack->mCurrent = stack->mPending;
    stack->mPending = 0;
    stack->mDepth++;
}

void fn_8001B164(UnkStateStack* stack) {
    if (stack->mDepth == 0) {
        stack->m430 = 0;
    } else {
        stack->mDepth--;
        fn_801BB478(*stack->mCurrent, 0);
        *stack->mCurrent = stack->mStack[stack->mDepth];
        stack->mStack[stack->mDepth] = 0;
        stack->m430 = 0;
    }
}

}
