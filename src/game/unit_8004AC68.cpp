// Game unit at 0x8004AC68-0x8004B498 (real file name unknown): the
// BeginAndEndCombatRoundCallback class (RTTI), derived from
// XBaseBeginAndEndCombatRoundCallback (RTTI; its constructor, destructor and virtual
// functions are at 0x8020F30C-0x8020F368, vtable at 0x80388E50). At the start of a
// combat round it marks the fighters whose selected action triggers an effect and
// plays it; at the end it plays the effects of the parts that took damage. Bounds:
// the previous unit's static initializer ends at 0x8004AC68 and this unit's static
// initializer ends at 0x8004B498.

#include "game/unk_vec3.mch"
#include "game/ActionSelectionCallback.h"
#include "game/externs.h"
#include "game/unk_vec3.h"

// Round information given to vf0C (real type unknown).
struct UnkRoundStart {
    /* 0x0 */ int m0;
};

// Damage per fighter and part given to vf10 (real type unknown).
struct UnkRoundEnd {
    /* 0x00 */ int mDamage[2][5];
};

class XBaseBeginAndEndCombatRoundCallback {
public:
    XBaseBeginAndEndCombatRoundCallback();
    virtual ~XBaseBeginAndEndCombatRoundCallback();
    virtual void vf0C(UnkRoundStart* start);
    virtual void vf10(UnkRoundEnd* end);
    virtual int vf14(f32 dt);
};

class BeginAndEndCombatRoundCallback : public XBaseBeginAndEndCombatRoundCallback {
public:
    BeginAndEndCombatRoundCallback();
    virtual ~BeginAndEndCombatRoundCallback() {}
    virtual void vf0C(UnkRoundStart* start);
    virtual void vf10(UnkRoundEnd* end);
    virtual int vf14(f32 dt);

    /* 0x04 */ u8 mTriggered[2];
    /* 0x06 */ u8 m6;
    /* 0x08 */ f32 mDuration;
    /* 0x0C */ f32 mTimer;
    /* 0x10 */ int mFighter;
    /* 0x14 */ u8 m14;
};

// Entity owning a part; the part points to its base at 0x264 (real types unknown).
struct UnkPad264 {
    /* 0x000 */ u8 unk0[0x264];
};
struct UnkPartOwnerBase {
    /* 0x0 */ u8 unk0[0x4];
};
struct UnkPartOwner : UnkPad264, UnkPartOwnerBase {};

// Part description (real type unknown).
struct UnkPartInfo {
    /* 0x00 */ u8 unk0[0x20];
    /* 0x20 */ int mType;
};

// Combat part of a fighter (real types unknown).
struct UnkPartLink {
    /* 0x0 */ UnkPartInfo* mInfo;
};
struct UnkCombatPart {
    /* 0x00 */ u8 unk0[0x20];
    /* 0x20 */ UnkPartOwnerBase* mOwner;
    /* 0x24 */ u8 unk24[0x28 - 0x24];
    /* 0x28 */ UnkPartLink mLink;
};

// Fighter of the combat (real type unknown).
struct UnkFighter {
    /* 0x00 */ UnkCombatPart* mParts[8];
    /* 0x20 */ u8 m20[0x4];
};

// Combat state at lbl_805FBBB8 (real type unknown).
struct UnkCombat {
    /* 0x000 */ u8 unk0[0xA00];
    /* 0xA00 */ UnkFighter* mFighters[2];
    /* 0xA08 */ u8 unkA08[0xA0C - 0xA08];
    /* 0xA0C */ void* mA0C;
    /* 0xA10 */ u8 unkA10[0xA4C - 0xA10];
    /* 0xA4C */ int mA4C;
};

