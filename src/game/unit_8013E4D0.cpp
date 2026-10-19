// Game unit at 0x8013E4D0-0x8013EB1C (real file name unknown): a team screen. Shows four
// values, a name and a "%d/%d" counter in the widgets of the screen 0x2C434515, keeps a
// blinking cursor actor at its rest position and saves the team into a 0x3F record.
// Bounds come from the static initializers of this unit and the previous one.

#include "game/unk_vec3.mch"

#include "game/externs.h"
#include "game/unk_actor.h"
#include "game/unk_vec3.h"

// Shown name.
static char sText[16];

// Team member (real type unknown).
struct UnkMember {
    /* 0x0 */ s32 m0;
    /* 0x4 */ s32 m4;
    /* 0x8 */ s32 m8;
};

// Team (real type unknown).
struct UnkTeam {
    /* 0x0 */ u8 unk0[0x4];
    /* 0x4 */ UnkMember* mMembers[5];
};

// One member slot of a saved record (real type unknown).
struct UnkRecordSlot {
    /* 0x0 */ s32 m0;
    /* 0x4 */ u8 m4;
    /* 0x5 */ u8 m5;
    /* 0x8 */ s32 m8;
    /* 0xC */ s32 mC;
};

// Saved team record (real type unknown).
struct UnkRecord {
    /* 0x00 */ u8 unk0[0x4];
    /* 0x04 */ f32 m4;
    /* 0x08 */ f32 m8;
    /* 0x0C */ f32 mC;
    /* 0x10 */ f32 m10;
    /* 0x14 */ char mName[0x10];
    /* 0x24 */ UnkRecordSlot mSlots[5];
};

// Object at lbl_805FBBE0 (real type unknown).
struct UnkGameData {
    /* 0x0000 */ u8 unk0[0x114];
    /* 0x0114 */ UnkTeam* m114;
    /* 0x0118 */ u8 unk118[0xA30 - 0x118];
    /* 0x0A30 */ UnkTeam* mTeams[(0x10E8 - 0xA30) / 4];
    /* 0x10E8 */ s32 m10E8;
};

extern "C" {
extern void* lbl_805FB654;
extern UnkGameData* lbl_805FBBE0;

void* fn_8018D1E4(void* obj);
void* fn_80198E34(void* screens, u32 hash);
void* fn_80199AEC(void* screen, u32 hash);
void fn_8019EFC8(void* widget, const char* text, int arg);
void fn_801A3FA0(void* widget, f32 value);
void fn_801A3010(void* actor, void* arg);
u32 fn_80140B58(void);
void fn_80140B50(u32 hash);
void fn_80140B9C(int arg);
int fn_8013E2D4(void);
void fn_8013E454(UnkTeam* team);
f32 fn_80113340(void);
f32 fn_801133BC(void);
f32 fn_80113438(void);
f32 fn_801134B4(void);
const char* fn_80113710(void);
int fn_801138F8(void);
void fn_801132BC(int index);
void* fn_801F3918(int type, int arg);
u8 fn_8021EF58(UnkMember* member);
u8 fn_8021EF60(UnkMember* member);
int fn_801409FC(void);
void* fn_800E876C(u32 id);
void* fn_801B7198(void* obj, int arg);
int fn_8004E300(void* obj, int arg);
void fn_8018F618(u32 hash, int arg);
void fn_80190BC8(u32 hash);
void fn_80190AFC(u32 hash);
char* strcpy(char* dst, const char* src);
}

static f32 sValue0;
static f32 sValue1;
static f32 sValue2;
static f32 sValue3;
static s32 sBlink;

static s32 sSelected = -1;

// Rest position of the cursor, taken from the cursor actor on first use.
static f32 sCursorX = -1.0f;
static f32 sCursorY = -1.0f;
static f32 sCursorZ = -1.0f;

// Constant (0, 0, -0.2) in .rodata among other 12-byte vectors used by units all over
// the game, so probably a global defined elsewhere (real name unknown).
extern const UnkVec3 lbl_80346008;

