// Game unit at 0x80018E00-0x800197A8 (real file name unknown): camera and in-game
// cutscene script callbacks ("acquire_camera_semaphore", "set_igc_mode",
// "SetCameraMode", "set_camera_location", "send_event", "should_igc_abort"...),
// registered through fn_801DC91C. Bounds come from the static initializers of this
// unit and the previous one.
// The callbacks take a const context: a load through a const pointer may be
// scheduled above the prologue stores, as in the DOL.
// NonMatching: fn_80019268 (99.9 %) loads entity->m170 before entity->m16C (the DOL
// loads them the other way round); fn_800195A4 only differs by string relocations.

#include "game/Entity.h"
#include "game/externs.h"
#include "game/unk_803C4298.h"
#include "game/unk_803E6D00.h"
#include "game/unk_vec3.h"

struct UnkScriptContext {
    /* 0x00 */ u8 unk0[0x10];
    /* 0x10 */ Entity_Prop* mEntity;
    /* 0x14 */ u8 unk14[0x1C - 0x14];
    /* 0x1C */ u32 mArg0;
    /* 0x20 */ u32 mArg1;
    /* 0x24 */ u8 unk24[0x4C - 0x24];
    /* 0x4C */ f32 mTimeout;
    /* 0x50 */ f32 mTimeLeft;
};

// Camera location of the level (real type unknown).
struct UnkCameraLocation {
    /* 0x00 */ u8 unk0[0x8];
    /* 0x08 */ UnkVec3 mPos;
    /* 0x14 */ UnkQuat mRot;
};

extern "C" {
void fn_801DC91C(const char* name, int type, void* func);
u32 fn_801B9B40(int channel);
void fn_801B9BC4(int channel, u32 bits);
UnkCameraLocation* fn_8012AA50(UnkGlobals803E6D00* globals, u32 hash);
UnkQuat fn_80145B00(const UnkQuat* rot);
UnkVec3 fn_801454CC(const UnkQuat* rot);
void fn_8006D3C8(void* obj, int arg);
void fn_801BA2FC(void* mgr, u32 hash, void* sender, u32 arg, int arg4, int arg5);
void fn_801BBAEC(const void* ctx, u32 arg0, u32 arg1);
void fn_801BBB0C(const void* ctx, u32 arg0, u32 arg1);
void fn_80121898(const char* text, u32 color);
extern u8 lbl_805FB1A1;

// Entity holding the camera semaphore, set while an in-game cutscene runs, and the
// number of semaphore holders.
Entity_Prop* lbl_805FAF60;
u8 lbl_805FAF64;
int lbl_805FAF68;

int fn_80018E00(const UnkScriptContext* ctx);
void fn_80018E54(const UnkScriptContext* ctx);
void fn_80018E6C(const UnkScriptContext* ctx);
void fn_80018EB4(const UnkScriptContext* ctx);
void fn_80018F40(const UnkScriptContext* ctx);
void fn_80018FB4(const UnkScriptContext* ctx);
void fn_80018FEC(const UnkScriptContext* ctx);
void fn_80019044(const UnkScriptContext* ctx);
void fn_800190C8(const UnkScriptContext* ctx);
void fn_800190E4(const UnkScriptContext* ctx);
void fn_80019268(const UnkScriptContext* ctx);
void fn_80019420(const UnkScriptContext* ctx);
void fn_80019438(const UnkScriptContext* ctx);
void fn_80019458(const UnkScriptContext* ctx);
void fn_80019464(const UnkScriptContext* ctx);
void fn_80019470(const UnkScriptContext* ctx);
void fn_80019480(UnkScriptContext* ctx);
void fn_800194BC(const UnkScriptContext* ctx);
void fn_80019548(const UnkScriptContext* ctx);
void fn_80019554(const UnkScriptContext* ctx);
void fn_8001957C(const UnkScriptContext* ctx);
int fn_80019594(const UnkScriptContext* ctx);
}

