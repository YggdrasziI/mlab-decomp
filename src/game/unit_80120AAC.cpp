// Game unit at 0x80120AAC-0x80120D34 (real file name unknown): a 16-character text
// entry buffer (typed characters, backspace, space, trailing-space trim). Bounds
// come from the static initializers of this unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/unk_vec3.h"
#include <string.h>

// Real names unknown.
struct UnkTextEntry {
    /* 0x00 */ char mText[0x10];
    /* 0x10 */ int mLength;
    /* 0x14 */ u8 mLocked;
};

UnkTextEntry* lbl_805FB2B8;

extern void* lbl_805FBBE0;

extern "C" {
void fn_8018EEB4(u32 hash);
void fn_8018F000(u32 hash);
void fn_800E69EC(int enable);
void fn_8018D1FC(const char* format, ...);
void fn_8022E3D8(void* obj, const char* text);
}

static inline void Terminate() {
    lbl_805FB2B8->mText[lbl_805FB2B8->mLength] = 0;
}

extern "C" {

void fn_80120AAC(void) {
    fn_8018EEB4(0x68B693B2);
    lbl_805FB2B8 = (UnkTextEntry*)operator new(sizeof(UnkTextEntry));
    memset(lbl_805FB2B8, 0, 0x10);
    lbl_805FB2B8->mLength = 0;
    lbl_805FB2B8->mLocked = 0;
    fn_800E69EC(1);
}

void fn_80120B08(void) {
    fn_8018F000(0x68B693B2);
    fn_800E69EC(0);
    operator delete(lbl_805FB2B8);
    lbl_805FB2B8 = 0;
}

void fn_80120B48(const char* c) {
    UnkTextEntry* entry = lbl_805FB2B8;
    if (entry->mLocked) {
        return;
    }
    if (entry->mLength == 0 && *c == ' ') {
        return;
    }
    if (entry == 0) {
        return;
    }
    if (*c != 0) {
        if (entry->mLength < 15) {
            lbl_805FB2B8->mText[entry->mLength++] = *c;
            Terminate();
        } else {
            entry->mText[entry->mLength - 1] = *c;
        }
    } else if (entry->mLength > 0) {
        entry->mLength--;
        Terminate();
    }
}

void fn_80120BE8(void) {
    if (!lbl_805FB2B8->mLocked) {
        char c = 0;
        fn_80120B48(&c);
    }
}

void fn_80120C24(void) {
    if (!lbl_805FB2B8->mLocked) {
        char c = ' ';
        fn_80120B48(&c);
    }
}

int fn_80120C60(void) {
    int result = 0;
    if (lbl_805FB2B8->mLength > 0) {
        result = 1;
        while (lbl_805FB2B8->mLength > 1 && lbl_805FB2B8->mText[lbl_805FB2B8->mLength - 1] == ' ') {
            lbl_805FB2B8->mLength--;
            Terminate();
        }
    }
    return result;
}

UnkTextEntry* fn_80120CBC(void) {
    return lbl_805FB2B8;
}

void fn_80120CC4(void) {
    if (lbl_805FB2B8 != 0) {
        fn_8018D1FC(lbl_805FB2B8->mText);
        fn_8022E3D8(lbl_805FBBE0, lbl_805FB2B8->mText);
    }
}

void fn_80120D00(u8 locked) {
    if (lbl_805FB2B8 != 0) {
        lbl_805FB2B8->mLocked = locked;
    }
}

}
