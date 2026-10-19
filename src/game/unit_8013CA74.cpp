// Game unit at 0x8013CA74-0x8013E4D0 (real file name unknown): the match screen of a
// fight against a friend's team. Keeps the player's and the opponent's cursor actors
// at their rest positions, shows the names and the "%d" counters, places the
// opponent's five monsters on their stands, runs the confirm and cancel prompts and
// builds the fight teams when the confirmation completes. Also the global
// XNonInventoryMonster used for the opponent (built by the static initializer).
// Bounds come from the static initializers of this unit and the previous one.
//
// NONMATCHING: every function matches but fn_8013E0EC (99.9 %, one register). The
// precompiled header gives the DOL's .bss order (sUnkVec3 initialized first, placed
// last).

#include "game/unk_vec3.mch"

#include "types.h"

// Opponent and team names.
static char sOpponentName[16];
static char sTeamName[32];

// The DOL's static initializer addresses sUnkVec3, the opponent monster and its
// destructor chain with one lis each instead of one shared .bss base: only
// pool_data off gives that.
#pragma pool_data off

// Monster whose data is not in the player's inventory (name from RTTI; only the members
// used here are known). The functions at 0x8020EAD4-0x8020EDF0 are its constructor,
// destructor and two of its virtual functions, called directly here.
class XNonInventoryMonster {
public:
    XNonInventoryMonster();
    ~XNonInventoryMonster();
    void fn_8020EDAC();
    void fn_8020EDF0(void* data);

    /* 0x000 */ u8 unk0[0x4];
    /* 0x004 */ void* m4[5];
    /* 0x018 */ u8 unk18[0x44 - 0x18];
    /* 0x044 */ s32 m44;
    /* 0x048 */ u8 unk48[0x13C - 0x48];
};

#include "game/externs.h"
#include "game/unk_actor.h"
#include "game/unk_vec3.h"
#include <string.h>

XNonInventoryMonster lbl_803F20C4;

// Monster slot of a team (real type unknown).
struct UnkMonsterSlot {
    /* 0x00 */ s32 mId;
    /* 0x04 */ s32 m4;
    /* 0x08 */ s32 m8;
    /* 0x0C */ u8 unkC[0x10 - 0xC];
    /* 0x10 */ s8 m10;
    /* 0x11 */ u8 m11;
};

// Team with its monster models (real type unknown).
struct UnkTeamData {
    /* 0x000 */ u8 unk0[0x898];
    /* 0x898 */ u8 m898[0x4];
    /* 0x89C */ UnkMonsterSlot* mSlots[5];
    /* 0x8B0 */ u8 unk8B0[0xC50 - 0x8B0];
    /* 0xC50 */ UnkActor* mModels[5];
    /* 0xC64 */ u8 unkC64[0xC6C - 0xC64];
    /* 0xC6C */ void* mPending[5];
};

typedef UnkTeamData UnkStands;

// Saved opponent record entry (real type unknown).
struct UnkRecordSlot {
    /* 0x0 */ s32 m0;
    /* 0x4 */ u8 m4;
    /* 0x5 */ s8 m5;
    /* 0x8 */ s32 m8;
    /* 0xC */ s32 mC;
};

// Monster description built for a fight team (0x40 bytes or less on the stack, real type
// unknown); see fn_8021D320.
struct UnkMonsterDesc {
    /* 0x00 */ u8 unk0[0x30];
};

// One fight team (real type unknown): a vtable whose function at 0x14 stores a monster.
class UnkFightTeam {
public:
    virtual void vf08();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14(int index, UnkMonsterDesc* desc);
};

// Global game object at 0x803B53A0 (real type unknown).
struct UnkGame {
    /* 0x0000 */ u8 unk0[0xAB98];
    /* 0xAB98 */ UnkFightTeam mTeam0;
    /* 0xAB9C */ u8 unkAB9C[0xACD4 - 0xAB9C];
    /* 0xACD4 */ UnkFightTeam mTeam1;
    /* 0xACD8 */ u8 unkACD8[0xD62C - 0xACD8];
    /* 0xD62C */ s32 mD62C;
    /* 0xD630 */ s32 mD630;
};

