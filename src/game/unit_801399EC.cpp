// Game unit at 0x801399EC-0x8013A03C (real file name unknown): the friend code entry
// screen. Keeps up to 12 typed digits, shows them as "XXXX-XXXX-XXXX" in the text
// widget of the screen 0x7D12C9EE, moves the cursor actor after the text and checks
// the confirmed code against the player's own code and the friend roster. The start
// follows the empty function at 0x801399E8, which the previous unit's static
// initializer uses; the end is this unit's static initializer.

// Typed digits and the text shown. Defined before the includes: the DOL has them
// before the sUnkVec3 of unk_vec3.h in .bss.
static char sDigits[16];
static char sText[16];

#include <revolution/os.h>

#include "game/Entity.h"
#include "game/unk_actor.h"
#include "game/unk_vec3.h"

// Widget showing the typed code (real type unknown).
struct UnkTextWidget {
    /* 0x000 */ u8 unk0[0x17C];
    /* 0x17C */ UnkMtx34 mMtx;
    /* 0x1AC */ u8 unk1AC[0x1D4 - 0x1AC];
    /* 0x1D4 */ UnkVec3 mTextEnd;
};

// Friend roster entry (real type unknown).
struct UnkFriend {
    /* 0x00 */ u64 mCode;
    /* 0x08 */ u8 unk8[0x20 - 0x8];
};

// Save data (real type unknown).
struct UnkSaveData {
    /* 0x000 */ u8 unk0[0x350];
    /* 0x350 */ UnkFriend mFriends[64];
};

// Global manager created on first use (see unit_8010FA48.cpp).
class UnkManager {
public:
    UnkManager();
    static int fn_80209E70(u32 hash, int arg1, int arg2, int arg3, int arg4, int arg5);

    static UnkManager* Instance() {
        if (sInstance == 0) {
            sInstance = new UnkManager;
        }
        return sInstance;
    }

    static UnkManager* sInstance;
};

extern "C" {
extern void* lbl_805FB654;
extern UnkSaveData lbl_80556E80;
extern OSMutex lbl_80556AB0;
extern s32 lbl_805FBB48;

void* fn_8018D1E4(void* obj);
void* fn_80198E34(void* screens, u32 hash);
void* fn_80199AEC(void* screen, u32 hash);
void fn_8019EFC8(void* widget, const char* text, int arg);
void fn_8000C7F4(u32 hash, int arg1, int arg2);
u32 fn_80140B58(void);
void fn_80140B50(u32 hash);
void fn_80140B9C(int arg);
void fn_8013F394(int arg);
void fn_8013F2A4(int arg);
u64 fn_802FB094(UnkSaveData* save);
int fn_802FA5BC(UnkSaveData* save, u64 code);
void fn_80190BC8(u32 hash);
void fn_80190AFC(u32 hash);
int sscanf(const char* str, const char* format, ...);
u32 strlen(const char* str);
}

static u32 sDigitCount;
static u32 sTextLen;
static bool sValid;
static u64 sCode;

// Rest position of the cursor, taken from the cursor actor on first use.
static f32 sCursorX = -1.0f;
static f32 sCursorY = -1.0f;
static f32 sCursorZ = -1.0f;

static inline void LockSave() {
    OSLockMutex(&lbl_80556AB0);
    lbl_805FBB48++;
}

static inline void UnlockSave() {
    OSUnlockMutex(&lbl_80556AB0);
    lbl_805FBB48--;
}

