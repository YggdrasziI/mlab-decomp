// Game unit at 0x80051EEC-0x8005230C (real file name unknown): text providers that
// return a string for the text system (line breaks, a space, the current item's
// name and value, counters and the build string). Bounds come from the static
// initializers of this unit and the previous one.

#include "game/unk_vec3.h"
#include "types.h"

// Entry of the item table (0x10 bytes, real type unknown).
struct UnkItemEntry {
    /* 0x0 */ u32 m0;
    /* 0x4 */ u32 mNameHash;
    /* 0x8 */ u8 unk8[3];
    /* 0xB */ s8 mValue;
    /* 0xC */ u32 mC;
};

struct UnkItemData {
    /* 0x0000 */ u8 unk0[0x4F00];
    /* 0x4F00 */ UnkItemEntry* mEntries;
};

struct UnkItemDatabase {
    /* 0x0 */ u8 unk0[0xC];
    /* 0xC */ UnkItemData* mData;
};

// Localized text table (real type unknown).
struct UnkTextTable {
    /* 0x0 */ const char** mTexts;
    /* 0x4 */ u8 unk4[0xC - 0x4];
    /* 0xC */ u32 mCount;
};

// Object at lbl_805FBBC8 (real type unknown).
struct UnkCounterOwner {
    /* 0x00 */ u8 unk0[0x68];
    /* 0x68 */ int m68;
};

// Object at lbl_805FBBE0 (real type unknown).
struct UnkSelection {
    /* 0x000 */ u8 unk0[0x128];
    /* 0x128 */ int m128;
};

int fn_80120D34(u32 hash);
void fn_8001426C(char* buf);

extern "C" {
extern UnkSelection* lbl_805FBBE0;
extern UnkItemDatabase* lbl_805FBBC0;
extern UnkCounterOwner* lbl_805FBBC8;
extern UnkTextTable* lbl_805FB90C;
extern UnkTextTable* lbl_805FB910;
// Strings emitted by another unit ("", "Not found", "NOT FOUND").
extern char lbl_805F8200[8];
extern char lbl_80354800[];
extern char lbl_8035480C[];

const char* fn_8022E3DC(UnkSelection* selection);
const char* fn_8022E170(UnkSelection* selection);
int fn_8022E438(UnkSelection* selection);
int fn_8022E430(UnkSelection* selection);
void fn_8021C968(const char* src, char* dst, int size);
int fn_8012341C(void);
int fn_80142EF8(char* buf, const char* format, ...);
void* memset(void* dst, int value, u32 size);
char* itoa(int value, char* str, int base);
}

static char lbl_803C53E8[0x80];

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

extern "C" {

const char* fn_80051EEC(void) {
    return "\n";
}

const char* fn_80051EF4(void) {
    return "\r";
}

const char* fn_80051EFC(void) {
    return fn_8022E3DC(lbl_805FBBE0);
}

const char* fn_80051F04(void) {
    fn_8021C968(fn_8022E170(lbl_805FBBE0), lbl_803C53E8, 0x20);
    return lbl_803C53E8;
}

const char* fn_80051F44(void) {
    const char* text = " ";
    int index = fn_8022E438(lbl_805FBBE0);
    if (index != -1) {
        UnkItemEntry entry = lbl_805FBBC0->mData->mEntries[index];
        text = GetText(entry.mNameHash);
    }
    return text;
}

const char* fn_80052054(void) {
    memset(lbl_803C53E8, 0, sizeof(lbl_803C53E8));
    int index = fn_8022E438(lbl_805FBBE0);
    if (index != -1) {
        UnkItemEntry entry = lbl_805FBBC0->mData->mEntries[index];
        fn_80142EF8(lbl_803C53E8, "%d", entry.mValue);
    }
    return lbl_803C53E8;
}

const char* fn_800520F0(void) {
    const char* text = " ";
    int index = fn_8022E430(lbl_805FBBE0);
    if (index != -1) {
        UnkItemEntry entry = lbl_805FBBC0->mData->mEntries[index];
        text = GetText(entry.mNameHash);
    }
    return text;
}

const char* fn_80052200(void) {
    memset(lbl_803C53E8, 0, sizeof(lbl_803C53E8));
    itoa(fn_8012341C(), lbl_803C53E8, 10);
    return lbl_803C53E8;
}

// NonMatching (99.3 %): two registers swapped in the subtraction.
const char* fn_8005224C(void) {
    memset(lbl_803C53E8, 0, sizeof(lbl_803C53E8));
    int count = 0;
    if (lbl_805FBBC8 != 0) {
        count = lbl_805FBBC8->m68 - lbl_805FBBE0->m128;
    }
    itoa(count, lbl_803C53E8, 10);
    return lbl_803C53E8;
}

const char* fn_800522B8(void) {
    fn_8001426C(lbl_803C53E8);
    return lbl_803C53E8;
}
}
