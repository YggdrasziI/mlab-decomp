// Game unit at 0x80016400-0x80018C04 (real file name unknown): character state
// callbacks ("character_combat_idle_entry", "character_exploration_move_update",
// "character_race_update", "character_challenge_entry", "CharacterCombatActionExit"...)
// and the target node predicates ("target_node_selected", "target_node_changed",
// "target_node_reached"), registered through fn_801DC91C. Bounds come from the
// static initializers of this unit and the previous one.
// NonMatching: fn_800182A4 (99.8 %, float registers of the direction tests).

#include "game/ActionSelectionCallback.h"
#include "game/externs.h"
#include "game/unk_803B53A0.h"
#include "game/unk_803C4298.h"
#include "game/unk_creature.h"
#include "game/unk_vec3.h"
#include <string.h>

UnkVec3 fn_801452A8(const UnkQuat* rot);

struct UnkScriptContext {
    /* 0x00 */ u8 unk0[0x10];
    /* 0x10 */ UnkCreature* mEntity;
};

// Combat state object at lbl_805FBBE0 (real type unknown).
struct UnkCombatState {
    /* 0x000 */ u8 unk0[0x100];
    /* 0x100 */ int m100;
    /* 0x104 */ int m104;
    /* 0x108 */ u8 unk108[0x130 - 0x108];
    /* 0x130 */ u32 m130;
    /* 0x134 */ u8 unk134[0xA1C - 0x134];
    /* 0xA1C */ int mA1C;
};

// Object at lbl_805FBBC8 (real type unknown).
struct UnkChallengeState {
    /* 0x00 */ u8 unk0[0x6C];
    /* 0x6C */ u8 m6C;
};

// Object at lbl_803B53A0.mD634 (real type unknown).
struct UnkConversationState {
    /* 0x000 */ u8 unk0[0x430];
    /* 0x430 */ u8 m430;
    /* 0x431 */ u8 unk431[0x434 - 0x431];
    /* 0x434 */ void* m434;
};

// Stick input object at lbl_803C4298 + 0x20 (see unit_80122B5C.cpp).
class UnkStickInput {
public:
    void fn_80122BD8();
    void fn_80122E2C(UnkVec3* dir, f32* length);
};

// Combat side object; the combatant holding it is 0xC48 bytes before it (real types
// unknown).
struct UnkCombatSide {
    /* 0x0 */ u32 m0;
};
struct UnkCombatantBase {
    /* 0x000 */ u8 unk0[0xC48];
};
class UnkCombatant : public UnkCombatantBase, public UnkCombatSide {};

struct UnkCombatSlot {
    /* 0x00 */ u8 unk0[0x1C];
    /* 0x1C */ UnkCombatSide* m1C;
};

struct UnkCombatSlots {
    /* 0x000 */ u8 unk0[0xA00];
    /* 0xA00 */ UnkCombatSlot* mA00;
    /* 0xA04 */ UnkCombatSlot* mA04;
};

struct UnkEntityA4 {
    /* 0x00 */ u8 unk0[0x2C];
    /* 0x2C */ int m2C;
};

extern UnkCombatState* lbl_805FBBE0;
extern UnkChallengeState* lbl_805FBBC8;
extern UnkCombatSlots* lbl_805FBBB8;
extern void* lbl_805FB654;
extern u8 lbl_805FB0E4;
extern u8 lbl_805FB1A4[2];

