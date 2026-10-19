// Game unit at 0x80014350-0x80016400 (real file name unknown): creature state
// callbacks ("PatrolIdleEntry", "CombatPlaceholderUpdate", "PumpkinShakeUpdate",
// "ObstacleShakeUpdate", "HazardDeathEntry", "MovingChallengeFaceCamera",
// "PlayerInCombatRange"...), registered through fn_801DC91C, and the shake helpers
// they share. Bounds come from the static initializers of this unit and the previous
// one.
// NonMatching: fn_80015404 (97.2 %, callee-saved register numbering). Several loads
// only match through const accessors (see unit_80018E00.cpp).

#include "game/unk_vec3.mch"
#include "game/Entity.h"
#include "game/externs.h"
#include "game/unit_800214D4.h"
#include "game/unk_803B53A0.h"
#include "game/unk_803C4298.h"
#include "game/unk_actor.h"
#include "game/unk_creature.h"
#include "game/unk_vec3.h"

struct UnkScriptContext {
    /* 0x00 */ u8 unk0[0x10];
    /* 0x10 */ UnkCreature* mEntity;
};

// Combat state object at lbl_805FBBE0 (real type unknown).
struct UnkCombatState {
    /* 0x000 */ u8 unk0[0x130];
    /* 0x130 */ u32 m130;
    /* 0x134 */ u32 m134;
    /* 0x138 */ int m138;
    /* 0x13C */ u8 unk13C[0xA1C - 0x13C];
    /* 0xA1C */ int mA1C;
};

// Object returned by fn_8004CED8 (real type unknown).
struct UnkShakeInput {
    /* 0x00 */ u8 unk0[0x8];
    /* 0x08 */ f32 m8;
    /* 0x0C */ u8 unkC[0x90 - 0xC];
    /* 0x90 */ u8 m90;
    /* 0x94 */ f32 m94;
};

// Object returned by Entity::vf0C (real type unknown).
struct UnkEntityC {
    /* 0x0 */ u8 unk0[0x8];
    /* 0x8 */ u32 m8;
};

// Global block at 0x803C2A20 (real type unknown).
struct UnkGlobals803C2A20 {
    /* 0x000 */ u8 unk0[0xDC4];
    /* 0xDC4 */ UnkInputSequenceSet* mDC4;
};
extern UnkGlobals803C2A20 lbl_803C2A20;

// Entity flag changes per shake key (real names unknown).
struct UnkFlagChange {
    /* 0x00 */ u32 mKey;
    /* 0x04 */ u32 mSetInfo;
    /* 0x08 */ u32 mClearInfo;
    /* 0x0C */ u32 mSet838;
    /* 0x10 */ u32 mClear838;
};

extern UnkCombatState* lbl_805FBBE0;
extern void* lbl_805FBBC8;