// Friend roster entry of the save data (real type unknown).
struct UnkFriendEntry {
    /* 0x00 */ char mName[0x10];
    /* 0x10 */ u8 unk10[0x20 - 0x10];
};

struct UnkSaveData {
    /* 0x000 */ u8 unk0[0x340];
    /* 0x340 */ UnkFriendEntry mFriends[64];
    /* 0xB40 */ char mName[0x10];
};

// Opponent monster data copied from the item database (0x48 bytes, real type unknown).
struct UnkOpponentData {
    /* 0x00 */ u32 m0;
    /* 0x04 */ u32 m4;
    /* 0x08 */ u32 m8;
    /* 0x0C */ u32 mC;
    /* 0x10 */ u32 m10;
    /* 0x14 */ u32 m14;
    /* 0x18 */ u32 m18;
    /* 0x1C */ u32 m1C;
    /* 0x20 */ u32 m20;
    /* 0x24 */ u32 m24;
    /* 0x28 */ u32 m28;
    /* 0x2C */ u32 m2C;
    /* 0x30 */ u32 m30;
    /* 0x34 */ u8 m34[16];
    /* 0x44 */ u8 m44;
};

struct UnkItemTable {
    /* 0x0000 */ u8 unk0[0x9180];
    /* 0x9180 */ u8* m9180;
};

struct UnkItemDatabase {
    /* 0x0 */ u8 unk0[0xC];
    /* 0xC */ UnkItemTable* mData;
};

struct UnkGameData {
    /* 0x000 */ u8 unk0[0xA28];
    /* 0xA28 */ s32 mA28;
    /* 0xA2C */ s32 mA2C;
};

extern "C" {
extern void* lbl_805FB654;
extern UnkGameData* lbl_805FBBE0;
extern UnkItemDatabase* lbl_805FBBC0;
extern UnkSaveData lbl_80556E80;
extern UnkGame lbl_803B53A0;
extern UnkRecordSlot lbl_803E69D8[5];
extern UnkOpponentData lbl_803F2250;
extern void* lbl_803C4E28[];
extern s32 lbl_805FB034;
extern s32 lbl_805FB038;
extern u32 lbl_805F9030;
extern f32 lbl_805FB918;
// Timer and its flag, defined in another unit (real names unknown).
extern f32 lbl_805FB410;
extern u8 lbl_805FB414;

void* fn_8018D1E4(void* obj);
void* fn_80198E34(void* screens, u32 hash);
UnkActor* fn_80198F18(void* screens, u32 hash);
void* fn_80199AEC(void* screen, u32 hash);
void fn_80199B38(void* screen, u32 hash);
void fn_8019EFC8(void* widget, const char* text, int arg);
void fn_801A3FA0(void* widget, f32 value);
void fn_801A3010(void* actor, void* arg);
u32 fn_80140B58(void);
void fn_80140B50(u32 hash);
void fn_80140B8C(int arg);
void fn_80140B9C(int arg);
int fn_801409FC(void);
void fn_8013F394(int arg);
void fn_8013F2A4(int arg);
void fn_8013FBCC(void* screen, u32 hash, int arg);
char* fn_8013E774(void);
void* fn_801B7198(void* obj, int arg);
int fn_8004E300(void* obj, int arg);
void fn_8018F618(u32 hash, int arg);
void fn_8018F580(u32 hash);
void fn_801F5724(int arg);
void fn_801F5794(int arg);
void fn_801F5814(int arg);
void* fn_801F3918(int type, int arg);
void fn_8010FDA4(void);
void fn_8010FF78(u32 hash);
void fn_80110048(u32 hash);
int fn_80113808(void);
UnkTeamData* fn_80113974(void);
int fn_80142490(void);
void fn_80190BC8(u32 hash);
void fn_80190AFC(u32 hash);
void fn_8000C7F4(u32 hash, int arg1, int arg2);
UnkStands* fn_800E876C(u32 id);
void fn_8004DCEC(UnkStands* stands, int index);
void* fn_8004FFB4(void* pending, void* arg);
void fn_8004C718(UnkStands* stands, int index, void* model);
void fn_8004EDD0(UnkActor* model, int arg1, int arg2);
void fn_8003DCE0(UnkActor* model);
void fn_8004D760(UnkStands* stands);
s32 fn_8004DAF8(UnkStands* stands, int arg1, int arg2);
void fn_8020EBB8(UnkFightTeam* team, void* data);
void fn_8021C8E0(UnkFightTeam* team, const char* name);
void fn_8021D320(UnkMonsterDesc* desc);
void fn_8021D374(UnkMonsterDesc* desc, int arg);
void fn_8021D408(UnkMonsterDesc* desc);
void fn_8021D43C(UnkMonsterDesc* desc, s32 id);
void fn_8021DA68(UnkMonsterDesc* desc, u8 value);
void fn_8021DA40(UnkMonsterDesc* desc, s8 value);
void fn_8021DA88(UnkMonsterDesc* desc, s32 value);
void fn_8021DA80(UnkMonsterDesc* desc, s32 value);
}

