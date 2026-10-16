// Game unit at 0x80077B50-0x80077D1C (real file name unknown): named callbacks
// registered by the next unit through fn_801DC91C. Each one works on the object at
// 0x10 in its argument. Bounds come from the static initializers of this unit and
// the previous one.

#include "types.h"

// Object at 0x10 in the callback argument (real type unknown); 0x290 holds a state.
struct UnkScriptTarget {
    /* 0x000 */ u8 unk0[0x290];
    /* 0x290 */ int mState;
};

struct UnkScriptContext {
    /* 0x00 */ u8 unk0[0x10];
    /* 0x10 */ UnkScriptTarget* mTarget;
};

// Real names unknown.
extern "C" {
void fn_80079C78(UnkScriptTarget* target);
void fn_80079CBC(UnkScriptTarget* target);
int fn_8007B194(UnkScriptTarget* target);
int fn_8007B318(UnkScriptTarget* target);
int fn_8007C080(UnkScriptTarget* target);
int fn_8007C110(UnkScriptTarget* target);
int fn_8007C11C(UnkScriptTarget* target);
int fn_8007C124(UnkScriptTarget* target);
int fn_8007C12C(UnkScriptTarget* target);
f32 fn_8007C138(UnkScriptTarget* target);
int fn_8007C140(UnkScriptTarget* target);
int fn_8007C278(UnkScriptTarget* target);
int fn_8007C3B0(UnkScriptTarget* target);
int fn_80079190(void);
}

extern "C" {

void fn_80077B50(const UnkScriptContext* ctx) {
    UnkScriptTarget* target = ctx->mTarget;
    fn_80079C78(target);
    target->mState = 3;
}

void fn_80077B88(const UnkScriptContext* ctx) {
    UnkScriptTarget* target = ctx->mTarget;
    fn_80079C78(target);
    fn_80079CBC(target);
}

int fn_80077BC0(const UnkScriptContext* ctx) {
    return fn_8007B194(ctx->mTarget);
}

int fn_80077BC8(const UnkScriptContext* ctx) {
    return fn_8007B318(ctx->mTarget) == 0;
}

int fn_80077BF4(const UnkScriptContext* ctx) {
    UnkScriptTarget* target = ctx->mTarget;
    if (fn_8007B318(target) == 1) {
        return 1;
    }
    return fn_8007B318(target) == 2;
}

int fn_80077C48(const UnkScriptContext* ctx) {
    return 0;
}

int fn_80077C50(UnkScriptContext* ctx) {
    return fn_8007C138(ctx->mTarget) >= 1.0f;
}

int fn_80077C88(UnkScriptContext* ctx) {
    return fn_8007C11C(ctx->mTarget) > 3;
}

int fn_80077CC0(const UnkScriptContext* ctx) {
    return fn_8007C124(ctx->mTarget);
}

int fn_80077CC8(const UnkScriptContext* ctx) {
    return fn_8007C080(ctx->mTarget);
}

int fn_80077CD0(const UnkScriptContext* ctx) {
    return fn_8007C110(ctx->mTarget);
}

int fn_80077CD8(const UnkScriptContext* ctx) {
    return fn_80079190();
}

int fn_80077CDC(const UnkScriptContext* ctx) {
    return fn_8007C12C(ctx->mTarget);
}

int fn_80077CE4(const UnkScriptContext* ctx) {
    return fn_8007C140(ctx->mTarget);
}

int fn_80077CEC(const UnkScriptContext* ctx) {
    return fn_8007C278(ctx->mTarget);
}

int fn_80077CF4(const UnkScriptContext* ctx) {
    return fn_8007C3B0(ctx->mTarget);
}

} // extern "C"

#include "game/unk_vec3.h"
