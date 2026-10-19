// Game unit at 0x800197A8-0x80019CC8 (real file name unknown): named script
// callbacks for props and hazard props ("prop_play_anim_and_fx_once",
// "prop_deactivate", "prop_counting_down", "prop_check_timer", "prop_anim_done",
// "NavTest", "reset_deathcount", "increment_deathcount", "increment_entity_state",
// "do_trigger_action", "start_trigger_proximity_text" and the
// "hazard_prop_idle_*" / "hazard_prop_attack_*" states), registered through
// fn_801DC91C. Bounds come from the static initializers of this unit and the
// previous one.
// The callbacks take a const context (see unit_80018E00.cpp); fn_80019A9C writes
// its result through a const_cast.

#include "game/unk_vec3.mch"
#include "game/Entity.h"
#include "game/externs.h"
#include "game/unk_vec3.h"

// Value slot of the script context, read and written as a float.
union UnkScriptValue {
    u32 mBits;
    f32 mFloat;
};

struct UnkPropContext {
    /* 0x00 */ u8 unk0[0x10];
    /* 0x10 */ Entity_Hazard_Prop* mEntity;
    /* 0x14 */ u8 unk14[0x2C - 0x14];
    /* 0x2C */ UnkScriptValue mValue;
};

// Re-entrancy guard (constructor 0x801F5384, destructor 0x801F53B0; real name
// unknown).
class UnkScopedGuard {
public:
    UnkScopedGuard();
    ~UnkScopedGuard();

    /* 0x0 */ int m0;
};

// Navigation key built by "NavTest" (real name unknown).
struct UnkNavKey {
    /* 0x0 */ u8 m0;
    /* 0x1 */ u8 m1;
    /* 0x2 */ u16 m2;
};

static int lbl_805FAF70;