extern "C" {
void fn_801DC91C(const char* name, int type, void* func);
void fn_800D87EC(void* obj, u32 id, u32 arg2, int arg3, int arg4);
int fn_800D83A8(void* obj);
void fn_800D5D5C(void* ctrl, int arg);
void fn_800D6A18(f32 arg, void* ctrl);
u32 fn_800D5850(void* ctrl, int arg);
void fn_800D675C(void* ctrl, int dir, int arg);
int fn_800D56E0(void* ctrl);
UnkVec3* fn_800D5718(void* ctrl);
UnkVec3 fn_800D5724(void* ctrl);
u32 fn_800D56F8(void* ctrl);
int fn_800D5924(void* ctrl);
u32 fn_800D5944(void* ctrl);
int fn_800D5984(void* ctrl);
void* fn_800D5964(void* ctrl);
u32 fn_800D581C(void* ctrl);
int fn_800E6BD4();
void fn_8000C7F4(u32 hash, int arg1, int arg2);
void fn_8004D604(UnkCreature* creature, int arg1, u32 hash, int arg3);
void fn_8005B650(UnkCreature* creature, UnkVec3* dir, int arg);
void fn_800F6B00();
void fn_8004CD28(UnkCreature* creature, int arg);
void fn_8004CE44(UnkCreature* creature);
int fn_8004D170(UnkCreature* creature);
void fn_8004D248(UnkCreature* creature, f32 arg);
void fn_8004D2B8(UnkCreature* creature, int arg);
int fn_8004D520(UnkCreature* creature, int arg);
int fn_8004D5BC(UnkCreature* creature);
void fn_800534C0(UnkCreature* creature);
void fn_8018EEB4(u32 hash);
void fn_8018F000(u32 hash);
void fn_8018F618(u32 hash, int arg);
void fn_80190E50(u32 hash, int arg);
void fn_801903CC(u32 hash, u32 arg);
int fn_8019080C(u32 hash);
void fn_801952AC(u32 hash, u32 arg);
void fn_80192358(u32 hash, f32 value);
void* fn_8018D1E4(void* obj);
void* fn_80198E34(void* obj, u32 hash);
int fn_80198C34(void* obj, void* node);
void* fn_80199AEC(void* node, u32 hash);
const char* fn_8019F3AC(void* obj);
void fn_80045138(void* obj, void* arg1, int arg2);
void fn_80044F20();
u32 fn_801B9B40(int channel);
u32 fn_801B9B98(int channel);
void fn_80147704(int channel, f32* x, f32* y);
void fn_801243B8(UnkMtx34* mtx, UnkVec3* in, UnkVec3* out);
void fn_80012820(void* obj, u32 arg, f32 value);
void fn_801BB5BC(void* obj, f32 dt);
u32 fn_801EB704(const char* text);
void fn_8005100C(void* conversation);
void fn_8001B0B8(void* obj, void* conversation);
void fn_800F5294(void* obj);
int fn_80015C40(int mode, f32 timer);

int fn_800182A4(const UnkScriptContext* ctx);
int fn_8001879C(const UnkScriptContext* ctx);
int fn_800186D8(const UnkScriptContext* ctx);
int fn_80018BC8(const UnkScriptContext* ctx);
void fn_80016788(const UnkScriptContext* ctx);
void fn_800168D8(const UnkScriptContext* ctx);
void fn_800168DC(const UnkScriptContext* ctx);
void fn_80016960(const UnkScriptContext* ctx);
void fn_800169A0(const UnkScriptContext* ctx);
void fn_800169A4(const UnkScriptContext* ctx);
void fn_80017290(const UnkScriptContext* ctx);
void fn_80017308(const UnkScriptContext* ctx);
void fn_80017454(const UnkScriptContext* ctx);
void fn_80017930(const UnkScriptContext* ctx);
void fn_80017A44(const UnkScriptContext* ctx);
void fn_80017AE0(const UnkScriptContext* ctx);
void fn_80017D28(const UnkScriptContext* ctx);
void fn_80017E5C(const UnkScriptContext* ctx);
void fn_80017F14(const UnkScriptContext* ctx);
void fn_8001786C(const UnkScriptContext* ctx);
void fn_80017870(const UnkScriptContext* ctx);
void fn_80017874(const UnkScriptContext* ctx);
void fn_80017F18(const UnkScriptContext* ctx);
void fn_8001802C(const UnkScriptContext* ctx);
void fn_80018040(const UnkScriptContext* ctx);
void fn_80018044(const UnkScriptContext* ctx);
void fn_80018174(const UnkScriptContext* ctx);
void fn_800181E0(const UnkScriptContext* ctx);
void fn_80017704(const UnkScriptContext* ctx);
void fn_80017770(const UnkScriptContext* ctx);
void fn_800177F4(const UnkScriptContext* ctx);
void fn_80017818(const UnkScriptContext* ctx);
void fn_80017620(const UnkScriptContext* ctx);
void fn_80018A34(const UnkScriptContext* ctx);
void fn_80018A4C(const UnkScriptContext* ctx);
void fn_80018A50(const UnkScriptContext* ctx);
void fn_80018A54(const UnkScriptContext* ctx);
void fn_80018A58(const UnkScriptContext* ctx);
void fn_80018AC4(const UnkScriptContext* ctx);
void fn_80018B44(const UnkScriptContext* ctx);
void fn_80018BAC(const UnkScriptContext* ctx);
void fn_80018BC4(const UnkScriptContext* ctx);
void fn_80018BE0(const UnkScriptContext* ctx);
void fn_80016FD0(UnkCreature* creature, int show);
void fn_80016EA0(int flag);
}

static inline void* GetD5F0() {
    return lbl_803B53A0.mD5F0;
}

static inline UnkStickInput* GetStickInput() {
    return (UnkStickInput*)lbl_803C4298.mStick;
}

static inline UnkConversationState* GetConversationState() {
    return *(UnkConversationState**)((u8*)&lbl_803B53A0 + 0xD634);
}

static inline void Scale(UnkVec3& v, f32 s) {
    f32 f = s;
    v.x *= f;
    v.y *= f;
    v.z *= f;
}