static f32 sValue0;
static f32 sValue1;
static f32 sValue2;
static f32 sValue3;
static s32 sCount0;
static s32 sCount1;
static u8 sCancelShown;
static u8 sConfirmShown;
static u8 sGauge;
static u8 sConfirmed;
static u8 sFlagB4;
static u8 sActive;
static s32 sMode;
static UnkActor* sCursor;
static UnkTeamData* sTeam;
static UnkTeamData* sTeamC4;
static UnkActor* sOpponentCursor;
static UnkStands* sStands;
static s32 sStandsState;
static s32 sD4;
static f32 sGaugeValue;
static u8 sLoadRequest;
static u8 sLoading;
static u8 sPromptShown;
static s32 sE0;
static s32 sOpponentBlink;
static s32 sBlink;
static s32 sEC;

static u8 sSoundA = 1;
static u8 sSoundB = 1;

// Rest positions of the two cursors, taken from the actors on first use.
static f32 sCursorX = -1.0f;
static f32 sCursorY = -1.0f;
static f32 sCursorZ = -1.0f;
static f32 sOpponentX = -1.0f;
static f32 sOpponentY = -1.0f;
static f32 sOpponentZ = -1.0f;

// Constant (0, 0, -0.2) in .rodata among other 12-byte vectors used by units all over
// the game, so probably a global defined elsewhere (real name unknown).
extern const UnkVec3 lbl_80345FFC;

// Plays the animation anim of the match screen when its widget check exists.
static inline void PlayPrompt(u32 check, u32 anim, u8& shown, u8 value) {
    void* screens = fn_8018D1E4(lbl_805FB654);
    if (screens != 0) {
        void* screen = fn_80198E34(screens, 0x9631294E);
        if (screen != 0) {
            if (fn_80199AEC(screen, 0x4DD41D3D) != 0) {
                if (fn_80199AEC(screen, check) != 0) {
                    fn_80199B38(screen, anim);
                    shown = value;
                }
            }
        }
    }
}

static inline void ShowHint(int on) {
    void* screens = fn_8018D1E4(lbl_805FB654);
    if (screens != 0) {
        void* screen = fn_80198E34(screens, 0x9631294E);
        if (screen != 0) {
            fn_8013FBCC(screen, 0xCC13FD8B, on);
        }
    }
}

static inline void HideHint() {
    void* screens = fn_8018D1E4(lbl_805FB654);
    if (screens != 0) {
        void* screen = fn_80198E34(screens, 0x9631294E);
        if (screen != 0) {
            fn_8018F580(0xCC13FD8B);
            fn_8013FBCC(screen, 0xCC13FD8B, 0);
        }
    }
}