extern "C" {

void fn_801399EC(void) {
    if (fn_80140B58() == 0x89CE136A) {
        return;
    }
    sDigitCount = 0;
    sDigits[0] = 0;
    sTextLen = 0;
    sText[0] = 0;
    sValid = false;
    sCode = 0;
    void* screens = fn_8018D1E4(lbl_805FB654);
    if (screens != 0) {
        void* screen = fn_80198E34(screens, 0x7D12C9EE);
        if (screen != 0) {
            if (-1.0f == sCursorX && -1.0f == sCursorY && -1.0f == sCursorZ) {
                UnkActor* cursor = (UnkActor*)fn_80199AEC(screen, 0x924D5809);
                if (cursor != 0) {
                    sCursorX = cursor->mPos.x;
                    sCursorY = cursor->mPos.y;
                    sCursorZ = cursor->mPos.z;
                }
            }
            void* text = fn_80199AEC(screen, 0x6C2E9B73);
            if (text != 0) {
                fn_8019EFC8(text, "", 1);
            }
        }
    }
    fn_8013F394(1);
    fn_8013F2A4(0);
    fn_80140B50(0x89CE136A);
    fn_80140B9C(1);
}

void fn_80139B0C(void) {}

// NONMATCHING: 99.7 %; in the digit branch the DOL keeps the count in r8 and the
// character in r9, this build the other way round (local, inline and order variants
// tried).
void fn_80139B10(u32 key) {
    UnkManager::Instance()->fn_80209E70(0xCA084594, 0, 0, 0, 0, 0);
    if (key == 10) {
        if (sDigitCount != 0) {
            sDigits[--sDigitCount] = 0;
            if (sText[sTextLen - 1] == '-') {
                sText[--sTextLen] = 0;
            }
            sText[--sTextLen] = 0;
        }
    } else if (key <= 9) {
        if (sDigitCount < 12) {
            char c = "0123456789"[key];
            sDigits[sDigitCount++] = c;
            sDigits[sDigitCount] = 0;
            sText[sTextLen++] = c;
            if (sDigitCount == 4 || sDigitCount == 8) {
                sText[sTextLen] = '-';
                sTextLen++;
            }
            sText[sTextLen] = 0;
        }
    } else {
        return;
    }
    void* screens = fn_8018D1E4(lbl_805FB654);
    if (screens != 0) {
        void* screen = fn_80198E34(screens, 0x7D12C9EE);
        if (screen != 0) {
            void* text = fn_80199AEC(screen, 0x6C2E9B73);
            if (text != 0) {
                fn_8019EFC8(text, sText, 1);
            }
        }
    }
}

void fn_80139CC4(void) {
    void* screens = fn_8018D1E4(lbl_805FB654);
    if (screens != 0) {
        void* screen = fn_80198E34(screens, 0x7D12C9EE);
        if (screen != 0) {
            UnkTextWidget* text = (UnkTextWidget*)fn_80199AEC(screen, 0x6C2E9B73);
            UnkActor* cursor = (UnkActor*)fn_80199AEC(screen, 0x924D5809);
            if (cursor != 0 && text != 0) {
                cursor->vf2C(2);
                if (strlen(sText) == 0) {
                    cursor->mPos = UnkVec3(sCursorX, sCursorY, sCursorZ);
                } else {
                    UnkVec3 end;
                    text->mMtx.Transform(text->mTextEnd, &end);
                    cursor->mPos = UnkVec3(end.x + 2.0f, sCursorY, sCursorZ);
                }
            }
        }
    }
}

void fn_80139E54(void) {
    if (sDigitCount != 12) {
        fn_8000C7F4(0x94B1685F, 0, 0);
        return;
    }
    LockSave();
    u64 code;
    sscanf(sDigits, "%012llu", &code);
    if (code == fn_802FB094(&lbl_80556E80) || !fn_802FA5BC(&lbl_80556E80, code)) {
        fn_8000C7F4(0x94B1685F, 0, 0);
    } else {
        for (int i = 0; i < 64; i++) {
            if (lbl_80556E80.mFriends[i].mCode == code) {
                fn_8000C7F4(0xF719853A, 0, 0);
                UnlockSave();
                return;
            }
        }
        sValid = true;
        sCode = code;
        fn_8000C7F4(0xD919921E, 0, 0);
    }
    UnlockSave();
}

void fn_80139FD0(u32 on) {
    if (on == 1) {
        fn_80190BC8(0x91198EF5);
    } else {
        fn_80190AFC(0x91198EF5);
    }
}

bool fn_80139FF0(void) {
    return sValid == true;
}

u64 fn_8013A004(void) {
    return sCode;
}

char* fn_8013A010(void) {
    return sText;
}
}