static inline bool IsNearZero(const UnkVec3& v) {
    return v.x <= 0.0001f && v.x >= -0.0001f && v.y <= 0.0001f && v.y >= -0.0001f &&
           v.z <= 0.0001f && v.z >= -0.0001f;
}

static inline void UpdateNodeEffect(UnkCreature* creature) {
    void* resource = fn_8018D1E4(lbl_805FB654);
    if (resource != 0) {
        void* node = fn_80198E34(resource, 0xDB1E4D0C);
        if (node != 0 && fn_80198C34(resource, node)) {
            if (creature->m9F8) {
                fn_80016FD0(creature, 1);
                creature->m9F8 = 0;
            }
            if (creature == (UnkCreature*)lbl_803B53A0.mD614 && !fn_8019080C(0x664BFF35)) {
                fn_80045138(resource, creature->vfF0(), 0);
            }
        }
    }
}

extern "C" {

void fn_80016400(void) {
    fn_801DC91C("target_node_selected", 29, (void*)fn_800182A4);
    fn_801DC91C("target_node_changed", 29, (void*)fn_8001879C);
    fn_801DC91C("target_node_reached", 29, (void*)fn_800186D8);
    fn_801DC91C("CharacterPreviewerActionDone", 29, (void*)fn_80018BC8);
    fn_801DC91C("character_combat_idle_entry", 78, (void*)fn_80016788);
    fn_801DC91C("character_combat_idle_exit", 78, (void*)fn_800168D8);
    fn_801DC91C("character_combat_idle_update", 78, (void*)fn_800168DC);
    fn_801DC91C("character_exploration_idle_entry", 78, (void*)fn_80016960);
    fn_801DC91C("character_exploration_idle_exit", 78, (void*)fn_800169A0);
    fn_801DC91C("character_exploration_idle_update", 78, (void*)fn_800169A4);
    fn_801DC91C("character_exploration_move_entry", 78, (void*)fn_80017290);
    fn_801DC91C("character_exploration_move_exit", 78, (void*)fn_80017308);
    fn_801DC91C("character_exploration_move_update", 78, (void*)fn_80017454);
    fn_801DC91C("character_race_entry", 78, (void*)fn_80017930);
    fn_801DC91C("character_race_exit", 78, (void*)fn_80017A44);
    fn_801DC91C("character_race_update", 78, (void*)fn_80017AE0);
    fn_801DC91C("character_challenge_entry", 78, (void*)fn_80017D28);
    fn_801DC91C("character_challenge_exit", 78, (void*)fn_80017E5C);
    fn_801DC91C("character_challenge_update", 78, (void*)fn_80017F14);
    fn_801DC91C("character_challenge_instruction_entry", 78, (void*)fn_8001786C);
    fn_801DC91C("character_challenge_instruction_exit", 78, (void*)fn_80017870);
    fn_801DC91C("character_challenge_instruction_update", 78, (void*)fn_80017874);
    fn_801DC91C("character_conversation_entry", 78, (void*)fn_80017F18);
    fn_801DC91C("character_conversation_exit", 78, (void*)fn_8001802C);
    fn_801DC91C("character_conversation_update", 78, (void*)fn_80018040);
    fn_801DC91C("character_Interaction_entry", 78, (void*)fn_80018044);
    fn_801DC91C("character_Interaction_exit", 78, (void*)fn_80018174);
    fn_801DC91C("character_Interaction_update", 78, (void*)fn_800181E0);
    fn_801DC91C("character_exploration_wait_entry", 78, (void*)fn_80017704);
    fn_801DC91C("character_exploration_wait_exit", 78, (void*)fn_80017770);
    fn_801DC91C("character_exploration_injure_entry", 78, (void*)fn_800177F4);
    fn_801DC91C("character_exploration_injure_exit", 78, (void*)fn_80017818);
    fn_801DC91C("character_exploration_exit_entry", 78, (void*)fn_80017620);
    fn_801DC91C("CharacterCombatIdleEntry", 78, (void*)fn_80018A34);
    fn_801DC91C("CharacterCombatIdleExit", 78, (void*)fn_80018A4C);
    fn_801DC91C("CharacterCombatIdleUpdate", 78, (void*)fn_80018A50);
    fn_801DC91C("CharacterCombatWaitForActionUpdate", 78, (void*)fn_80018A54);
    fn_801DC91C("CharacterCombatActionEntry", 78, (void*)fn_80018A58);
    fn_801DC91C("CharacterCombatActionExit", 78, (void*)fn_80018AC4);
    fn_801DC91C("CharacterCombatActionUpdate", 78, (void*)fn_80018B44);
    fn_801DC91C("CharacterCombatReactionEntry", 78, (void*)fn_80018BAC);
    fn_801DC91C("CharacterCombatReactionUpdate", 78, (void*)fn_80018BC4);
    fn_801DC91C("CharacterPreviewerIdleEntry", 78, (void*)fn_80018BE0);
}

void fn_80016788(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        if (fn_800D83A8(GetD5F0()) == 2 || fn_800D83A8(GetD5F0()) == 4) {
            if (creature->m2C != 0xA9638202) {
                fn_801BA190(lbl_805FB6C8, creature, 0, 0x58DED561, 0, 0, 0, 0);
                if (creature == (UnkCreature*)lbl_803B53A0.mD614 && !fn_800E6BD4()) {
                    fn_80016FD0(creature, 1);
                }
            }
        } else if (fn_800D83A8(GetD5F0()) == 6) {
            if (lbl_803C4298.m1C != 0x1C) {
                fn_801BA190(lbl_805FB6C8, creature, 0, 0x5ED5DBDF, 0, 0, 0, 0);
            } else {
                fn_801BA190(lbl_805FB6C8, creature, 0, 0xAD86D6D5, 0, 0, 0, 0);
            }
        }
    }
}