extern "C" {
void fn_801DC91C(const char* name, int type, void* func);
void fn_8006ED68(Entity* entity, int arg);
void fn_80069350(Entity* entity, int arg1, int arg2, int arg3);
void fn_801254F8(Entity* entity, void* arg);
void fn_80069FDC(Entity* entity);
void fn_801E0054(void* registers, Entity* entity, int index);
void fn_801ED314(UnkNavKey* key, int arg);
void fn_801B8734(Entity* entity, UnkNavKey key, f32 arg);

void fn_80019930(const UnkPropContext* ctx);
void fn_80019950(const UnkPropContext* ctx);
void fn_80019954(const UnkPropContext* ctx);
void fn_800199A8(const UnkPropContext* ctx);
void fn_800199C8(const UnkPropContext* ctx);
void fn_800199CC(const UnkPropContext* ctx);
void fn_80019A20(const UnkPropContext* ctx);
void fn_80019A38(const UnkPropContext* ctx);
void fn_80019A9C(const UnkPropContext* ctx);
int fn_80019AD8(const UnkPropContext* ctx);
void fn_80019B04(const UnkPropContext* ctx);
int fn_80019B60(const UnkPropContext* ctx);
void fn_80019BA0(const UnkPropContext* ctx);
void fn_80019BB4(const UnkPropContext* ctx);
void fn_80019BCC(const UnkPropContext* ctx);
void fn_80019BEC(const UnkPropContext* ctx);
void fn_80019C18(const UnkPropContext* ctx);

void fn_800197A8(void) {
    fn_801DC91C("prop_play_anim_and_fx_once", 78, (void*)fn_80019A38);
    fn_801DC91C("prop_deactivate", 78, (void*)fn_80019B04);
    fn_801DC91C("prop_counting_down", 78, (void*)fn_80019A9C);
    fn_801DC91C("prop_check_timer", 29, (void*)fn_80019AD8);
    fn_801DC91C("prop_anim_done", 29, (void*)fn_80019B60);
    fn_801DC91C("NavTest", 78, (void*)fn_80019C18);
    fn_801DC91C("reset_deathcount", 78, (void*)fn_80019BB4);
    fn_801DC91C("increment_deathcount", 78, (void*)fn_80019BCC);
    fn_801DC91C("increment_entity_state", 78, (void*)fn_80019BA0);
    fn_801DC91C("do_trigger_action", 78, (void*)fn_80019BEC);
    fn_801DC91C("start_trigger_proximity_text", 78, (void*)fn_80019A20);
    fn_801DC91C("hazard_prop_idle_entry", 78, (void*)fn_80019930);
    fn_801DC91C("hazard_prop_idle_exit", 78, (void*)fn_80019950);
    fn_801DC91C("hazard_prop_idle_update", 78, (void*)fn_80019954);
    fn_801DC91C("hazard_prop_attack_entry", 78, (void*)fn_800199A8);
    fn_801DC91C("hazard_prop_attack_exit", 78, (void*)fn_800199C8);
    fn_801DC91C("hazard_prop_attack_update", 78, (void*)fn_800199CC);
    lbl_805FAF70 = 0;
}

void fn_80019930(const UnkPropContext* ctx) {
    Entity_Hazard_Prop* entity = ctx->mEntity;
    if (entity != 0) {
        entity->m264 = 0.0f;
        entity->m268 = 0;
    }
}

void fn_80019950(const UnkPropContext* ctx) {}

void fn_80019954(const UnkPropContext* ctx) {
    Entity_Hazard_Prop* entity = ctx->mEntity;
    if (entity != 0) {
        entity->m264 += entity->m24;
        f32 limit = entity->m274;
        if (entity->m264 >= limit) {
            fn_801BA190(lbl_805FB6C8, entity, 0, 0x1FF53675, 0, 0, 0, 0);
        }
    }
}

void fn_800199A8(const UnkPropContext* ctx) {
    Entity_Hazard_Prop* entity = ctx->mEntity;
    if (entity != 0) {
        entity->m264 = 0.0f;
        entity->m268 = 1;
    }
}

void fn_800199C8(const UnkPropContext* ctx) {}

void fn_800199CC(const UnkPropContext* ctx) {
    Entity_Hazard_Prop* entity = ctx->mEntity;
    if (entity != 0) {
        entity->m264 += entity->m24;
        f32 limit = entity->m278;
        if (entity->m264 >= limit) {
            fn_801BA190(lbl_805FB6C8, entity, 0, 0xE9BAFAD5, 0, 0, 0, 0);
        }
    }
}

void fn_80019A20(const UnkPropContext* ctx) {
    Entity_Hazard_Prop* entity = ctx->mEntity;
    if (entity != 0) {
        fn_8006ED68(entity, entity->m168);
    }
}

void fn_80019A38(const UnkPropContext* ctx) {
    Entity_Hazard_Prop* entity = ctx->mEntity;
    if (entity != 0) {
        fn_80069350(entity, 0, 0, 1);
        if (entity->m9C != 0 && entity->m9C->mB4 != 0) {
            fn_801254F8(entity, entity->m9C->mB4);
        }
    }
}

void fn_80019A9C(const UnkPropContext* ctx) {
    Entity_Hazard_Prop* entity = ctx->mEntity;
    f32 result = 0.0f;
    if (entity != 0) {
        UnkScriptValue value = ctx->mValue;
        f32 time = entity->m24;
        result = value.mFloat - time;
    }
    UnkScriptValue out;
    out.mFloat = result;
    const_cast<UnkPropContext*>(ctx)->mValue = out;
}

int fn_80019AD8(const UnkPropContext* ctx) {
    UnkScriptValue value = ctx->mValue;
    return value.mFloat <= 0.0f;
}

void fn_80019B04(const UnkPropContext* ctx) {
    Entity_Hazard_Prop* entity = ctx->mEntity;
    if (entity != 0) {
        UnkScopedGuard guard;
        entity->vf18((void*)1);
    }
}

int fn_80019B60(const UnkPropContext* ctx) {
    Entity_Hazard_Prop* entity = ctx->mEntity;
    if (entity != 0 && entity->mA0 != 0 && (entity->mA0->mFlags & 0x10)) {
        entity->m15C |= 1;
        return 1;
    }
    return 0;
}

void fn_80019BA0(const UnkPropContext* ctx) {
    Entity_Hazard_Prop* entity = ctx->mEntity;
    if (entity != 0) {
        fn_80069FDC(entity);
    }
}

void fn_80019BB4(const UnkPropContext* ctx) {
    Entity_Hazard_Prop* entity = ctx->mEntity;
    if (entity != 0) {
        entity->m164 = 0.0f;
    }
}

void fn_80019BCC(const UnkPropContext* ctx) {
    Entity_Hazard_Prop* entity = ctx->mEntity;
    if (entity != 0) {
        entity->m164 += 1.0f;
    }
}

void fn_80019BEC(const UnkPropContext* ctx) {
    Entity_Hazard_Prop* entity = ctx->mEntity;
    if (entity != 0 && (entity->mInfo->mTypeMask & 0x100)) {
        fn_801E0054(&entity->mSlot.mCur.m0[2], entity, entity->m1AC);
    }
}

void fn_80019C18(const UnkPropContext* ctx) {
    f32 radius = 2.0f;
    Entity_Hazard_Prop* entity = ctx->mEntity;
    UnkNavKey key;
    key.m2 = 0xFFFF;
    key.m0 = 0;
    key.m1 = 0;
    key.m1 = entity->mInfo->m32;
    key.m2 = entity->mInfo->m3A;
    fn_801ED314(&key, 2);
    fn_801B8734(entity, key, radius);
}

}