static inline void ReleaseModel(UnkTeamData* team, int index) {
    UnkActor* model = team->mModels[index];
    fn_8004C718(team, index, 0);
    fn_8003DCE0(model);
}

extern "C" {

void fn_8013CA74(void) {
    sOpponentName[0] = 0;
    sMode = 0;
    sConfirmed = 0;
    lbl_805FB414 = 0;
    sFlagB4 = 0;
    sActive = 0;
    sLoadRequest = 0;
    sLoading = 0;
    sPromptShown = 0;
    sStands = 0;
    sE0 = 0;
    fn_8010FDA4();
    fn_8010FF78(0xF5EF54D9);
    fn_80110048(0xE5368B8B);
    UnkOpponentData data = *(UnkOpponentData*)(lbl_805FBBC0->mData->m9180 + 0x360);
    lbl_803F2250 = data;
    fn_8013F394(0);
    fn_8013F2A4(0);
    fn_801F5794(1);
    fn_801F5814(0);
}

void fn_8013CC60(void) {
    char buf[16];
    if (fn_80140B58() == 0x9D31BDAB) {
        return;
    }
    sCancelShown = 0;
    sConfirmShown = 0;
    sGauge = 0;
    sSoundA = 1;
    sSoundB = 1;
    sBlink = 0;
    sE0 = 1;
    void* screens = fn_8018D1E4(lbl_805FB654);
    if (screens != 0) {
        sCursor = fn_80198F18(screens, 0x8FB60C54);
        if (sCursor != 0) {
            if (-1.0f == sCursorX && -1.0f == sCursorY && -1.0f == sCursorZ) {
                sCursorX = sCursor->mPos.x;
                sCursorY = sCursor->mPos.y;
                sCursorZ = sCursor->mPos.z;
            }
        }
        sOpponentCursor = fn_80198F18(screens, 0x6530D136);
        if (sOpponentCursor != 0) {
            sOpponentX = sOpponentCursor->mPos.x;
            sOpponentY = sOpponentCursor->mPos.y;
            sOpponentZ = sOpponentCursor->mPos.z;
        }
        if (sStands != 0) {
            lbl_803F20C4.m44 = 0x39;
        }
        void* screen = fn_80198E34(screens, 0x9631294E);
        if (screen != 0 && fn_80199AEC(screen, 0x4DD41D3D) != 0) {
            void* name = fn_80199AEC(screen, 0x514126CC);
            void* team = fn_80199AEC(screen, 0xC3F30972);
            if (name != 0 && team != 0) {
                fn_8019EFC8(name, lbl_80556E80.mName, 1);
                fn_8019EFC8(team, fn_8013E774(), 1);
            }
            void* count0 = fn_80199AEC(screen, 0x9C492A14);
            void* count1 = fn_80199AEC(screen, 0x674FFA87);
            if (count0 != 0 && count1 != 0) {
                fn_80142EF8(buf, "%d", lbl_805FBBE0->mA28);
                fn_8019EFC8(count0, buf, 1);
                fn_80142EF8(buf, "%d", lbl_805FBBE0->mA2C);
                fn_8019EFC8(count1, buf, 1);
            }
            void* opponent = fn_80199AEC(screen, 0xC8487776);
            if (opponent != 0) {
                fn_8019EFC8(opponent, sTeamName, 1);
            }
            void* opp0 = fn_80199AEC(screen, 0xEBD7F8E4);
            void* opp1 = fn_80199AEC(screen, 0xDA859649);
            if (opp0 != 0 && opp1 != 0) {
                fn_80142EF8(buf, "%d", sCount0);
                fn_8019EFC8(opp0, buf, 1);
                fn_80142EF8(buf, "%d", sCount1);
                fn_8019EFC8(opp1, buf, 1);
            }
        }
    }
    fn_80140B8C(0x10);
    fn_80140B8C(8);
    fn_80140B50(0x9D31BDAB);
    fn_80140B9C(1);
}

void fn_8013CF6C(void) {
    fn_8018F618(0x8FB60C54, 0);
    fn_8018F618(0x6530D136, 0);
}

void fn_8013DC54(u8 on);
void fn_8013DE7C(void);
void fn_8013E0EC(void);

void fn_8013CFA8(void) {
    if (fn_801409FC() == 0) {
        return;
    }
    if (sCursor != 0) {
        if (-1.0f == sCursorX && -1.0f == sCursorY && -1.0f == sCursorZ) {
            sCursorX = sCursor->mPos.x;
            sCursorY = sCursor->mPos.y;
            sCursorZ = sCursor->mPos.z;
        }
        UnkVec3 pos(sCursorX, sCursorY, sCursorZ);
        void* target = fn_801B7198((u8*)sCursor + 0x140, 0);
        if (target != 0 && fn_8004E300(target, 1) == 13) {
            pos += UnkVec3(lbl_80345FFC.x, lbl_80345FFC.y, lbl_80345FFC.z);
        }
        sCursor->mPos = UnkVec3(pos);
        sCursor->vf2C(0x18);
    }
    if (sOpponentCursor != 0) {
        if (-1.0f == sOpponentX && -1.0f == sOpponentY && -1.0f == sOpponentZ) {
            sOpponentX = sOpponentCursor->mPos.x;
            sOpponentY = sOpponentCursor->mPos.y;
            sOpponentZ = sOpponentCursor->mPos.z;
        }
        UnkVec3 pos(sOpponentX, sOpponentY, sOpponentZ);
        void* target = fn_801B7198((u8*)sOpponentCursor + 0x140, 0);
        if (target != 0 && fn_8004E300(target, 1) == 13) {
            pos += UnkVec3(lbl_80345FFC.x, lbl_80345FFC.y, lbl_80345FFC.z);
        }
        sOpponentCursor->mPos = UnkVec3(pos);
        sOpponentCursor->vf2C(0x18);
    }
    if (sCursor != 0) {
        if (fn_80113808() != 0 && !(sCursor->m64 & 2) && sBlink == 0) {
            sTeam = fn_80113974();
            if (sTeam != 0) {
                fn_801A3010(sCursor, sTeam);
                sBlink = 3;
            }
        }
        if (sBlink > 0 && --sBlink == 0) {
            if (sSoundA) {
                sSoundA = 0;
                fn_8018F618(0x363BBFFF, 0);
            }
            sCursor->vf2C(2);
        }
    }
    fn_8013E0EC();
    fn_8013DE7C();
    if (sOpponentCursor != 0) {
        if (!(sOpponentCursor->m64 & 2) && sActive && sOpponentBlink == 0) {
            sOpponentBlink = 3;
        }
        if (sOpponentBlink > 0 && --sOpponentBlink == 0) {
            sOpponentCursor->vf2C(2);
        }
    }
    if (sActive && sOpponentBlink <= 0) {
        if (sSoundB) {
            sSoundB = 0;
            fn_8018F618(0xAF32EE45, 0);
        }
        if (sPromptShown != 1) {
            sPromptShown = 1;
            fn_8018F618(0x5AFA58C8, 1);
        }
        if (sMode == 0) {
            fn_8018F618(0xE24CF5B1, 1);
            fn_8018F618(0x00682601, 1);
            fn_8013DC54(0);
            if (sConfirmShown != 1) {
                PlayPrompt(0x01AAFF52, 0x2F283883, sConfirmShown, 1);
            }
            ShowHint(1);
            if (sConfirmed == 1) {
                fn_8013F2A4(1);
            } else {
                fn_8013F2A4(0);
            }
        } else {
            if (sConfirmShown) {
                PlayPrompt(0x01AAFF52, 0x4A36C76E, sConfirmShown, 0);
            }
            HideHint();
            if (sConfirmed == 0) {
                fn_8018F618(0xE24CF5B1, 1);
                fn_8018F618(0x00682601, 1);
                fn_8013DC54(0);
                if (sCancelShown != 1) {
                    PlayPrompt(0xA3556852, 0x63CC8B0B, sCancelShown, 1);
                }
            } else {
                fn_8013F2A4(0);
                if (sCancelShown) {
                    PlayPrompt(0xA3556852, 0xA51E8C39, sCancelShown, 0);
                }
                fn_8018F618(0xE24CF5B1, 0);
                fn_8018F618(0x00682601, 0);
                fn_8013DC54(1);
            }
        }
    } else {
        HideHint();
        fn_8018F618(0x6530D136, 0);
        if (sPromptShown) {
            sPromptShown = 0;
            fn_8018F618(0x5AFA58C8, 0);
        }
        if (sSoundB != 1) {
            sSoundB = 1;
            fn_8018F618(0xAF32EE45, 1);
        }
        if (sConfirmShown) {
            PlayPrompt(0x01AAFF52, 0x4A36C76E, sConfirmShown, 0);
        }
        if (sCancelShown) {
            PlayPrompt(0xA3556852, 0xA51E8C39, sCancelShown, 0);
        }
        fn_8018F618(0xE24CF5B1, 1);
        fn_8018F618(0x00682601, 1);
        fn_8013DC54(0);
        if (sConfirmed == 0) {
            fn_8013F2A4(0);
        }
    }
    void* screens = fn_8018D1E4(lbl_805FB654);
    if (screens == 0) {
        return;
    }
    if (lbl_805FB414 == 1) {
        void* timer = fn_80198E34(screens, 0x43538242);
        if (timer != 0 && fn_80199AEC(timer, 0x9D841CAA) != 0) {
            void* text = fn_80199AEC(timer, 0x29DD4A4E);
            if (text != 0) {
                if (0.0f == lbl_805FB410) {
                    fn_801F5724(0x2000);
                }
                char buf[16];
                fn_80142EF8(buf, "0:%0.02d", (int)lbl_805FB410);
                fn_8019EFC8(text, buf, 1);
            }
        }
    }
    void* screen = fn_80198E34(screens, 0x9631294E);
    if (screen == 0) {
        return;
    }
    if (fn_80199AEC(screen, 0x4DD41D3D) == 0) {
        return;
    }
    if (sGauge == 1 && fn_80199AEC(screen, 0xDE4906B0) != 0) {
        void* gauge = fn_80199AEC(screen, 0x25E15199);
        if (gauge != 0) {
            f32 duration = 5.0f;
            sGaugeValue = sGaugeValue + lbl_805FB918;
            fn_801A3FA0(gauge, sGaugeValue / duration);
            if (sGaugeValue / duration >= 1.0f && fn_80142490() == 0) {
                fn_801F3918(0x4D, 0);
                UnkMonsterDesc desc;
                fn_8021D320(&desc);
                lbl_803B53A0.mD62C = 0;
                fn_8020EBB8(&lbl_803B53A0.mTeam0, sTeam->m898);
                for (int i = 0; i < 5; i++) {
                    fn_8021D408(&desc);
                    if (sTeam->mSlots[i] != 0 && sTeam->mSlots[i]->mId != -1) {
                        fn_8021D43C(&desc, sTeam->mSlots[i]->mId);
                        fn_8021DA68(&desc, sTeam->mSlots[i]->m11);
                        fn_8021DA40(&desc, sTeam->mSlots[i]->m10);
                        fn_8021DA88(&desc, sTeam->mSlots[i]->m8);
                        fn_8021DA80(&desc, sTeam->mSlots[i]->m4);
                    }
                    UnkFightTeam* team = &lbl_803B53A0.mTeam0;
                    team->vf14(i, &desc);
                }
                lbl_803B53A0.mD630 = 1;
                UnkRecordSlot* slot = lbl_803E69D8;
                for (int i = 0; i < 5; i++) {
                    fn_8021D43C(&desc, slot->m0);
                    fn_8021DA68(&desc, slot->m4);
                    fn_8021DA40(&desc, slot->m5);
                    fn_8021DA88(&desc, slot->m8);
                    fn_8021DA80(&desc, slot->mC);
                    UnkFightTeam* team = &lbl_803B53A0.mTeam1;
                    team->vf14(i, &desc);
                    slot++;
                }
                fn_8021C8E0(&lbl_803B53A0.mTeam1, sOpponentName);
                fn_8018F618(0xE24CF5B1, 0);
                fn_8018F618(0x00682601, 0);
                fn_8013DC54(0);
                fn_8000C7F4(0xF7E43ED2, 0, 0);
                fn_8021D374(&desc, -1);
            }
        }
    }
    if (fn_80199AEC(screen, 0x7CC42DED) != 0) {
        void* name = fn_80199AEC(screen, 0xC8487776);
        void* team = fn_80199AEC(screen, 0x5AFA58C8);
        if (name != 0 && team != 0) {
            if (lbl_805F9030 != 0x40) {
                fn_8019EFC8(name, lbl_80556E80.mFriends[lbl_805F9030].mName, 1);
            }
            fn_8019EFC8(team, sOpponentName, 1);
        }
    }
}

void fn_8013DC54(u8 on) {
    if (sGauge == on) {
        return;
    }
    void* screens = fn_8018D1E4(lbl_805FB654);
    if (screens != 0) {
        void* screen = fn_80198E34(screens, 0x9631294E);
        if (screen != 0) {
            if (fn_80199AEC(screen, 0x4DD41D3D) != 0) {
                if (fn_80199AEC(screen, 0xDE4906B0) != 0) {
                    if (on == 1) {
                        fn_80199B38(screen, 0xBFD893EF);
                        sGaugeValue = 0.0f;
                    } else {
                        fn_80199B38(screen, 0x77C91068);
                    }
                    sGauge = on;
                }
            }
        }
    }
}

void fn_8013DD1C(void) {
    fn_801F3918(0x40, 0);
    sMode = 0;
    sConfirmed = 0;
}

void fn_8013DD50(void) {
    fn_801F3918(0x41, 0);
    sMode = 1;
}

void fn_8013DD80(void) {
    fn_801F3918(0x42, 0);
    sMode = 0;
    sConfirmed = 0;
}

void fn_8013DDB4(void) {
    if (sCursor != 0) {
        if (fn_80113808() != 0 && !(sCursor->m64 & 2) && sBlink == 0) {
            sTeam = fn_80113974();
            if (sTeam != 0) {
                fn_801A3010(sCursor, sTeam);
                sBlink = 3;
            }
        }
        if (sBlink > 0 && --sBlink == 0) {
            if (sSoundA) {
                sSoundA = 0;
                fn_8018F618(0x363BBFFF, 0);
            }
            sCursor->vf2C(2);
        }
    }
}

void fn_8013DE7C(void) {
    if (!sLoading) {
        return;
    }
    if (fn_80113808() == 0) {
        return;
    }
    for (int i = 0; i < lbl_805FB034; i++) {
        if (lbl_803C4E28[i] == 0) {
            return;
        }
    }
    for (int i = 0; i < 5; i++) {
        fn_8004DCEC(sStands, i);
        if (sStands->mPending[i] != 0) {
            void* model = fn_8004FFB4(sStands->mPending[i], lbl_803F20C4.m4[i]);
            if (model != 0) {
                fn_8004C718(sStands, i, 0);
                fn_8004C718(sStands, i, model);
                if (sStands->mModels[i] != 0) {
                    fn_8004EDD0(sStands->mModels[i], 0, 0);
                    *(u32*)((u8*)sStands->mModels[i] + 0x15C) |= 0x400000;
                }
            }
            sStands->mPending[i] = 0;
        }
    }
    for (int i = 0; i < lbl_805FB034; i++) {
        lbl_803C4E28[i] = 0;
    }
    lbl_805FB034 = 0;
    lbl_805FB038 = 0;
    if (sOpponentCursor != 0) {
        fn_801A3010(sOpponentCursor, sStands);
        sOpponentCursor->vf2C(0x18);
        sOpponentBlink = 5;
    }
    sLoading = 0;
    sActive = 1;
    fn_80190BC8(0x4DD41D3D);
    fn_80190BC8(0x4D2C3770);
    fn_80190BC8(0x7CC42DED);
    fn_80190BC8(0x38569AA8);
}

// NONMATCHING: 99.9 %; after fn_8020EDF0 the DOL builds the monster address for the
// m44 store in r4, this build in r3 (local, inline, member and return type variants
// tried).
void fn_8013E0EC(void) {
    if (sLoadRequest == 1 && !sLoading) {
        fn_80190AFC(0x4DD41D3D);
        fn_80190AFC(0x4D2C3770);
        fn_80190AFC(0x7CC42DED);
        fn_80190AFC(0x38569AA8);
        sD4 = 0;
        void* screens = fn_8018D1E4(lbl_805FB654);
        if (screens != 0) {
            sOpponentCursor = fn_80198F18(screens, 0x6530D136);
            if (sOpponentCursor != 0) {
                fn_801A3010(sOpponentCursor, 0);
                sOpponentCursor->vf30(0x1A);
            }
            if (sStands != 0) {
                for (int i = 0; i < 5; i++) {
                    ReleaseModel(sStands, i);
                }
            }
            sStands = fn_800E876C(0x3E5D2A8E);
            if (sStands != 0) {
                lbl_803F20C4.fn_8020EDF0(&lbl_803F2250);
                lbl_803F20C4.m44 = 0x39;
                fn_8004D760(sStands);
                sStandsState = fn_8004DAF8(sStands, 1, 1);
            }
        }
        sLoadRequest = 0;
        sEC = 0;
        sLoading = 1;
    }
}

void fn_8013E264(u32 on) {
    if (on == 1) {
        fn_80190BC8(0x4DD41D3D);
        fn_80190BC8(0x4D2C3770);
        fn_80190BC8(0x7CC42DED);
    } else {
        fn_80190AFC(0x4DD41D3D);
        fn_80190AFC(0x4D2C3770);
        fn_80190AFC(0x7CC42DED);
    }
}

s32 fn_8013E2D4(void) {
    return sE0;
}

void fn_8013E2DC(void) {
    if (sTeam != 0) {
        for (int i = 0; i < 5; i++) {
            ReleaseModel(sTeam, i);
        }
        sTeam = 0;
    }
    sTeamC4 = 0;
    if (sStands != 0) {
        for (int i = 0; i < 5; i++) {
            ReleaseModel(sStands, i);
        }
        sStands = 0;
    }
    lbl_803F20C4.fn_8020EDAC();
}

void fn_8013E3BC(const char* name) {
    strncpy(sOpponentName, name, 15);
}

void fn_8013E3D0(f32 value) {
    sValue0 = value;
}

void fn_8013E3D8(f32 value) {
    sValue1 = value;
}

void fn_8013E3E0(f32 value) {
    sValue2 = value;
}

void fn_8013E3E8(f32 value) {
    sValue3 = value;
}

char* fn_8013E3F0(void) {
    return sOpponentName;
}

f32 fn_8013E3FC(void) {
    return sValue0;
}

f32 fn_8013E404(void) {
    return sValue1;
}

f32 fn_8013E40C(void) {
    return sValue2;
}

f32 fn_8013E414(void) {
    return sValue3;
}

void fn_8013E41C(s32 count) {
    sCount0 = count;
}

void fn_8013E424(s32 count) {
    sCount1 = count;
}

void fn_8013E42C(u8 active) {
    sActive = active;
}

void fn_8013E434(s32 mode) {
    sMode = mode;
}

void fn_8013E43C(u8 confirmed) {
    sConfirmed = confirmed;
}

u8 fn_8013E444(void) {
    return sConfirmed;
}

UnkStands* fn_8013E44C(void) {
    return sStands;
}

void fn_8013E454(UnkTeamData* team) {
    sTeamC4 = team;
}

void fn_8013E45C(u8 request) {
    sLoadRequest = request;
}

u8 fn_8013E464(void) {
    return sLoading;
}
}