extern "C" {
void fn_801DC91C(const char* name, int type, void* func);
void fn_800D8854(void* obj, u32 id);
void fn_800D87EC(void* obj, u32 id, int arg2, int arg3, int arg4);
int fn_800D83A8(void* obj);
void fn_800D5D5C(void* ctrl, int arg);
void fn_800D6A18(f32 arg, void* ctrl);
u32 fn_800D5850(void* ctrl, int arg);
int fn_800D6998(void* ctrl, u32 dir);
void fn_800D675C(void* ctrl, int dir, int arg);
int fn_800D648C(void* ctrl, u32 id, int arg);
int fn_800D6534(void* ctrl, void* other, int arg);
int fn_800D56E0(void* ctrl);
void fn_800D6A00(void* ctrl);
UnkVec3* fn_800D5718(void* ctrl);
int fn_800D5DCC(void* ctrl);
void fn_800D5DA4(void* ctrl, int arg);
UnkCreature* fn_800E876C(u32 id);
u32 fn_80143460();
int fn_800E6BD4();
int fn_800E75E0();
void fn_8000C7F4(u32 hash, int arg1, int arg2);
void fn_80016CEC();
void fn_80016EA0(int arg);
void fn_80017454(const UnkScriptContext* ctx);
void fn_8004D604(UnkCreature* creature, int arg1, u32 hash, int arg3);
void fn_8005B650(UnkCreature* creature, UnkVec3* dir, int arg);
void fn_80219004(void* obj, u32 arg);
void fn_800F6B00();
void fn_800F6BBC();
void fn_8004CD28(UnkCreature* creature, int arg);
void fn_8004CE44(UnkCreature* creature);
UnkShakeInput* fn_8004CED8(UnkCreature* creature, int arg);
int fn_8004D170(UnkCreature* creature);
void fn_800E9494(UnkCreature* creature);
int fn_800E9538(UnkCreature* creature);
int fn_80050F88();
void fn_8018EEB4(u32 hash);
void fn_8018F000(u32 hash);
void fn_8018F618(u32 hash, int arg);
void fn_80190E50(u32 hash, int arg);
void fn_8018FF54(u32 hash, f32 r, f32 g, f32 b, f32 a);
void fn_80190074(u32 hash, int r, int g, int b, int a);
UnkActor* fn_80195464(u32 hash);
void fn_80192358(u32 hash, f32 value);

void fn_80014610(const UnkScriptContext* ctx);
void fn_80014734(const UnkScriptContext* ctx);
void fn_80014A38(const UnkScriptContext* ctx);
void fn_80014B54(const UnkScriptContext* ctx);
void fn_80014C24(const UnkScriptContext* ctx);
void fn_80014D5C(const UnkScriptContext* ctx);
void fn_80014E0C(const UnkScriptContext* ctx);
void fn_80014E70(const UnkScriptContext* ctx);
void fn_80014EDC(const UnkScriptContext* ctx);
void fn_80014F00(const UnkScriptContext* ctx);
void fn_80014F04(const UnkScriptContext* ctx);
void fn_80015020(const UnkScriptContext* ctx);
void fn_8001507C(const UnkScriptContext* ctx);
void fn_800150F4(const UnkScriptContext* ctx);
void fn_80015140(const UnkScriptContext* ctx);
void fn_80015190(const UnkScriptContext* ctx);
void fn_800151F8(const UnkScriptContext* ctx);
void fn_80015214(const UnkScriptContext* ctx);
void fn_80015378(const UnkScriptContext* ctx);
void fn_80015404(const UnkScriptContext* ctx);
void fn_80015638(const UnkScriptContext* ctx);
void fn_8001568C(const UnkScriptContext* ctx);
void fn_800156C8(const UnkScriptContext* ctx);
void fn_80015760(const UnkScriptContext* ctx);
void fn_80015784(const UnkScriptContext* ctx);
void fn_80015804(const UnkScriptContext* ctx);
void fn_80015878(const UnkScriptContext* ctx);
void fn_8001595C(const UnkScriptContext* ctx);
void fn_800159B4(const UnkScriptContext* ctx);
void fn_80015B70(const UnkScriptContext* ctx);
void fn_80015BA0(const UnkScriptContext* ctx);
void fn_80015BF0(const UnkScriptContext* ctx);
int fn_80015C40(int mode, f32 timer);
int fn_800160C4(const UnkScriptContext* ctx);

}

static inline void* GetD5F0() {
    return lbl_803B53A0.mD5F0;
}

// Passes a copy of the vector (the DOL copies it on the stack first).
static inline void Call8005B650(UnkCreature* creature, UnkVec3 dir, int arg) {
    fn_8005B650(creature, &dir, arg);
}

static inline void SetFlags(UnkCreature* creature, u32 flags) {
    if ((flags & 2) && creature->vf0C() != 0) {
        ((UnkEntityC*)creature->vf0C())->m8 |= 2;
    }
    creature->mInfo->mFlags |= flags;
}

static inline void ClearFlags(UnkCreature* creature, u32 flags) {
    if ((flags & 2) && creature->vf0C() != 0) {
        ((UnkEntityC*)creature->vf0C())->m8 &= ~2;
    }
    creature->mInfo->mFlags &= ~flags;
}