void fn_800168D8(const UnkScriptContext* ctx) {}

void fn_800168DC(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0 && fn_800D83A8(GetD5F0()) == 2 && creature->m2C != 0xA9638202) {
        fn_801BA190(lbl_805FB6C8, creature, 0, 0x58DED561, 0, 0, 0, 0);
    }
}

void fn_80016960(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        creature->m9E4 = 0.0f;
        creature->m844 = UnkVec3Zero();
        creature->mC80 = 0;
    }
}

void fn_800169A0(const UnkScriptContext* ctx) {}

void fn_800169A4(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature == 0) {
        return;
    }
    UpdateNodeEffect(creature);
    if (fn_800E6BD4() || lbl_805FBBE0->mA1C != 0 || fn_800D83A8(GetD5F0()) == 4) {
        fn_800F6B00();
        return;
    }
    if (creature->mA00 != 0) {
        creature->mInfo->mPos = *fn_800D5718(creature->mA00);
        if (!fn_800D56E0(creature->mA00)) {
            UnkVec3 dir = fn_800D5724(creature->mA00);
            fn_8005B650(creature, &dir, 0);
            fn_800D5D5C(creature->mA00, 0);
            fn_801BA190(lbl_805FB6C8, creature, 0, 0x705B8B52, 0, 0, 0, 0);
            return;
        }
    }
    if ((fn_801B9B40((int)lbl_803C4298.m38) & 0x40) && !lbl_8047C7D8.m0 && !fn_800E6BD4() &&
        lbl_805FB0E4 && !fn_8004D170(creature) && creature->mA00 != 0) {
        u32 id = fn_800D56F8(creature->mA00);
        lbl_805FBBE0->m100 = 0;
        fn_800D87EC(GetD5F0(), 0xAE2DA05D, id, 0, 1);
        if (lbl_805FBBE0->m100 == 0 && fn_800D5924(creature->mA00)) {
            switch (fn_800D5944(creature->mA00)) {
            case 8:
                fn_801BA190(lbl_805FB6C8, creature, 0, 0xD0354B85, 0, 0, 0, 0);
                break;
            case 2:
                fn_801BA190(lbl_805FB6C8, creature, 0, 0x74027504, 0, 0, 0, 0);
                break;
            case 4:
                fn_801BA190(lbl_805FB6C8, creature, 0, 0x8AD54581, 0, 0, 0, 0);
                break;
            case 16:
                fn_801BA190(lbl_805FB6C8, creature, 0, 0x06B0A31C, 0, 0, 0, 0);
                lbl_805FBBE0->m104 = 9;
                break;
            }
        }
    }
}

void fn_80016CEC() {
    Entity** list = lbl_803B53A0.m140;
    for (int i = 0; i < 32; i++) {
        Entity* entity = list[i];
        if (entity != 0 && (entity->mInfo->mFlags & 1) && (entity->mInfo->mTypeMask & 0x10) &&
            entity != lbl_803B53A0.mD614 && ((UnkCreature*)entity)->mA00 != 0) {
            fn_801BA190(lbl_805FB6C8, entity, 0, 0x4F9213C1, 0, 0, 0, 0);
        }
    }
}

void fn_80016DB0() {
    Entity** list = lbl_803B53A0.m140;
    for (int i = 0; i < 32; i++) {
        if (list[i] != 0 && (list[i]->mInfo->mFlags & 1) && (list[i]->mInfo->mTypeMask & 0x10) &&
            list[i] != lbl_803B53A0.mD614) {
            UnkCreature* creature = (UnkCreature*)list[i];
            if (creature->mA00 != 0) {
                if (fn_800D56E0(creature->mA00)) {
                    fn_801BA190(lbl_805FB6C8, list[i], 0, 0x33EE55C6, 0, 0, 0, 0);
                } else {
                    fn_801BA190(lbl_805FB6C8, list[i], 0, 0x00F4D7AB, 0, 0, 0, 0);
                }
            }
        }
    }
}

