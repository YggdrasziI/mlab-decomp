#ifndef GAME_UNK_TEXT_H
#define GAME_UNK_TEXT_H

// Localized text lookup (real names unknown). fn_80120D34 maps a text hash to an
// index; indices with the top bit set use the table at lbl_805FB90C, the others the
// table at lbl_805FB910. The strings returned on failure are emitted by another unit.

#include "types.h"

struct UnkTextTable {
    /* 0x0 */ const char** mTexts;
    /* 0x4 */ u8 unk4[0xC - 0x4];
    /* 0xC */ u32 mCount;
};

int fn_80120D34(u32 hash);

extern "C" {
extern UnkTextTable* lbl_805FB90C;
extern UnkTextTable* lbl_805FB910;
// "", "Not found" and "NOT FOUND".
extern char lbl_805F8200[8];
extern char lbl_80354800[];
extern char lbl_8035480C[];
}

static inline const char* GetText(u32 hash) {
    u32 index = fn_80120D34(hash);
    if (index == 0xFFFFFFFF) {
        return lbl_80354800;
    }
    if ((index & 0x80000000) && lbl_805FB90C != 0) {
        UnkTextTable* table = lbl_805FB90C;
        if (table->mTexts != 0 && (index & 0x7FFFFFFF) < table->mCount) {
            return table->mTexts[index & 0x7FFFFFFF];
        }
        return lbl_805F8200;
    }
    UnkTextTable* table = lbl_805FB910;
    if (table->mTexts != 0 && index < table->mCount) {
        return table->mTexts[index];
    }
    if (index == 0xFFFFFFFF) {
        return lbl_8035480C;
    }
    return lbl_805F8200;
}

#endif // GAME_UNK_TEXT_H