static inline u8 CheckShake(UnkCreature* player) {
    u8 shake = 0;
    if (fn_800E75E0()) {
        UnkCreature* current = (UnkCreature*)lbl_803B53A0.mD614;
        UnkInputSequence* seq = lbl_803C2A20.mDC4->fn_80021550(0x1C);
        seq->fn_80020070(current->m24);
        if (lbl_803C2A20.mDC4->fn_80021550(0x1C)->fn_80020068() > 0.0f) {
            shake = 1;
        } else {
            shake = 0;
        }
    } else if (fn_8004CED8(player, 0)) {
        UnkShakeInput* input = fn_8004CED8(player, 0);
        if (input->m90 && input->m94 > 0.0f) {
            input->m8 = input->m94;
        }
        shake = input->m90;
    }
    return shake;
}

extern "C" {

void fn_80014350(void) {
    fn_801DC91C("PatrolIdleEntry", 78, (void*)fn_80014610);
    fn_801DC91C("PatrolIdleExit", 78, (void*)fn_80014734);
    fn_801DC91C("PatrolIdleUpdate", 78, (void*)fn_80014A38);
    fn_801DC91C("CombatPlaceholderEntry", 78, (void*)fn_80014B54);
    fn_801DC91C("CombatPlaceholderExit", 78, (void*)fn_80014C24);
    fn_801DC91C("CombatPlaceholderUpdate", 78, (void*)fn_80014D5C);
    fn_801DC91C("EntityReverseDirection", 78, (void*)fn_80014E0C);
    fn_801DC91C("EntityWaitEntry", 78, (void*)fn_80014E70);
    fn_801DC91C("EntityWaitExit", 78, (void*)fn_80014EDC);
    fn_801DC91C("EntityWaitUpdate", 78, (void*)fn_80014F00);
    fn_801DC91C("PumpkinWaitEntry", 78, (void*)fn_800150F4);
    fn_801DC91C("PumpkinWaitUpdate", 78, (void*)fn_80015140);
    fn_801DC91C("PumpkinFreezeEntry", 78, (void*)fn_80015190);
    fn_801DC91C("PumpkinFreezeExit", 78, (void*)fn_800151F8);
    fn_801DC91C("PumpkinShakeEntry", 78, (void*)fn_80015214);
    fn_801DC91C("PumpkinShakeExit", 78, (void*)fn_80015378);
    fn_801DC91C("PumpkinShakeUpdate", 78, (void*)fn_80015404);
    fn_801DC91C("ObstacleShakeEntry", 78, (void*)fn_80015878);
    fn_801DC91C("ObstacleShakeExit", 78, (void*)fn_8001595C);
    fn_801DC91C("ObstacleShakeUpdate", 78, (void*)fn_800159B4);
    fn_801DC91C("PumpkinEscapeEntry", 78, (void*)fn_80015638);
    fn_801DC91C("PumpkinEscapeExit", 78, (void*)fn_8001568C);
    fn_801DC91C("PumpkinEscapeUpdate", 78, (void*)fn_800156C8);
    fn_801DC91C("PumpkinDeathEntry", 78, (void*)fn_80015760);
    fn_801DC91C("PumpkinDeathExit", 78, (void*)fn_80015784);
    fn_801DC91C("PumpkinDeathUpdate", 78, (void*)fn_80015804);
    fn_801DC91C("HazardDeathEntry", 78, (void*)fn_80015B70);
    fn_801DC91C("HazardDustCloudUpdate", 78, (void*)fn_80015BF0);
    fn_801DC91C("HazardPoisonGasUpdate", 78, (void*)fn_80015BA0);
    fn_801DC91C("MovingChallengeFaceCamera", 78, (void*)fn_80014F04);
    fn_801DC91C("MovingChallengeGameEntry", 78, (void*)fn_80015020);
    fn_801DC91C("MovingChallengeGameExit", 78, (void*)fn_8001507C);
    fn_801DC91C("PlayerInCombatRange", 29, (void*)fn_800160C4);
}

void fn_80014610(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0 && lbl_805FB180 != 0) {
        if ((creature->mInfo->mFlags & 0x400000) && creature->m2C == lbl_805FBBE0->m130 &&
            (lbl_805FBBE0->m138 == 0 || lbl_805FBBE0->m138 == 3)) {
            creature->mInfo->mFlags &= ~0x400000;
            creature->mInfo->mFlags |= 0x80;
            lbl_805FBBE0->m134 = creature->m2C;
            if (lbl_805FBBE0->m138 == 0) {
                fn_800D8854(GetD5F0(), creature->m2C);
            }
            creature->vf58();
        } else {
            if (creature->mInfo->mFlags & 0x400000) {
                creature->mInfo->mFlags &= ~0x400000;
            }
            creature->m844 = UnkVec3Zero();
            creature->mC40 = 0.0f;
            if (creature->mA00 != 0) {
                fn_800D5D5C(creature->mA00, 1);
            }
        }
    }
}