void fn_80016EA0(int flag) {
    void* resource = fn_8018D1E4(lbl_805FB654);
    if (resource != 0) {
        void* node = fn_80198E34(resource, 0xDB1E4D0C);
        if (node != 0 && flag) {
            fn_801903CC(0x664BFF35, 0x784097C0);
        }
        if (fn_80198C34(resource, node)) {
            void* first = fn_80199AEC(node, 0x640078E4);
            void* second = fn_80199AEC(node, 0x9E8A6F47);
            if ((first != 0 && strcmp(fn_8019F3AC(first), "") != 0) ||
                (second != 0 && strcmp(fn_8019F3AC(second), "") != 0)) {
                fn_801952AC(0x798F99BC, 0xDB1E4D0C);
                fn_80190E50(0x640078E4, 0x557);
                fn_80190E50(0x9E8A6F47, 0x557);
            }
        }
    }
}

void fn_80016FD0(UnkCreature* creature, int show) {
    if (creature == 0 || fn_800E6BD4()) {
        return;
    }
    void* resource = fn_8018D1E4(lbl_805FB654);
    if (resource != 0) {
        void* node = fn_80198E34(resource, 0xDB1E4D0C);
        if (node != 0 && fn_80198C34(resource, node)) {
            if (show) {
                fn_80044F20();
                fn_801903CC(0x664BFF35, 0x4A4CBE89);
            }
        } else {
            creature->m9F8 = 1;
            return;
        }
    }
    if (creature->mA00 != 0) {
        int first = 0x557;
        int second = 0x557;
        int play = 0;
        switch (fn_800D5944(creature->mA00)) {
        case 8:
            first = 0x3CC;
            play = 1;
            break;
        case 2:
            second = 0x567;
            switch (fn_800D5984(creature->mA00)) {
            case 0xA41D84B9:
                first = 0x3CB;
                break;
            case 0xC4CE4FC3:
                first = 0x3C1;
                break;
            case 0x711A757D:
                first = 0x3C5;
                break;
            case 0x0144CE0D:
                first = 0x3D1;
                break;
            case 0x1EF3AA63:
                first = 0x3C3;
                break;
            case 0xF3CC643A:
                first = 0x3CD;
                break;
            case 0x01C00E16:
                first = 0x3DC;
                break;
            case 0x4B9FE6EB:
                first = 0x3C2;
                break;
            case 0xBAC3287F:
                first = 0x3D6;
                break;
            case 0x5D664B82:
                first = 0x3DA;
                break;
            }
            play = 1;
            break;
        case 4:
            first = 0x3D8;
            play = 1;
            break;
        case 16:
            first = 0x3D3;
            play = 1;
            break;
        }
        if (!fn_800D5924(creature->mA00)) {
            play = 0;
        }
        if (fn_800D581C(creature->mA00)) {
            first = fn_800D581C(creature->mA00);
            second = 0x557;
            play = 1;
        }
        if (play) {
            fn_801952AC(0xFE181559, 0xDB1E4D0C);
            fn_80190E50(0x640078E4, first);
            fn_80190E50(0x9E8A6F47, second);
        }
    }
}

void fn_80017290(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        if (creature->mA00 != 0) {
            fn_800D6A18(creature->m9F4, creature->mA00);
            fn_800D5D5C(creature->mA00, 0);
        }
        if (creature == (UnkCreature*)lbl_803B53A0.mD614) {
            fn_80016EA0(0);
            creature->mC80 = 0;
        }
    }
}

void fn_80017308(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0 && creature->mA00 != 0 && fn_800D56E0(creature->mA00)) {
        if (creature == (UnkCreature*)lbl_803B53A0.mD614) {
            if (fn_800D5924(creature->mA00)) {
                fn_80016FD0(creature, 0);
            }
            lbl_805FBBE0->m100 = 0;
            fn_800D87EC(GetD5F0(), 0x6E9F46BC, creature->m2C, fn_800D56F8(creature->mA00), 1);
            fn_801BB5BC(GetConversationState()->m434, creature->m24);
        } else {
            fn_800D87EC(GetD5F0(), 0x00C72F9E, creature->m2C, fn_800D56F8(creature->mA00), 1);
        }
        creature->mInfo->mPos = *fn_800D5718(creature->mA00);
        if (creature->HasType(0x2000) && creature != 0 && creature->mC54 != 0) {
            fn_800F5294(creature->mC54);
        }
    }
}

