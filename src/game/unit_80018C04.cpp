// Game unit at 0x80018C04-0x80018E00 (real file name unknown): registers three named
// animation predicates through fn_801DC91C, then calls other registration functions.
// Bounds come from the static initializers of this unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/unk_vec3.h"

// Object at 0xA0 in the callback target: animation state (real type unknown).
struct UnkAnimState {
    /* 0x00 */ u8 unk0[0xEC];
    /* 0xEC */ int mEC;
    /* 0xF0 */ u32 mFlags;
};

struct UnkAnimEntry {
    /* 0x00 */ u8 unk0[0x28];
    /* 0x28 */ int mFrame;
};

struct UnkAnimOwner {
    /* 0x00 */ u8 unk0[0x48];
    /* 0x48 */ void* mSet;
};

struct UnkAnimTarget {
    /* 0x00 */ u8 unk0[0x9C];
    /* 0x9C */ UnkAnimOwner* m9C;
    /* 0xA0 */ UnkAnimState* mA0;
};

struct UnkAnimContext {
    /* 0x00 */ u8 unk0[0x10];
    /* 0x10 */ UnkAnimTarget* mTarget;
};

typedef int (*UnkNamedPredicate)(const UnkAnimContext* ctx);

// Real names unknown.
extern "C" {
void fn_801DC91C(const char* name, int type, UnkNamedPredicate func);
int fn_801AC2FC(void* set, int id);
UnkAnimEntry* fn_801AC164(void* set, int index);
f32 fn_8018BE78(UnkAnimState* state);
f32 fn_8018C014(UnkAnimState* state, f32 rate);
f32 fn_8018C07C(UnkAnimState* state);
void fn_80014350(void);
void fn_80019CC8(void);
void fn_800195A4(void);
void fn_80074D80(void);
void fn_800197A8(void);
}
void fn_8006F5A0();

extern "C" {

int fn_80018C84(const UnkAnimContext* ctx);
int fn_80018CB0(const UnkAnimContext* ctx);
int fn_80018D64(const UnkAnimContext* ctx);

void fn_80018C04(void) {
    fn_801DC91C("anim_done", 29, fn_80018C84);
    fn_801DC91C("anim_past_impact_frame", 29, fn_80018CB0);
    fn_801DC91C("anim_past_preload_time", 29, fn_80018D64);
    fn_80014350();
    fn_8006F5A0();
    fn_80019CC8();
    fn_800195A4();
    fn_80074D80();
    fn_800197A8();
}

int fn_80018C84(const UnkAnimContext* ctx) {
    UnkAnimTarget* target = ctx->mTarget;
    if (target != 0 && (target->mA0->mFlags & 0x10)) {
        return 1;
    }
    return 0;
}

int fn_80018CB0(const UnkAnimContext* ctx) {
    UnkAnimTarget* target = ctx->mTarget;
    if (target != 0) {
        UnkAnimOwner* owner = target->m9C;
        if (owner != 0 && owner->mSet != 0) {
            int index = fn_801AC2FC(owner->mSet, target->mA0->mEC);
            if (index >= 0) {
                int frame = fn_801AC164(owner->mSet, index)->mFrame;
                if (fn_8018C014(target->mA0, 30.0f) >= frame) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

int fn_80018D64(const UnkAnimContext* ctx) {
    UnkAnimTarget* target = ctx->mTarget;
    if (target != 0 && target->mA0 != 0) {
        f32 time = fn_8018BE78(target->mA0);
        if (fn_8018C07C(target->mA0) - time <= 1.0f) {
            return 1;
        }
    }
    return 0;
}

}