void fn_80014734(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0 && creature->m87C != 0 && creature->mA00 != 0) {
        fn_800D6A18(creature->m9F4, creature->mA00);
        if (creature->m880 == 3) {
            u32 dir = fn_80143460() & 3;
            u32 mask = fn_800D5850(creature->mA00, 0);
            if (mask == 0) {
                return;
            }
            int i = 0;
            while (!(mask & (1 << dir)) || fn_800D6998(creature->mA00, dir)) {
                i++;
                dir = (dir + 1) & 3;
                if (i >= 4) {
                    creature->m87C = 0;
                    return;
                }
            }
            fn_800D675C(creature->mA00, dir, 0);
        } else if ((u32)(creature->m87C - 3) <= 1) {
            int target = -1;
            if (creature->m888 != 0 && creature->m87C == 3) {
                target = fn_800D648C(creature->mA00, creature->m888, 0);
            } else {
                UnkCreature* other = (UnkCreature*)lbl_803B53A0.mD614;
                if (creature->m888 != 0 && creature->m87C == 4) {
                    other = fn_800E876C(creature->m888);
                }
                if (other != 0) {
                    target = fn_800D6534(creature->mA00, other->mA00, 0);
                }
            }
            if (target != -1) {
                fn_800D675C(creature->mA00, target, 0);
            }
        } else if (creature->m884 != 0) {
            int dir = creature->m884[creature->m88C];
            if (!creature->m894) {
                switch (dir) {
                case 0:
                    dir = 2;
                    break;
                case 1:
                    dir = 3;
                    break;
                case 2:
                    dir = 0;
                    break;
                case 3:
                    dir = 1;
                    break;
                }
            }
            if (creature->m890 <= 0) {
                creature->m87C = 0;
                return;
            }
            fn_800D675C(creature->mA00, dir, 0);
            if (creature->m894) {
                creature->m88C++;
            } else if (--creature->m88C < 0) {
                if (creature->m880 == 1) {
                    creature->m88C = 1;
                    creature->m894 = 1;
                } else if (creature->m880 == 0) {
                    creature->m88C = creature->m890 - 1;
                }
            }
            if (creature->m88C >= 0x40 || creature->m884[creature->m88C] == -1) {
                if (creature->m880 == 0) {
                    creature->m88C = 0;
                } else if (creature->m880 == 1) {
                    creature->m88C = creature->m890 - 1;
                    creature->m894 = 0;
                } else if (creature->m880 == 2) {
                    creature->m88C = creature->m890;
                    creature->m87C = 0;
                    fn_801BA190(lbl_805FB6C8, creature, 0, 0xFE0A9838, 0, 0, 0, 0);
                }
            }
        }
    }
}