extern "C" {

void fn_8013E4D0(void) {
    if (fn_80140B58() == 0x13E20C39) {
        return;
    }
    sBlink = 0;
    sValue0 = 0.0f;
    sValue1 = 0.0f;
    sValue2 = 0.0f;
    sValue3 = 0.0f;
    sText[0] = 0;
    if (fn_8013E2D4() == 0) {
        sSelected = -1;
    }
    void* screens = fn_8018D1E4(lbl_805FB654);
    if (screens != 0) {
        void* screen = fn_80198E34(screens, 0x2C434515);
        if (screen != 0) {
            UnkActor* cursor = (UnkActor*)fn_80199AEC(screen, 0x60B15D91);
            if (cursor != 0) {
                if (-1.0f == sCursorX && -1.0f == sCursorY && -1.0f == sCursorZ) {
                    sCursorX = cursor->mPos.x;
                    sCursorY = cursor->mPos.y;
                    sCursorZ = cursor->mPos.z;
                }
            }
            void* widget = fn_80199AEC(screen, 0x3E22E495);
            if (widget != 0) {
                sValue0 = fn_80113340();
                fn_801A3FA0(widget, sValue0);
            }
            widget = fn_80199AEC(screen, 0x67E9BE17);
            if (widget != 0) {
                sValue1 = fn_801133BC();
                fn_801A3FA0(widget, sValue1);
            }
            widget = fn_80199AEC(screen, 0xB738EABD);
            if (widget != 0) {
                sValue2 = fn_80113438();
                fn_801A3FA0(widget, sValue2);
            }
            widget = fn_80199AEC(screen, 0xE4E00B6F);
            if (widget != 0) {
                sValue3 = fn_801134B4();
                fn_801A3FA0(widget, sValue3);
            }
            void* name = fn_80199AEC(screen, 0x2EADCD52);
            if (name != 0) {
                fn_80142EF8(sText, "%s", fn_80113710());
                fn_8019EFC8(name, sText, 1);
            }
            void* counter = fn_80199AEC(screen, 0x524A7A53);
            if (counter != 0) {
                char buf[16];
                fn_80142EF8(buf, "%d/%d", fn_801138F8() + 1, lbl_805FBBE0->m10E8);
                fn_8019EFC8(counter, buf, 1);
            }
        }
    }
    fn_80140B50(0x13E20C39);
    fn_80140B9C(1);
}

void fn_8013E734(void) {
    fn_8018F618(0x60B15D91, 0);
}

void fn_8013E744(f32 value) {
    sValue0 = value;
}

void fn_8013E74C(f32 value) {
    sValue1 = value;
}

void fn_8013E754(f32 value) {
    sValue2 = value;
}

void fn_8013E75C(f32 value) {
    sValue3 = value;
}

void fn_8013E764(const char* text) {
    strcpy(sText, text);
}

char* fn_8013E774(void) {
    return sText;
}

void fn_8013E780(void) {
    UnkTeam* team;
    UnkRecord* record = (UnkRecord*)fn_801F3918(0x3F, 0);
    if (record == 0) {
        return;
    }
    strcpy(record->mName, sText);
    record->m4 = sValue0;
    record->m8 = sValue1;
    record->mC = sValue2;
    record->m10 = sValue3;
    int index = fn_801138F8();
    sSelected = index;
    team = lbl_805FBBE0->mTeams[index];
    fn_8013E454(team);
    lbl_805FBBE0->m114 = team;
    for (int i = 0; i < 5; i++) {
        if (team->mMembers[i] == 0) {
            record->mSlots[i].m0 = -1;
            record->mSlots[i].m4 = 0;
            record->mSlots[i].m5 = 0;
            record->mSlots[i].m8 = -1;
            record->mSlots[i].mC = -1;
        } else {
            record->mSlots[i].m0 = team->mMembers[i]->m0;
            record->mSlots[i].m4 = fn_8021EF58(team->mMembers[i]);
            record->mSlots[i].m5 = fn_8021EF60(team->mMembers[i]);
            record->mSlots[i].m8 = team->mMembers[i]->m8;
            record->mSlots[i].mC = team->mMembers[i]->m4;
        }
    }
}

void fn_8013E898(void) {
    if (fn_801409FC() == 0) {
        return;
    }
    void* screens = fn_8018D1E4(lbl_805FB654);
    if (screens == 0) {
        return;
    }
    void* screen = fn_80198E34(screens, 0x2C434515);
    if (screen == 0) {
        return;
    }
    UnkActor* cursor = (UnkActor*)fn_80199AEC(screen, 0x60B15D91);
    if (cursor == 0) {
        return;
    }
    // The DOL tests one compare three times: the original probably checks the x
    // coordinate three times.
    if (-1.0f == sCursorX && -1.0f == sCursorX && -1.0f == sCursorX) {
        sCursorX = cursor->mPos.x;
        sCursorY = cursor->mPos.y;
        sCursorZ = cursor->mPos.z;
    }
    fn_801A3010(cursor, fn_800E876C(0xA9638202));
    UnkVec3 pos(sCursorX, sCursorY, sCursorZ);
    void* target = fn_801B7198((u8*)cursor + 0x140, 0);
    if (target != 0 && fn_8004E300(target, 1) == 13) {
        UnkVec3 offset(lbl_80346008.x, lbl_80346008.y, lbl_80346008.z);
        pos += offset;
    }
    cursor->mPos = UnkVec3(pos);
    cursor->vf2C(0x18);
    if (!(cursor->m64 & 2) && sBlink == 0) {
        sBlink = 3;
    }
    if (sBlink > 0 && --sBlink == 0) {
        fn_8018F618(0x379F79BD, 0);
        cursor->vf2C(2);
    }
}

bool fn_8013EA68(void) {
    return fn_801138F8() == sSelected;
}

void fn_8013EA98(void) {
    s32 selected = sSelected;
    bool same = fn_801138F8() == selected;
    if (!same) {
        fn_801132BC(selected);
    }
}

void fn_8013EADC(u32 on) {
    if (on == 1) {
        fn_80190BC8(0x0A38E5BD);
    } else {
        fn_80190AFC(0x0A38E5BD);
    }
}
}
