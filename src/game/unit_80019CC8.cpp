// Game unit at 0x80019CC8-0x80019FB0 (real file name unknown): script callbacks for
// props that send up to three delayed events ("check_delayed_timer",
// "send_delayed_event_to_zone", "reset_trigger_send_delayed_events"), registered
// through fn_801DC91C. Bounds come from the static initializers of this unit and
// the previous one.

#include "game/unk_vec3.mch"
#include "game/Entity.h"
#include "game/externs.h"
#include "game/unk_vec3.h"

struct UnkPropContext {
    /* 0x00 */ u8 unk0[0x10];
    /* 0x10 */ Entity_Prop* mEntity;
};

// State of the entity's state machine (0x30 bytes, real name unknown).
struct UnkMachineState {
    /* 0x00 */ u8 unk0[0x4];
    /* 0x04 */ u32 mHash;
    /* 0x08 */ u8 unk8[0x30 - 0x8];
};

struct UnkMachineStates {
    /* 0x0 */ UnkMachineState* mStates;
};

// Object at the start of Entity::mA4 (real name unknown).
struct UnkMachine {
    UnkMachineState* GetCurrent() {
        if (mCurrent > -1) {
            return &mStates->mStates[mCurrent];
        }
        return 0;
    }

    /* 0x0 */ UnkMachineStates* mStates;
    /* 0x4 */ int mCurrent;
};

static inline f32 GetTime(Entity* entity) {
    return entity->m28;
}

extern "C" {
void fn_801DC91C(const char* name, int type, void* func);
void fn_8006EBC4(Entity_Prop* entity, int event);

int fn_80019D30(Entity_Prop* entity);
void fn_80019DCC(Entity_Prop* entity);
void fn_80019F68(UnkPropContext* ctx);

void fn_80019CC8(void) {
    fn_801DC91C("check_delayed_timer", 22, (void*)fn_80019D30);
    fn_801DC91C("send_delayed_event_to_zone", 47, (void*)fn_80019DCC);
    fn_801DC91C("reset_trigger_send_delayed_events", 78, (void*)fn_80019F68);
}

int fn_80019D30(Entity_Prop* entity) {
    if (entity == 0 || !(entity->mInfo->mTypeMask & 0x100)) {
        return 0;
    }
    if (entity->mA4 != 0 && *entity->mA4 != 0) {
        UnkMachineState* state = ((UnkMachine*)*entity->mA4)->GetCurrent();
        u32 hash = state != 0 ? state->mHash : 0;
        if (hash == 0xBD1B1FC9) {
            return 0;
        }
    }
    return entity->m164 < GetTime(entity);
}

static inline void SendDelayedEvent(Entity_Prop* entity) {
    if (entity->mA4 != 0 && *entity->mA4 != 0) {
        fn_801BA190(lbl_805FB6C8, entity, entity, 0xF5A9CE6C, 0, 0, 1, 0);
    }
}

void fn_80019DCC(Entity_Prop* entity) {
    if (entity == 0 || !(entity->mInfo->mTypeMask & 0x100)) {
        return;
    }
    switch (entity->m174) {
    case 0:
        fn_8006EBC4(entity, entity->m168);
        entity->m174 = 1;
        entity->m164 = entity->m15CFloat + GetTime(entity);
        if (entity->m16C == 0) {
            SendDelayedEvent(entity);
        }
        break;
    case 1:
        fn_8006EBC4(entity, entity->m16C);
        entity->m174 = 2;
        entity->m164 = entity->m160 + GetTime(entity);
        if (entity->m170 == 0) {
            SendDelayedEvent(entity);
        }
        break;
    case 2:
        fn_8006EBC4(entity, entity->m170);
        entity->m174 = 3;
        SendDelayedEvent(entity);
        break;
    }
}

void fn_80019F68(UnkPropContext* ctx) {
    Entity_Prop* entity = ctx->mEntity;
    if (entity != 0) {
        entity->m164 = entity->m158 + GetTime(entity);
        entity->m174 = 0;
    }
}
}