void fn_80014A38(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature == 0) {
        return;
    }
    if (!creature->m895 &&
        (fn_800E6BD4() || lbl_805FBBE0->mA1C != 0 || fn_800D83A8(GetD5F0()) != 2)) {
        fn_801BA190(lbl_805FB6C8, creature, 0, 0x4F9213C1, 0, 0, 0, 0);
        return;
    }
    if (creature->m87C != 0) {
        if (creature->mA00 != 0 &&
            (!fn_800D56E0(creature->mA00) || creature->GetC40() > creature->GetC44())) {
            fn_800D5D5C(creature->mA00, 0);
            fn_801BA190(lbl_805FB6C8, creature, 0, 0x705B8B52, 0, 0, 0, 0);
        } else {
            f32 dt = creature->m24;
            creature->mC40 += dt;
        }
    }
}

void fn_80014B54(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        UnkCreature* player = (UnkCreature*)lbl_803B53A0.mD614;
        if (creature == player) {
            fn_8000C7F4(0xDDE9264F, 0, 0);
            fn_80016CEC();
        } else {
            if (player != 0) {
                fn_801BA190(lbl_805FB6C8, player, creature, 0xDDE9264F, 0, 0, 0, 0);
            }
            creature->mInfo->mFlags |= 0x400000;
            lbl_805FBBE0->m130 = creature->m2C;
        }
        if (creature->mA00 != 0) {
            fn_800D5D5C(creature->mA00, 1);
        }
        creature->mC82 = 0;
        creature->m9E4 = 0.0f;
    }
}

void fn_80014C24(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        creature->mC82 = 1;
        if (creature->mA00 != 0) {
            fn_800D5D5C(creature->mA00, 0);
            if (creature->HasType(0x4000)) {
                creature->mInfo->mFlags |= 0x80;
                creature->vf58();
            } else {
                Entity** list = lbl_803B53A0.m140;
                for (int i = 0; i < 32; i++) {
                    if (list[i] != 0 && (list[i]->mInfo->mFlags & 1) &&
                        (list[i]->mInfo->mTypeMask & 0x4000)) {
                        if (fn_800D56E0(creature->mA00)) {
                            fn_801BA190(lbl_805FB6C8, list[i], 0, 0x33EE55C6, 0, 0, 0, 0);
                        } else {
                            fn_801BA190(lbl_805FB6C8, list[i], 0, 0x00F4D7AB, 0, 0, 0, 0);
                        }
                    }
                }
            }
        }
    }
}

void fn_80014D5C(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        UnkCreature* player = (UnkCreature*)lbl_803B53A0.mD614;
        if (creature != player) {
            if (player != 0) {
                fn_801BA190(lbl_805FB6C8, player, creature, 0xDDE9264F, 0, 0, 0, 0);
            }
        } else {
            f32 dt = creature->m24;
            creature->m9E4 += dt;
            if (creature->m9E4 > 1.0f) {
                fn_8000C7F4(0xDDE9264F, 0, 0);
                creature->m9E4 = 0.0f;
            }
        }
    }
}

void fn_80014E0C(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0 && creature->mA00 != 0) {
        fn_800D6A00(creature->mA00);
        creature->mInfo->mPos = *fn_800D5718(creature->mA00);
    }
}

void fn_80014E70(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0 && creature->mA00 != 0) {
        fn_800D5D5C(creature->mA00, 1);
        if (creature->mInfo->mTypeMask & 0x2000) {
            fn_8004D604(creature, 1, 0xE3712F44, 1);
        }
    }
}

void fn_80014EDC(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0 && creature->mA00 != 0) {
        fn_800D5D5C(creature->mA00, 0);
    }
}

void fn_80014F00(const UnkScriptContext* ctx) {}

void fn_80014F04(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        UnkVec3 dir = *lbl_803C4298.mCamera->mPos - creature->mInfo->mPos;
        dir = dir.Normalized();
        Call8005B650(creature, dir, 1);
    }
}

