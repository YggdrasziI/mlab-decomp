// Game unit at 0x8013EB1C-0x8013F034 (real file name unknown): the name entry
// screen. Keeps up to 15 typed characters, shows them in the text widget of the
// screen 0x68B693B2 and moves the cursor actor after the text; the confirmed name is
// copied into the save data. Bounds come from the static initializers of this unit
// and the previous one.

#include "game/unk_vec3.mch"
#include "game/Entity.h"
#include "game/unk_actor.h"
#include "game/unk_text.h"
#include "game/unk_vec3.h"

// Typed name.
static char lbl_803F2230[16];

// Widget showing the typed name (real type unknown).
struct UnkTextWidget {
    /* 0x000 */ u8 unk0[0x17C];
    /* 0x17C */ UnkMtx34 mMtx;
    /* 0x1AC */ u8 unk1AC[0x1D4 - 0x1AC];
    /* 0x1D4 */ UnkVec3 mTextEnd;
};

// Save data (real type unknown).
struct UnkSaveData {
    /* 0x000 */ u8 unk0[0xB40];
    /* 0xB40 */ char mName[16];
};

struct UnkScreenSet {
    /* 0x00 */ u8 unk0[0x88];
    /* 0x88 */ void* mScreens;
};

extern "C" {
extern UnkScreenSet* lbl_805FB654;
extern UnkSaveData lbl_80556E80;

void* fn_80198E34(void* screens, u32 hash);
void* fn_80199AEC(void* screen, u32 hash);
void fn_8019EFC8(void* widget, const char* text, int arg);
void fn_8000C7F4(u32 hash, int arg1, int arg2);
void fn_80123A88(void);
char* strcpy(char* dst, const char* src);
}


extern "C" {
u32 lbl_805FB408;
int lbl_805FB40C;
}

// Rest position of the cursor, taken from the cursor actor on first use.
static f32 lbl_805F90C0 = -1.0f;
static f32 lbl_805F90C4 = -1.0f;
static f32 lbl_805F90C8 = -1.0f;

static inline void UpdateText() {
    void* screens = lbl_805FB654->mScreens;
    if (screens != 0) {
        void* screen = fn_80198E34(screens, 0x68B693B2);
        if (screen != 0) {
            void* text = fn_80199AEC(screen, 0x68B693B2);
            if (text != 0) {
                fn_8019EFC8(text, lbl_803F2230, 1);
            }
        }
    }
}

extern "C" {

int fn_8013EB1C(void) {
    return lbl_80556E80.mName[0] != 0;
}

void fn_8013EB3C(void) {
    lbl_803F2230[0] = 0;
    lbl_805FB408 = 0;
    void* screens = lbl_805FB654->mScreens;
    if (screens != 0) {
        void* screen = fn_80198E34(screens, 0x68B693B2);
        if (screen != 0) {
            void* title = fn_80199AEC(screen, 0x2D6ECFDE);
            UnkActor* cursor = (UnkActor*)fn_80199AEC(screen, 0xC0E2E2AB);
            if (title != 0 && cursor != 0) {
                if (-1.0f == lbl_805F90C0) {
                    lbl_805F90C0 = cursor->mPos.x;
                    lbl_805F90C4 = cursor->mPos.y;
                    lbl_805F90C8 = cursor->mPos.z;
                }
                UnkTextTable* table = lbl_805FB910;
                const char* str;
                if (table->mTexts != 0 && table->mCount > 0x5A4) {
                    str = table->mTexts[0x5A4];
                } else {
                    str = lbl_805F8200;
                }
                fn_8019EFC8(title, str, 1);
            }
            void* text = fn_80199AEC(screen, 0x68B693B2);
            if (text != 0) {
                fn_8019EFC8(text, "", 1);
            }
        }
    }
    lbl_805FB40C = 1;
}

void fn_8013EC58(void) {
    lbl_805FB40C = 0;
}

void fn_8013EC64(void) {
    if (lbl_803F2230[0] == 0) {
        fn_8000C7F4(0x68F56B4B, 0, 0);
    } else {
        strcpy(lbl_80556E80.mName, lbl_803F2230);
        fn_80123A88();
        fn_8000C7F4(0x72C954E3, 0, 0);
    }
}

void fn_8013ECD4(const char* c) {
    if (lbl_805FB408 < 15) {
        lbl_803F2230[lbl_805FB408] = *c;
        lbl_805FB408++;
        lbl_803F2230[lbl_805FB408] = 0;
        UpdateText();
    }
}

void fn_8013ED6C(void) {
    if (lbl_805FB408 != 0) {
        lbl_803F2230[--lbl_805FB408] = 0;
        UpdateText();
    }
}

void fn_8013EDFC(void) {
    void* screens = lbl_805FB654->mScreens;
    if (screens != 0) {
        void* screen = fn_80198E34(screens, 0x68B693B2);
        if (screen != 0) {
            UnkTextWidget* text = (UnkTextWidget*)fn_80199AEC(screen, 0x68B693B2);
            UnkActor* cursor = (UnkActor*)fn_80199AEC(screen, 0xC0E2E2AB);
            if (cursor != 0 && text != 0) {
                if (lbl_803F2230[0] == 0) {
                    cursor->mPos = UnkVec3(lbl_805F90C0, lbl_805F90C4, lbl_805F90C8);
                } else {
                    UnkVec3 end;
                    text->mMtx.Transform(text->mTextEnd, &end);
                    cursor->mPos = UnkVec3(end.x + 2.0f, lbl_805F90C4, lbl_805F90C8);
                }
            }
        }
    }
}

int fn_8013EF6C(void) {
    return lbl_805FB40C;
}

void fn_8013EF74(void) {
    if (lbl_805FB408 != 0 && lbl_805FB408 < 15) {
        lbl_803F2230[lbl_805FB408] = ' ';
        lbl_805FB408++;
        lbl_803F2230[lbl_805FB408] = 0;
        UpdateText();
    }
}
}