static inline void SetCameraLocation(u32 locationHash, u32 targetHash) {
    UnkCameraLocation* location = fn_8012AA50(&lbl_803E6D00, locationHash);
    UnkCameraLocation* target = 0;
    if (targetHash != 0) {
        target = fn_8012AA50(&lbl_803E6D00, targetHash);
    }
    if (location != 0) {
        if (target != 0) {
            lbl_803C4298.mCamera->vf2C(&location->mPos, &target->mPos);
            lbl_803C4298.mCamera->vf1C(&location->mPos);
        } else {
            UnkQuat rot = fn_80145B00(&location->mRot);
            UnkVec3 at = fn_801454CC(&rot);
            at += location->mPos;
            lbl_803C4298.mCamera->vf2C(&location->mPos, &at);
            lbl_803C4298.mCamera->vf1C(&location->mPos);
        }
    } else {
        lbl_803C4298.mCamera->vf2C(lbl_803C4298.mCamera->mPos, &lbl_803E6D00.m0->m8->mInfo->mPos);
        lbl_803C4298.mCamera->vf1C(lbl_803C4298.mCamera->mPos);
    }
}

static inline void ClearIgcMode() {
    if (lbl_805FAF68 <= 0) {
        lbl_805FAF64 = 0;
    }
}

extern "C" {

int fn_80018E00(const UnkScriptContext* ctx) {
    if (fn_801B9B40((int)lbl_803C4298.m38) & 0x80) {
        fn_801B9BC4((int)lbl_803C4298.m38, 5);
        return 1;
    }
    return 0;
}

void fn_80018E54(const UnkScriptContext* ctx) {
    lbl_805FAF60 = ctx->mEntity;
    lbl_805FAF68++;
}

void fn_80018E6C(const UnkScriptContext* ctx) {
    if (lbl_805FAF60 == ctx->mEntity) {
        lbl_805FAF60 = 0;
    }
    if (--lbl_805FAF68 < 0) {
        lbl_805FAF68 = 0;
    }
    if (lbl_805FAF68 <= 0) {
        ClearIgcMode();
    }
}

void fn_80018EB4(const UnkScriptContext* ctx) {
    UnkCamera* camera = lbl_803C4298.mCamera;
    u32 hash = ctx->mArg0;
    if (camera == 0) {
        return;
    }
    if (hash == 0x8A54CB00) {
        camera->vf0C(7);
    } else if (hash == 0x7BBB2C7C) {
        camera->vf0C(5);
    } else if (hash == 0x699D0C82) {
        camera->vf0C(6);
    } else {
        camera->vf10();
    }
}

void fn_80018F40(const UnkScriptContext* ctx) {
    UnkCamera* camera = lbl_803C4298.mCamera;
    if (camera == 0) {
        return;
    }
    Entity_Prop* entity = ctx->mEntity;
    if (entity == 0) {
        return;
    }
    u32 type = entity->m9C->m8;
    if (type == 0x4BA45FB7) {
        camera->vf0C(6);
    } else if (type == 0x5C1F86EE) {
        camera->vf0C(7);
    }
}

void fn_80018FB4(const UnkScriptContext* ctx) {
    if (lbl_805FAF68 > 1) {
        return;
    }
    UnkCamera* camera = lbl_803C4298.mCamera;
    if (camera != 0) {
        camera->vf10();
    }
}

void fn_80018FE8(const UnkScriptContext* ctx) {}

void fn_80018FEC(const UnkScriptContext* ctx) {
    lbl_803C4298.mCamera->vf30(ctx->mArg0);
}

void fn_80019040(const UnkScriptContext* ctx) {}

void fn_80019044(const UnkScriptContext* ctx) {
    Entity_Prop* entity = ctx->mEntity;
    if (entity != 0 && (entity->mInfo->mTypeMask & 0x100)) {
        if (entity != 0 && lbl_805FAF68 == 1 && entity->m174 != 0) {
            lbl_803C4298.mCamera->vf20();
        }
    } else if (lbl_805FAF68 == 1) {
        lbl_803C4298.mCamera->vf20();
    }
}

void fn_800190C4(const UnkScriptContext* ctx) {}

void fn_800190C8(const UnkScriptContext* ctx) {
    lbl_803C4298.mCamera->vf20();
}

void fn_800190E4(const UnkScriptContext* ctx) {
    u32 location = ctx->mArg0;
    u32 target = ctx->mArg1;
    SetCameraLocation(location, target);
}

void fn_80019268(const UnkScriptContext* ctx) {
    Entity_Prop* entity = ctx->mEntity;
    if (entity != 0) {
        u32 location = entity->m16C;
        u32 target = entity->m170;
        SetCameraLocation(location, target);
        lbl_803C4298.mCamera->vf30(entity->m164);
    }
}

void fn_80019420(const UnkScriptContext* ctx) {
    Entity_Prop* entity = ctx->mEntity;
    if (entity != 0) {
        fn_8006D3C8(entity, 0);
    }
}

void fn_80019438(const UnkScriptContext* ctx) {
    fn_801BA2FC(lbl_805FB6C8, ctx->mArg0, ctx->mEntity, ctx->mArg1, 0, 0);
}

void fn_80019458(const UnkScriptContext* ctx) {
    fn_801BBAEC(ctx, ctx->mArg0, ctx->mArg1);
}

void fn_80019464(const UnkScriptContext* ctx) {
    fn_801BBB0C(ctx, ctx->mArg0, ctx->mArg1);
}

void fn_80019470(const UnkScriptContext* ctx) {
    fn_80121898("ping", 0xA4FFFF);
}

void fn_80019480(UnkScriptContext* ctx) {
    Entity_Prop* entity = ctx->mEntity;
    if (entity != 0) {
        f32 timeout = entity->m158;
        if (timeout > 30.0f) {
            timeout = 30.0f;
        }
        if (timeout < 0.0f) {
            timeout = 0.0f;
        }
        ctx->mTimeout = timeout;
        ctx->mTimeLeft = timeout;
    }
}

void fn_800194BC(const UnkScriptContext* ctx) {
    Entity_Prop* entity = ctx->mEntity;
    if (entity != 0) {
        u32 event = entity->m15C;
        if (event != 0) {
            if (entity->m184 != 0) {
                fn_801BA2FC(lbl_805FB6C8, entity->m184, entity, event, 0, 0);
            }
            if (entity->m188 != 0) {
                fn_801BA2FC(lbl_805FB6C8, entity->m188, entity, event, 0, 0);
            }
        }
    }
}

void fn_80019548(const UnkScriptContext* ctx) {
    lbl_805FAF64 = 1;
}

void fn_80019554(const UnkScriptContext* ctx) {
    Entity_Prop* entity = ctx->mEntity;
    if (entity != 0 && 0.0 != entity->m160) {
        lbl_805FAF64 = 1;
    }
}

void fn_8001957C(const UnkScriptContext* ctx) {
    if (lbl_805FAF68 <= 0) {
        lbl_805FAF64 = 0;
    }
}

int fn_80019594(const UnkScriptContext* ctx) {
    return lbl_805FB1A1 == 0;
}

void fn_800195A4(void) {
    fn_801DC91C("acquire_camera_semaphore", 78, (void*)fn_80018E54);
    fn_801DC91C("release_camera_semaphore", 78, (void*)fn_80018E6C);
    fn_801DC91C("set_igc_mode", 78, (void*)fn_80019548);
    fn_801DC91C("set_igc_mode_from_sensor_param", 78, (void*)fn_80019554);
    fn_801DC91C("clear_igc_mode", 78, (void*)fn_8001957C);
    fn_801DC91C("SetCameraMode", 78, (void*)fn_80018EB4);
    fn_801DC91C("set_camera_mode_from_sensor_type", 78, (void*)fn_80018F40);
    fn_801DC91C("clear_camera_mode", 78, (void*)fn_80018FB4);
    fn_801DC91C("set_camera_location", 78, (void*)fn_800190E4);
    fn_801DC91C("end_trigger_camera", 78, (void*)fn_80019420);
    fn_801DC91C("send_event", 78, (void*)fn_80019438);
    fn_801DC91C("send_request", 78, (void*)fn_80019458);
    fn_801DC91C("send_request_complete", 78, (void*)fn_80019464);
    fn_801DC91C("ping", 78, (void*)fn_80019470);
    fn_801DC91C("broadcast_on_done_event", 78, (void*)fn_800194BC);
    fn_801DC91C("set_camera_location_from_sensor_param", 78, (void*)fn_80019268);
    fn_801DC91C("set_timeout_from_sensor_param", 78, (void*)fn_80019480);
    fn_801DC91C("should_igc_abort", 29, (void*)fn_80018E00);
    fn_801DC91C("special_camera_done_slerping", 29, (void*)fn_80019594);
    fn_801DC91C("set_interp_timer", 78, (void*)fn_80018FEC);
    fn_801DC91C("fixed_camera_return", 78, (void*)fn_80019044);
    fn_801DC91C("force_fixed_camera_return", 78, (void*)fn_800190C8);
}
}