void fn_80015020(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        lbl_805FBBE0->m130 = creature->m2C;
        fn_80219004(lbl_805FBBC8, creature->m9D8);
        fn_8000C7F4(creature->m6B4, 0, 0);
        fn_800F6B00();
    }
}

void fn_8001507C(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        creature->mInfo->mFlags |= 0x80;
        creature->vf58();
        fn_800D87EC(GetD5F0(), creature->m2C, 0, 0, 0);
    }
}

void fn_800150F4(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0 && creature->mA00 != 0) {
        fn_800D5D5C(creature->mA00, 1);
        creature->m9E4 = 0.0f;
    }
}

void fn_80015140(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        f32 dt = creature->m24;
        creature->m9E4 += dt;
        if (creature->m9E4 > 2.0f) {
            fn_801BA190(lbl_805FB6C8, creature, 0, 0x9BEE735B, 0, 0, 0, 0);
        }
    }
}

void fn_80015190(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0 && !fn_800D5DCC(creature->mA00)) {
        fn_801BA190(lbl_805FB6C8, creature, 0, 0x59CFBC0F, 0, 0, 0, 0);
    }
}

void fn_800151F8(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        fn_800D5DA4(creature->mA00, 0);
    }
}

void fn_80015214(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        UnkCreature* player = (UnkCreature*)lbl_803B53A0.mD614;
        fn_8004CD28(player, 0);
        player->mC40 = 0.0f;
        player->m9E4 = 0.0f;
        player->m6B4 = 0;
        fn_800D5D5C(creature->mA00, 1);
        Call8005B650(player, creature->mInfo->mPos, 0);
        fn_800F6B00();
        fn_80016EA0(0);
        fn_8018EEB4(0x2CFF8941);
        fn_8018EEB4(0xE5BF8E82);
        fn_8018F618(0x552FC6C7, 1);
        fn_8018F618(0xC4E94BB0, 0);
        fn_8018F618(0xB2601D05, 0);
        fn_8018F618(0x1A9EE60F, 1);
        fn_8018F618(0x8D6D65B1, 0);
        fn_80190E50(0x314B749C, 0x3D7);
        fn_8018FF54(0x314B749C, 1.0f, 1.0f, 1.0f, 1.0f);
        UnkActor* actor = fn_80195464(0xA90A6046);
        if (actor != 0) {
            actor->vf2C(0x80000);
        }
    }
}

void fn_80015378(const UnkScriptContext* ctx) {
    if (ctx->mEntity != 0) {
        fn_8004CE44((UnkCreature*)lbl_803B53A0.mD614);
        fn_8018F618(0x552FC6C7, 0);
        fn_8018F000(0xE5BF8E82);
        fn_8018F000(0x2CFF8941);
        UnkActor* actor = fn_80195464(0xA90A6046);
        if (actor != 0) {
            actor->vf30(0x80000);
        }
    }
}

void fn_80015404(const UnkScriptContext* ctx) {
    u8 shake = 0;
    UnkCreature* player;
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        player = (UnkCreature*)lbl_803B53A0.mD614;
        f32 dt = creature->m24;
        player->mC40 += dt;
        dt = creature->m24;
        player->m9E4 += dt;
        shake = CheckShake(player);
        if (player->mC40 > 5.0f) {
            if (creature->m87C == 0) {
                fn_801BA190(lbl_805FB6C8, creature, 0, 0xD7E36EF2, 0, 0, 0, 0);
            } else {
                fn_801BA190(lbl_805FB6C8, creature, 0, 0x9BEE735B, 0, 0, 0, 0);
            }
        } else if (shake) {
            player->m9E4 += 0.5f;
            if (++player->m6B4 >= 10) {
                fn_801BA190(lbl_805FB6C8, creature, 0, 0x5069BF6E, 0, 0, 0, 0);
            }
        }
        f32 count = player->m6B4;
        fn_80192358(0xA90A6046, count / 10.0f);
        if (fn_80015C40(0, player->m9E4)) {
            player->m9E4 = 0.0f;
        }
    }
}