// Database records (real types unknown).
struct UnkSelectionRecord {
    /* 0x00 */ u32 m0;
    /* 0x04 */ u32 m4;
    /* 0x08 */ u32 m8;
    /* 0x0C */ u32 mC;
    /* 0x10 */ u32 m10;
};
struct UnkActionRecord {
    /* 0x00 */ u32 m0;
    /* 0x04 */ u32 m4;
    /* 0x08 */ int m8;
    /* 0x0C */ u32 mC;
    /* 0x10 */ u32 m10;
    /* 0x14 */ u32 m14;
    /* 0x18 */ u32 m18;
    /* 0x1C */ u32 m1C;
    /* 0x20 */ u32 m20;
};
struct UnkEffectRecord {
    /* 0x00 */ u32 m0;
    /* 0x04 */ u32 m4;
    /* 0x08 */ u32 m8;
    /* 0x0C */ u32 mC;
    /* 0x10 */ int mEffect;
    /* 0x14 */ u32 m14;
    /* 0x18 */ u32 m18;
    /* 0x1C */ u32 m1C;
    /* 0x20 */ u32 m20;
};
struct UnkCombatTables {
    /* 0x0000 */ u8 unk0[0x8];
    /* 0x0008 */ UnkActionRecord* mActions;
    /* 0x000C */ u8 unkC[0x430 - 0xC];
    /* 0x0430 */ UnkSelectionRecord* mSelections;
    /* 0x0434 */ u8 unk434[0x3610 - 0x434];
    /* 0x3610 */ UnkEffectRecord* mEffects;
};
struct UnkCombatDatabase {
    /* 0x0 */ u8 unk0[0xC];
    /* 0xC */ UnkCombatTables* mTables;
};

// Records returned by fn_8021679C and fn_802167E4 (real types unknown).
struct UnkEffectSlot {
    /* 0x0 */ u8 unk0[0x8];
    /* 0x8 */ int m8;
    /* 0xC */ int mC;
};

extern "C" {
extern UnkCombat* lbl_805FBBB8;
extern UnkCombatDatabase* lbl_805FBBC0;
// Debug text buffer (real name unknown).
extern char lbl_803C5358[];
// Effect hashes (real name unknown).
extern u32 lbl_80388258[];

int fn_8003F7DC(void);
int fn_8003F8D4(void);
UnkActionSelection fn_80041810(void* obj);
void fn_8004504C(void);
void fn_80048BF4(int fighter, int arg);
void fn_800F4C9C(UnkPartOwner* owner, u32 hash, u32 arg);
void fn_800F4E00(UnkPartOwner* owner, u32 hash, u32 arg);
void fn_800F4EB4(int arg);
void fn_800F4F4C(int arg);
int fn_80214F48(UnkRoundStart* start);
void fn_802152B4(void* obj, int fighter, int part, int arg);
int fn_802152E4(void* obj, int fighter, int part);
UnkEffectSlot* fn_8021679C(UnkCombatDatabase* db, int effect);
UnkEffectSlot* fn_802167E4(UnkCombatDatabase* db, int effect, int part);
}

u32 fn_800523B8(void* obj, int id, int mode);

BeginAndEndCombatRoundCallback::BeginAndEndCombatRoundCallback() {
    mDuration = 0.0f;
    mTimer = 0.0f;
    mFighter = -1;
    m14 = 0;
    mTriggered[0] = 0;
    mTriggered[1] = 0;
    m6 = 0;
}

void BeginAndEndCombatRoundCallback::vf0C(UnkRoundStart* start) {
    fn_80142EF8(lbl_803C5358, "BeginRound 1\n");
    mDuration = 0.25f;
    if (fn_8003F8D4() != 0) {
        fn_800F4F4C(fn_8003F7DC());
    } else {
        fn_800F4EB4(fn_8003F7DC());
    }
    mFighter = fn_80214F48(start);
    m14 = 1;
    if (start->m0 == 6) {
        goto end;
    }
    for (int i = 0; i < 2; i++) {
        fn_80142EF8(lbl_803C5358, "BeginRound 2\n");
        UnkFighter* fighter = lbl_805FBBB8->mFighters[i];
        fn_80142EF8(lbl_803C5358, "BeginRound 3 (0x%x)\n", fighter);
        void* selection = fighter->m20;
        fn_80142EF8(lbl_803C5358, "BeginRound 4 (0x%x)\n", selection);
        int index = fn_80041810(selection).m8;
        fn_80142EF8(lbl_803C5358, "BeginRound 4 (0x%x)\n", selection);
        UnkCombatPart* part = fighter->mParts[index];
        if (part == 0) {
            continue;
        }
        if (&part->mLink == 0) {
            continue;
        }
        int selected = fn_80041810(selection).m0;
        UnkSelectionRecord record = lbl_805FBBC0->mTables->mSelections[selected];
        int type = part->mLink.mInfo->mType;
        UnkActionRecord action = lbl_805FBBC0->mTables->mActions[record.m4];
        if (action.m8 == 0 && m6 == 0 && type == 15 && mFighter == i) {
            mTriggered[i] = 1;
            m6 = 1;
        }
        if ((action.m8 == 1 || i == mFighter) && type == 10) {
            mTriggered[i] = 1;
        }
        fn_80142EF8(lbl_803C5358, "BeginRound 5\n");
        if (mTriggered[i] == 0) {
            continue;
        }
        if (mDuration < 2.0f) {
            mDuration = 2.0f;
        }
        if (type == -1 || type >= 16) {
            continue;
        }
        fn_80142EF8(lbl_803C5358, "BeginRound 6\n");
        UnkEffectRecord effect = lbl_805FBBC0->mTables->mEffects[type];
        if (effect.mEffect == -1 || effect.mEffect >= 0x2A9) {
            continue;
        }
        int id = fn_8021679C(lbl_805FBBC0, type)->mC;
        int slot = fn_8021679C(lbl_805FBBC0, type)->m8;
        u32 arg = fn_800523B8(lbl_805FBBC0, id, slot);
        fn_80142EF8(lbl_803C5358, "BeginRound 7\n");
        UnkCombatPart* target = lbl_805FBBB8->mFighters[i]->mParts[slot];
        if (target != 0) {
            fn_800F4C9C(static_cast<UnkPartOwner*>(target->mOwner), lbl_80388258[effect.mEffect],
                        arg);
        }
    }
end:
    mTimer = 0.0f;
    fn_80142EF8(lbl_803C5358, "");
}