void fn_80017454(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        UpdateNodeEffect(creature);
        f32 dt = creature->m24;
        creature->m9E4 += dt;
        if (creature->mA00 != 0) {
            UnkVec3 dir = fn_800D5724(creature->mA00);
            dir.z = 0.0f;
            if (!fn_800D56E0(creature->mA00) && dir.LengthSq() > 0.01f) {
                creature->mInfo->mPos = *fn_800D5718(creature->mA00);
                UnkVec3 face = fn_800D5724(creature->mA00);
                fn_8005B650(creature, &face, 0);
                creature->mInfo->mFlags |= 0x8000;
                creature->mInfo->mFlags |= 0x10000000;
                creature->mInfo->mFlags |= 0x4000;
                creature->mInfo->mFlags |= 0x10000000;
            }
        }
    }
}

void fn_80017620(const UnkScriptContext* ctx) {
    if (ctx->mEntity != 0) {
        fn_8000C7F4(0x06B0A31C, 0, 0);
        fn_80016CEC();
    }
}

void fn_80017704(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        if (creature->mA00 != 0) {
            fn_800D5D5C(creature->mA00, 1);
        }
        if (creature->HasType(0x2000)) {
            fn_8004D604(creature, 1, 0xE3712F44, 1);
        }
    }
}

void fn_80017770(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        if (creature == (UnkCreature*)lbl_803B53A0.mD614 && !fn_800E6BD4()) {
            fn_80016FD0(creature, 1);
        }
        if (creature->mA00 != 0 && fn_800D56E0(creature->mA00)) {
            fn_800D5D5C(creature->mA00, 0);
        }
    }
}

void fn_800177F4(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0 && creature->mA00 != 0) {
        fn_800D5D5C(creature->mA00, 1);
    }
}

void fn_80017818(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0 && creature->mA00 != 0 && fn_800D56E0(creature->mA00)) {
        fn_800D5D5C(creature->mA00, 0);
    }
}

void fn_8001786C(const UnkScriptContext* ctx) {}

void fn_80017870(const UnkScriptContext* ctx) {}

void fn_80017874(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    u32 buttons = fn_801B9B40((int)lbl_803C4298.m38);
    if (buttons & 0x40) {
        fn_80012820((u8*)&lbl_803B53A0 + 0x5C8, fn_800D56F8(creature->mA00), 65.0f);
        fn_801BA190(lbl_805FB6C8, creature, 0, 0x74027504, 0, 0, 0, 0);
    } else if (buttons & 0x80) {
        fn_801BA190(lbl_805FB6C8, creature, 0, 0x33EE55C6, 0, 0, 0, 0);
    }
}

void fn_80017930(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        f32 speed = 0.0f;
        fn_800534C0(creature);
        creature->m9F4 = speed;
        ((UnkEntityA4*)creature->mA4[0])->m2C = 0;
        creature->m9E4 = 0.0f;
        fn_8004CD28(creature, 0);
        creature->mC81 = 1;
        fn_800D6A18(speed, creature->mA00);
        fn_80016EA0(0);
        if (creature->m9F4 > 0.0f) {
            fn_8004D604(creature, 1, 0xF5770040, 1);
        }
        fn_8018EEB4(0xE5BF8E82);
        fn_8018F618(0x552FC6C7, 1);
        fn_8018F618(0xC4E94BB0, 0);
        fn_8018F618(0xB2601D05, 0);
        fn_8018F618(0x1A9EE60F, 1);
        fn_8018F618(0x8D6D65B1, 0);
    }
}

void fn_80017A44(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        f32 speed = 6.0f;
        creature->m9F4 = speed;
        fn_8004CE44(creature);
        creature->mC81 = 0;
        fn_8004D248(creature, 1.0f);
        fn_800D6A18(speed, creature->mA00);
        fn_80192358(0xA90A6046, 0.0f);
        fn_8018F618(0x552FC6C7, 0);
        fn_8018F000(0xE5BF8E82);
    }
}

void fn_80017AE0(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        void* resource = fn_8018D1E4(lbl_805FB654);
        fn_80045138(resource, creature->vfF0(), 0);
        f32 dt = creature->m24;
        creature->m9E4 += dt;
        if (fn_800D56E0(creature->mA00)) {
            fn_800D87EC(GetD5F0(), 0x6E9F46BC, 0, 0, 1);
            if (creature->m890 <= 0 || creature->m884[creature->m88C] == -1) {
                if (creature->m9F4 > 0.0f) {
                    fn_8004D604(creature, 1, 0xE3712F44, 1);
                    creature->m9F4 = 0.0f;
                    creature->mInfo->mPos = *fn_800D5718(creature->mA00);
                    fn_8004D248(creature, 1.0f);
                }
                return;
            }
            if (creature->m884 != 0) {
                fn_800D675C(creature->mA00, creature->m884[creature->m88C], 0);
                creature->m88C++;
            }
        }
        fn_8018F618(0x552FC6C7, 1);
        fn_80192358(0xA90A6046, creature->m9F4 / 11.0f);
        if (fn_80015C40(0, creature->m9E4) && fn_80015C40(1, creature->m9E4)) {
            creature->m9E4 = 0.0f;
        }
        UnkVec3 dir = fn_800D5724(creature->mA00);
        dir.z = 0.0f;
        if (creature->mA00 != 0 && !fn_800D56E0(creature->mA00) && dir.LengthSq() > 0.01f) {
            creature->mInfo->mPos = *fn_800D5718(creature->mA00);
            UnkVec3 face = fn_800D5724(creature->mA00);
            fn_8005B650(creature, &face, 0);
        }
    }
}