void fn_80015638(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        creature->m9E4 = 0.0f;
        creature->mC44 = 0.0f;
        fn_800D5D5C(creature->mA00, 0);
        fn_800D6A18(creature->m9F4, creature->mA00);
    }
}

void fn_8001568C(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        fn_800F6BBC();
        creature->mC44 = 0.5f;
    }
}

void fn_800156C8(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        if (fn_800D56E0(creature->mA00)) {
            if (creature->m9E4 > 2.0f) {
                fn_801BA190(lbl_805FB6C8, creature, 0, 0x33EE55C6, 0, 0, 0, 0);
            } else {
                fn_80014734(ctx);
            }
        }
        fn_80017454(ctx);
    }
}

void fn_80015760(const UnkScriptContext* ctx) {
    if (ctx->mEntity != 0) {
        fn_800E9494((UnkCreature*)lbl_803B53A0.mD614);
    }
}

void fn_80015784(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        creature->mInfo->mFlags |= 0x80;
        creature->vf58();
    }
    fn_800F6BBC();
    fn_800D87EC(GetD5F0(), creature->m2C, 0, 0, 0);
}

}

// Probably an inline function of Entity emitted here (see Entity.h).
void Entity::vf58() {}

extern "C" {

void fn_80015804(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    UnkCreature* player = (UnkCreature*)lbl_803B53A0.mD614;
    if (creature != 0 && fn_800E9538(player)) {
        fn_801BA190(lbl_805FB6C8, creature, creature, 0xE9BAFAD5, 0, 0, 0, 0);
    }
}

void fn_80015878(const UnkScriptContext* ctx) {
    UnkCreature* player = (UnkCreature*)lbl_803B53A0.mD614;
    fn_8004CD28(player, 0);
    player->m6B0 = 0.0f;
    player->m9E4 = 0.0f;
    fn_80016EA0(0);
    fn_8018F618(0x552FC6C7, 1);
    fn_8018EEB4(0x2CFF8941);
    fn_8018EEB4(0xE5BF8E82);
    fn_8018F618(0xC4E94BB0, 0);
    fn_8018F618(0xB2601D05, 0);
    fn_8018F618(0x1A9EE60F, 1);
    fn_8018F618(0x8D6D65B1, 0);
    fn_80190E50(0x314B749C, 0x3D7);
    fn_80190074(0x314B749C, 0xFF, 0xFF, 0xFF, 0xFF);
}

void fn_8001595C(const UnkScriptContext* ctx) {
    fn_8004CE44((UnkCreature*)lbl_803B53A0.mD614);
    fn_8018F618(0x552FC6C7, 0);
    fn_8018F000(0x2CFF8941);
    fn_8018F000(0xE5BF8E82);
}

void fn_800159B4(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        u8 shake = 0;
        UnkCreature* player = (UnkCreature*)lbl_803B53A0.mD614;
        player->m6B0 -= 3.0f * player->m24;
        shake = CheckShake(player);
        if (shake) {
            player->m9E4 += 0.5f;
            player->m6B0 += 1.0f;
            if (player->m6B0 >= 10.0f) {
                fn_801BA190(lbl_805FB6C8, creature, 0, 0x0C591BF4, 0, 0, 0, 0);
            }
        }
        if (player->m6B0 < 0.0f) {
            player->m6B0 = 0.0f;
        }
        fn_80192358(0xA90A6046, player->m6B0 / 10.0f);
        if (fn_80015C40(0, player->m9E4)) {
            player->m9E4 = 0.0f;
        }
    }
}

void fn_80015B70(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        creature->mInfo->mFlags |= 0x80;
        creature->vf58();
    }
}

void fn_80015BA0(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        creature->m6B0 += creature->m24;
        if (creature->m6B0 > 10.0f) {
            fn_801BA190(lbl_805FB6C8, creature, 0, 0x874A902B, 0, 0, 0, 0);
        }
    }
}