// 98.9 %: the loop counters and pointers take saved registers one above the DOL's,
// and the effect index of the last block is read back from the stack copy (orders of
// declarations and statements tried).
void BeginAndEndCombatRoundCallback::vf10(UnkRoundEnd* end) {
    void* obj;
    UnkPartOwner* owner;
    int i;
    int damaged;
    int j;
    UnkCombatPart* part;
    fn_80142EF8(lbl_803C5358, "EndRound 1\n");
    m14 = 0;
    for (i = 0; i < 2; i++) {
        damaged = 0;
        for (j = 0; j < 5; j++) {
            obj = lbl_805FBBB8->mA0C;
            part = lbl_805FBBB8->mFighters[i]->mParts[j];
            if (part != 0) {
                fn_80142EF8(lbl_803C5358, "EndRound 2\n");
                owner = static_cast<UnkPartOwner*>(part->mOwner);
                int type = fn_802152E4(obj, i, j);
                if (type != -1 && type < 16) {
                    UnkEffectRecord effect = lbl_805FBBC0->mTables->mEffects[type];
                    u32 hash = lbl_80388258[effect.mEffect];
                    int id = fn_802167E4(lbl_805FBBC0, type, j)->mC;
                    fn_800F4E00(owner, hash, fn_800523B8(lbl_805FBBC0, id, j));
                }
            }
            if (end->mDamage[i][j] > 0 && lbl_805FBBB8->mA4C == 0) {
                fn_80142EF8(lbl_803C5358, "EndRound 3\n");
                damaged = 1;
                fn_802152B4(obj, i, j, 7);
                fn_80048BF4(i, 0);
                if (mDuration < 1.0f) {
                    mDuration = 1.0f;
                }
            }
        }
        if (damaged) {
            fn_80142EF8(lbl_803C5358, "EndRound 4\n");
            int id = fn_8021679C(lbl_805FBBC0, 7)->mC;
            int slot = fn_8021679C(lbl_805FBBC0, 7)->m8;
            u32 arg = fn_800523B8(lbl_805FBBC0, id, slot);
            UnkEffectRecord effect = lbl_805FBBC0->mTables->mEffects[7];
            UnkCombatPart* target = lbl_805FBBB8->mFighters[i]->mParts[slot];
            if (target != 0) {
                fn_800F4C9C(static_cast<UnkPartOwner*>(target->mOwner),
                            lbl_80388258[effect.mEffect], arg);
            }
        }
    }
    fn_8004504C();
    m6 = 0;
    fn_80142EF8(lbl_803C5358, "");
}

int BeginAndEndCombatRoundCallback::vf14(f32 dt) {
    if (mDuration > mTimer) {
        mTimer += dt;
        if (mTimer >= 0.0f && m14 != 0) {
            fn_80048BF4(mFighter, 0);
        }
        return 0;
    }
    if (m14 != 0) {
        fn_80048BF4(mFighter, 0);
    }
    mTriggered[0] = 0;
    mTriggered[1] = 0;
    return 1;
}