void fn_80017D28(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        u32 hash = fn_800D5984(creature->mA00);
        if (hash == fn_801EB704("1")) {
            fn_8000C7F4(0xBA5FF08B, 0, 0);
        } else if (hash != 0) {
            fn_8000C7F4(hash, 0, 0);
        }
        lbl_805FBBE0->m130 = fn_800D56F8(creature->mA00);
        fn_80016EA0(0);
        fn_80016CEC();
    }
}

void fn_80017E5C(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        if (lbl_805FBBC8->m6C) {
            fn_800D87EC(GetD5F0(), 0xACA6B62C, 0, 0, 0);
        } else {
            fn_800D87EC(GetD5F0(), 0xADCA036F, 0, 0, 0);
        }
        fn_80016DB0();
        fn_8000C7F4(0xACE44170, 0, 0);
        fn_80016FD0(creature, 1);
    }
}

void fn_80017F14(const UnkScriptContext* ctx) {}

void fn_80017F18(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        void* conversation = fn_800D5964(creature->mA00);
        if (conversation != 0) {
            fn_8005100C(conversation);
        } else {
            fn_801BA190(lbl_805FB6C8, creature, creature, 0x6877205C, 0, 0, 0, 0);
            return;
        }
        fn_80016CEC();
    }
}

void fn_8001802C(const UnkScriptContext* ctx) {
    if (ctx->mEntity != 0) {
        fn_80016DB0();
    }
}

void fn_80018040(const UnkScriptContext* ctx) {}

void fn_80018044(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        void* conversation = fn_800D5964(creature->mA00);
        if (conversation != 0) {
            creature->m6B4 = 1;
            fn_8001B0B8(GetConversationState(), conversation);
        } else {
            creature->m6B4 = 0;
            fn_801BA190(lbl_805FB6C8, creature, creature, 0x2524AD6D, 0, 0, 0, 0);
            return;
        }
        fn_80016CEC();
    }
}

void fn_80018174(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        fn_80016DB0();
        fn_80016FD0(creature, 0);
        if (creature->m6B4 != 0) {
            GetConversationState()->m430 = 1;
            creature->m6B4 = 0;
        }
    }
}

void fn_800181E0(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0 && creature->mA00 != 0 && !fn_800D56E0(creature->mA00)) {
        creature->mInfo->mPos = *fn_800D5718(creature->mA00);
        UnkVec3 dir = fn_800D5724(creature->mA00);
        dir.z = 0.0f;
        if (dir.LengthSq() > 0.01f) {
            UnkVec3 face = fn_800D5724(creature->mA00);
            fn_8005B650(creature, &face, 0);
        }
    }
}

int fn_800182A4(const UnkScriptContext* ctx) {
    u32 buttons = fn_801B9B98((int)lbl_803C4298.m38);
    int result = 0;
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0 && !lbl_8047C7D8.m0 && !fn_800E6BD4() && lbl_805FBBE0->mA1C == 0 &&
        !fn_8004D170(creature) && fn_800D83A8(GetD5F0()) == 2) {
        UnkMtx34 mtx;
        lbl_803C4298.mCamera->vf38(&mtx);
        UnkVec3 stick = UnkVec3Zero();
        f32 x = 0.0f;
        f32 y = 0.0f;
        if (!lbl_805FB1A4[1]) {
            fn_80147704((int)lbl_803C4298.m38 * 16 + 14, &x, &y);
            stick.y = y;
            stick.x = x;
            if (stick.x > -0.1f && stick.x < 0.1f) {
                stick.x = 0.0f;
            }
            if (stick.y > -0.1f && stick.y < 0.1f) {
                stick.y = 0.0f;
            }
        }
        u32 mask = 0xFFFFFFFF;
        if (creature->mA00 != 0) {
            mask = fn_800D5850(creature->mA00, 0);
        }
        if ((buttons & 0xF) || !IsNearZero(stick)) {
            result = 1;
            UnkVec3 pad = UnkVec3Zero();
            if (buttons & 2) {
                pad.y = -1.0f;
            } else if (buttons & 1) {
                pad.y = 1.0f;
            }
            if (buttons & 4) {
                pad.x = -1.0f;
            } else if (buttons & 8) {
                pad.x = 1.0f;
            }
            if (creature->m9D4) {
                Scale(pad, -1.0f);
                Scale(stick, -1.0f);
            }
            fn_801243B8(&mtx, &pad, &pad);
            pad.z = 0.0f;
            fn_801243B8(&mtx, &stick, &stick);
            stick.z = 0.0f;
            if ((pad.y < -0.5f && pad.y < pad.x) || (stick.y < -0.5f && stick.y < stick.x)) {
                if (creature->mA00 != 0 && (mask & 4)) {
                    fn_800D675C(creature->mA00, 2, 0);
                }
            } else if ((pad.y > 0.5f && pad.y > pad.x) || (stick.y > 0.5f && stick.y > stick.x)) {
                if (creature->mA00 != 0 && (mask & 1)) {
                    fn_800D675C(creature->mA00, 0, 0);
                }
            } else if (pad.x < -0.5f || stick.x < -0.5f) {
                if (creature->mA00 != 0 && (mask & 8)) {
                    fn_800D675C(creature->mA00, 3, 0);
                }
            } else if (pad.x > 0.5f || stick.x > 0.5f) {
                if (creature->mA00 != 0 && (mask & 2)) {
                    fn_800D675C(creature->mA00, 1, 0);
                }
            }
            if (creature->mA00 == 0) {
                result = 0;
            } else if (creature->mA00 != 0 && fn_800D56E0(creature->mA00)) {
                result = 0;
            }
        }
    }
    return result;
}