void fn_80015BF0(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        creature->m6B0 += creature->m24;
        if (creature->m6B0 > 10.0f) {
            fn_801BA190(lbl_805FB6C8, creature, 0, 0x874A902B, 0, 0, 0, 0);
        }
    }
}

int fn_80015C40(int mode, f32 timer) {
    fn_8018F618(0x552FC6C7, 1);
    int result = 0;
    if (timer > 0.5f) {
        UnkActor* first = fn_80195464(0x1A9EE60F);
        UnkActor* second = fn_80195464(0x8D6D65B1);
        if (mode == 1) {
            first = fn_80195464(0xC4E94BB0);
            second = fn_80195464(0xB2601D05);
        }
        if (first != 0 && second != 0) {
            if (first->m64 & 2) {
                first->vf30(2);
                second->vf2C(2);
            } else {
                first->vf2C(2);
                second->vf30(2);
            }
        }
        result = 1;
    }
    return result;
}

void fn_80015D70() {
    fn_801BA190(lbl_805FB6C8, (UnkCreature*)lbl_803B53A0.mD614, 0, 0x4F9213C1, 0, 0, 0, 0);
    fn_80015878(0);
}

void fn_80015E80() {
    fn_801BA190(lbl_805FB6C8, (UnkCreature*)lbl_803B53A0.mD614, 0, 0x33EE55C6, 0, 0, 0, 0);
    fn_8001595C(0);
}

void fn_80015F0C() {
    u8 shake = 0;
    UnkCreature* player = (UnkCreature*)lbl_803B53A0.mD614;
    f32 dt = player->m24;
    player->m9E4 += dt;
    player->m6B0 -= 3.0f * player->m24;
    shake = CheckShake(player);
    if (shake) {
        player->m9E4 += 0.5f;
        player->m6B0 += 1.0f;
        if (player->m6B0 >= 6.0f) {
            fn_800D87EC(GetD5F0(), 0x0C591BF4, 0, 0, 0);
        }
    }
    if (player->m6B0 < 0.0f) {
        player->m6B0 = 0.0f;
    }
    fn_80192358(0xA90A6046, player->m6B0 / 6.0f);
    if (fn_80015C40(0, player->m9E4)) {
        player->m9E4 = 0.0f;
    }
}

int fn_800160C4(const UnkScriptContext* ctx) {
    int result = 0;
    UnkCreature* creature = ctx->mEntity;
    UnkCreature* player = (UnkCreature*)lbl_803B53A0.mD614;
    if (creature != 0 && player != 0 && !fn_8004D170(player) && creature->mBDF &&
        !fn_800E6BD4() && fn_80050F88() && fn_800D83A8(GetD5F0()) == 2) {
        u32 flags = creature->mInfo->mFlags;
        if (!(flags & 0x400080) && (flags & 1)) {
            UnkVec3 delta = ((UnkCreature*)lbl_803B53A0.mD614)->mInfo->mPos - creature->mInfo->mPos;
            if (delta.LengthSq() < 2.0f) {
                lbl_805FBBE0->m138 = -1;
                player->mC80 = 1;
                result = 1;
            }
        }
    }
    return result;
}

void fn_80016204(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0 && !creature->vf4C()) {
        creature->vf50(creature, 0, 0, 1e7f, 0);
        creature->m838 |= 0x80;
    }
}

UnkFlagChange lbl_80354B48[2] = {
    {0, 0, 0, 0, 0},
    {0xE1C807EF, 0, 0, 0, 0},
};

void fn_80016284(UnkCreature* creature, u32 key) {
    for (int i = 0; i < 2; i++) {
        if (key == lbl_80354B48[i].mKey) {
            creature->m158 = i;
            creature->m838 |= lbl_80354B48[i].mSet838;
            creature->m838 &= ~lbl_80354B48[i].mClear838;
            SetFlags(creature, lbl_80354B48[i].mSetInfo);
            ClearFlags(creature, lbl_80354B48[i].mClearInfo);
            return;
        }
    }
    creature->m158 = 0;
}

}