int fn_800186D8(const UnkScriptContext* ctx) {
    int result = 0;
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        UnkVec3 delta = UnkVec3(creature->m9E8) - creature->mInfo->mPos;
        if (creature->mA00 != 0) {
            result = fn_800D56E0(creature->mA00);
        }
        if (lbl_805FB180 == 0 && delta.LengthSq() <= 0.01f) {
            result = 1;
        }
    }
    return result;
}

int fn_8001879C(const UnkScriptContext* ctx) {
    u32 buttons = fn_801B9B98((int)lbl_803C4298.m38);
    int result = 0;
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0 && creature->m9E4 > 0.5f) {
        UnkVec3 forward = fn_801452A8(&creature->mInfo->mRot);
        UnkMtx34 mtx;
        lbl_803C4298.mCamera->vf38(&mtx);
        UnkVec3 stick = UnkVec3Zero();
        f32 length = 0.8f;
        if (!lbl_805FB1A4[1]) {
            GetStickInput()->fn_80122BD8();
            GetStickInput()->fn_80122E2C(&stick, &length);
        }
        if ((buttons & 0xF) || !IsNearZero(stick)) {
            UnkVec3 pad = UnkVec3Zero();
            f32 step = 3.0f;
            if (buttons & 2) {
                pad.y = -step;
            } else if (buttons & 1) {
                pad.y = step;
            }
            if (buttons & 4) {
                pad.x = -step;
            } else if (buttons & 8) {
                pad.x = step;
            }
            if (creature->m9D4) {
                Scale(pad, -1.0f);
                Scale(stick, -1.0f);
            }
            fn_801243B8(&mtx, &pad, &pad);
            pad.z = 0.0f;
            fn_801243B8(&mtx, &stick, &stick);
            stick.z = 0.0f;
            if (forward.x * pad.x + forward.y * pad.y + forward.z * pad.z <= -0.7f ||
                forward.x * stick.x + forward.y * stick.y + forward.z * stick.z <= -0.7f) {
                result = 1;
            }
        }
    }
    return result;
}

void fn_80018A34(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        fn_8004D2B8(creature, 1);
    }
}

void fn_80018A4C(const UnkScriptContext* ctx) {}

void fn_80018A50(const UnkScriptContext* ctx) {}

void fn_80018A54(const UnkScriptContext* ctx) {}

void fn_80018A58(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0 && !fn_8004D520(creature, 0)) {
        fn_801BA190(lbl_805FB6C8, creature, 0, 0xC7EA30FB, 0, 0, 0, 0);
    }
}

void fn_80018AC4(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        void* self = creature->vfF0();
        UnkCombatSlot* slot = lbl_805FBBB8->mA00;
        if (slot == self) {
            slot = lbl_805FBBB8->mA04;
        }
        fn_801BA190(lbl_805FB6C8, static_cast<UnkCombatant*>(slot->m1C), 0, 0x72397992, 0, 0, 0, 0);
    }
}

void fn_80018B44(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0 && fn_8004D5BC(creature)) {
        fn_801BA190(lbl_805FB6C8, creature, 0, 0xC7EA30FB, 0, 0, 0, 0);
    }
}

void fn_80018BAC(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        fn_8004D2B8(creature, 1);
    }
}

void fn_80018BC4(const UnkScriptContext* ctx) {}

int fn_80018BC8(const UnkScriptContext* ctx) {
    UnkCreature* creature = ctx->mEntity;
    if (creature != 0) {
        return fn_8004D5BC(creature);
    }
    return 1;
}

void fn_80018BE0(const UnkScriptContext* ctx) {}

}
