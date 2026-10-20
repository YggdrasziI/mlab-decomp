// Game unit at 0x8011D5C4-0x8011EA20 (real file name unknown): the Statistics section
// of the Journal (RTTI). Four tabs: the player's rank and its progress, counters, and
// three lists (best times, 12 entries over two pages, 6 entries) sorted by name and
// written into ten rows of the interface. Bounds come from the static initializers of
// this unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/Journal.h"
#include "game/unk_text.h"
#include "game/unk_vec3.h"

// Declared here, not through <string.h>: the DOL reloads the compared rows after each
// call, as for strlen in unit_80121508.
extern "C" int strcmp(const char* a, const char* b);

// Player statistics (lbl_805FBBE0, real type unknown).
struct UnkPlayerStats {
    u32 GetBestTime(int index) {
        if (index < 0 || index >= 10) {
            return 0;
        }
        return mBestTimes[index];
    }
    u32 GetA04(int index) {
        if (index < 0 || index >= 6) {
            return 0;
        }
        return mA04[index];
    }

    /* 0x000 */ u8 unk0[0x128];
    /* 0x128 */ u32 mRank;
    /* 0x12C */ u32 m12C;
    /* 0x130 */ u8 unk130[0x8EC - 0x130];
    /* 0x8EC */ u32 m8EC;
    /* 0x8F0 */ u32 m8F0;
    /* 0x8F4 */ u32 m8F4;
    /* 0x8F8 */ u8 unk8F8[0x908 - 0x8F8];
    /* 0x908 */ u32 m908;
    /* 0x90C */ u32 m90C;
    /* 0x910 */ u8 unk910[0x9A8 - 0x910];
    /* 0x9A8 */ u32 mBestTimes[10];
    /* 0x9D0 */ u8 unk9D0[0xA04 - 0x9D0];
    /* 0xA04 */ u32 mA04[6];
};

// Rank description (0x10 bytes, real type unknown).
struct UnkRankInfo {
    /* 0x0 */ u32 m0;
    /* 0x4 */ u32 mNameHash;
    /* 0x8 */ f32 mThreshold;
    /* 0xC */ s16 mC;
};

struct UnkStatInfoC {
    /* 0x0 */ u32 m0;
    /* 0x4 */ u32 mNameHash;
    /* 0x8 */ u32 m8;
};

struct UnkStatInfo8 {
    /* 0x0 */ u32 m0;
    /* 0x4 */ u32 mNameHash;
};

struct UnkStatInfo4C {
    /* 0x00 */ u32 m0;
    /* 0x04 */ u32 mNameHash;
    /* 0x08 */ u8 unk8[0x4C - 0x8];
};

struct UnkFloatPair {
    /* 0x0 */ u32 m0;
    /* 0x4 */ f32 m4;
};

struct UnkStatsObject {
    /* 0x000 */ u8 unk0[0x190];
    /* 0x190 */ UnkFloatPair m190;
};

struct UnkStatsData {
    /* 0x0000 */ u8 unk0[0x3E60];
    /* 0x3E60 */ UnkStatInfoC* m3E60;
    /* 0x3E64 */ u8 unk3E64[0x63C8 - 0x3E64];
    /* 0x63C8 */ UnkRankInfo* mRanks;
    /* 0x63CC */ u8 unk63CC[0x7040 - 0x63CC];
    /* 0x7040 */ UnkStatInfo8* m7040;
    /* 0x7044 */ u8 unk7044[0x80E0 - 0x7044];
    /* 0x80E0 */ UnkStatInfo4C* m80E0;
    /* 0x80E4 */ u8 unk80E4[0x99D0 - 0x80E4];
    /* 0x99D0 */ UnkStatsObject* m99D0;
};

struct UnkStatsDatabase {
    /* 0x0 */ u8 unk0[0xC];
    /* 0xC */ UnkStatsData* mData;
};

extern "C" {
extern UnkPlayerStats* lbl_805FBBE0;
extern UnkStatsDatabase* lbl_805FBBC0;

int fn_80142EF8(char* buf, const char* format, ...);
// Hash of text continued from seed (real name unknown).
u32 fn_80143974(u32 seed, const char* text, int length);
// Interface element by hash: set its text, show or hide it, set a number (real
// names unknown).
void fn_80190E48(u32 hash, const char* text);
void fn_8018F618(u32 hash, int show);
void fn_801911C8(u32 hash, u32 value);
u32 fn_8022CD34(UnkPlayerStats* stats, int index);
}

// Row of a sorted list.
struct UnkStatRow {
    UnkStatRow() {}
    UnkStatRow(const UnkStatRow& other) : mName(other.mName), mValue(other.mValue) {}

    /* 0x0 */ const char* mName;
    /* 0x4 */ u32 mValue;
};

struct UnkSortRange {
    /* 0x0 */ UnkStatRow* mHi;
    /* 0x4 */ UnkStatRow* mLo;
};

static inline bool UnkNameLess(const char* a, const char* b) {
    return strcmp(a, b) < 0;
}

static inline void UnkSwap(UnkStatRow& a, UnkStatRow& b) {
    UnkStatRow tmp = a;
    a = b;
    b = tmp;
}

// The sort is not MSL's std::sort (Petari and the other references have a different
// algorithm); it was rebuilt from the DOL. Sorts the rows by name: quicksort over a stack of ranges down to five rows, then
// an insertion sort behind the smallest of the first five rows.
static inline void UnkSortRows(UnkStatRow* array, int count) {
    if (count <= 1) {
        return;
    }
    UnkStatRow* rows = array;
    if ((u32)count > 4) {
        UnkSortRange stack[32];
        UnkSortRange* sp = stack + 1;
        UnkStatRow* lo = rows;
        UnkStatRow* hi = rows + count - 1;
        while (stack < sp) {
            UnkStatRow* mid = lo + ((hi - lo) >> 1);
            if (UnkNameLess(mid->mName, lo->mName)) {
                UnkSwap(*mid, *lo);
            }
            if (UnkNameLess(hi->mName, mid->mName)) {
                UnkSwap(*mid, *hi);
                if (UnkNameLess(mid->mName, lo->mName)) {
                    UnkSwap(*mid, *lo);
                }
            }
            UnkStatRow* i = lo + 1;
            UnkStatRow* j = hi - 1;
            do {
                const char* key;
                while (UnkNameLess(i->mName, key = mid->mName)) {
                    i++;
                }
                while (UnkNameLess(key, j->mName)) {
                    j--;
                }
                if (i < j) {
                    UnkSwap(*i, *j);
                    if (mid == i) {
                        mid = j;
                    } else if (mid == j) {
                        mid = i;
                    }
                    i++;
                    j--;
                } else if (i == j) {
                    i++;
                    j--;
                    break;
                }
            } while (i <= j);
            int left = j - lo;
            if (left <= 4) {
                if (hi - i <= 4) {
                    sp--;
                    lo = sp->mLo;
                    hi = sp->mHi;
                } else {
                    lo = i;
                }
            } else {
                int right = hi - i;
                if (right <= 4) {
                    hi = j;
                } else if (left > right) {
                    sp->mLo = lo;
                    sp->mHi = j;
                    sp++;
                    lo = i;
                } else {
                    sp->mLo = i;
                    sp->mHi = hi;
                    sp++;
                    hi = j;
                }
            }
        }
    }
    UnkStatRow* last = rows + count - 1;
    UnkStatRow* limit = rows + 4;
    if (limit > last) {
        limit = last;
    }
    UnkStatRow* min = rows;
    for (UnkStatRow* p = rows + 1; p <= limit; p++) {
        if (UnkNameLess(p->mName, min->mName)) {
            min = p;
        }
    }
    if (min != rows) {
        UnkSwap(*min, *rows);
    }
    for (UnkStatRow* p = rows + 1; p <= last; p++) {
        const char* key = p->mName;
        UnkStatRow* q = p - 1;
        while (UnkNameLess(key, q->mName)) {
            q--;
        }
        q++;
        if (q != p) {
            for (UnkStatRow* s = p + 1; --s >= p;) {
                UnkStatRow tmp = *s;
                UnkStatRow* dst = s;
                for (UnkStatRow* src = s; --src >= q; dst = src) {
                    *dst = *src;
                }
                *dst = tmp;
            }
        }
    }
}

Statistics::Statistics() {}

Statistics::~Statistics() {}

void Statistics::vf0C() {
    mTab = 0;
    mPage = 0;
    m8 = 4;
    m4 = 1;
}

void Statistics::vf10() {}

void Statistics::vf14() {}

void Statistics::vf18() {}

void Statistics::fn_8011D69C(int delta) {
    mTab += delta;
    while (mTab < 0) {
        mTab += 4;
    }
    if (mTab >= 4) {
        mTab %= 4;
    }
    m4 = mTab + 1;
}

void Statistics::fn_8011D6F4() {
    mPage = 0;
    mProgress.fn_80116EF0(mPage * 10);
}

void Statistics::fn_8011D708() {
    mPage = 1;
    mProgress.fn_80116EF0(mPage * 10);
}

u32 Statistics::fn_8011D71C() {
    return lbl_805FBBE0->m12C;
}

u32 Statistics::fn_8011D728() {
    u32 next = lbl_805FBBE0->mRank + 1;
    if (next == 50) {
        return 0;
    }
    UnkRankInfo info = lbl_805FBBC0->mData->mRanks[next];
    return info.mThreshold;
}

// NonMatching (99.5 %), like fn_8011D894 (99.1 %) and fn_8011D930 (98.4 %): the DOL
// loads the two globals into the other registers.
const char* Statistics::fn_8011D79C() {
    UnkRankInfo info = lbl_805FBBC0->mData->mRanks[lbl_805FBBE0->mRank];
    return GetText(info.mNameHash);
}

u32 Statistics::fn_8011D894() {
    UnkStatsData* data = lbl_805FBBC0->mData;
    UnkRankInfo info = data->mRanks[lbl_805FBBE0->mRank];
    UnkFloatPair pair = data->m99D0->m190;
    return info.mC * pair.m4;
}

int Statistics::fn_8011D930() {
    UnkRankInfo info = lbl_805FBBC0->mData->mRanks[lbl_805FBBE0->mRank];
    return info.mC;
}

u32 Statistics::fn_8011D97C() {
    return lbl_805FBBE0->m8EC;
}

u32 Statistics::fn_8011D988() {
    return lbl_805FBBE0->m8F0;
}

u32 Statistics::fn_8011D994() {
    return lbl_805FBBE0->m8F4;
}

u32 Statistics::fn_8011D9A0() {
    return 0;
}

u32 Statistics::fn_8011D9A8() {
    return lbl_805FBBE0->m908;
}

u32 Statistics::fn_8011D9B4() {
    return lbl_805FBBE0->m90C;
}

// NonMatching (96.2 %), like fn_8011DF20 (96.0 %) and fn_8011E440 (96.0 %): the
// same code with other register numbers (the loop variables and the sort pointers).
void Statistics::fn_8011D9C0() {
    char text[0x60];
    UnkStatRow rows[10];
    int count = 0;
    for (int i = 0; i < 10; i++) {
        u32 time = lbl_805FBBE0->GetBestTime(i);
        if (time != 0) {
            UnkStatInfo4C info = lbl_805FBBC0->mData->m80E0[i];
            rows[count].mName = GetText(info.mNameHash);
            rows[count].mValue = time;
            count++;
        }
    }
    UnkSortRows(rows, count);
    for (int i = 0; i < 10; i++) {
        int length = fn_80142EF8(text, "%d", i + 1);
        u32 nameId = fn_80143974(0xF9F39258, text, length);
        u32 valueId = fn_80143974(0xEA239A84, text, length);
        if (i < count) {
            fn_80190E48(nameId, rows[i].mName);
            fn_80142EF8(text, "%02d:%02d", rows[i].mValue / 60, rows[i].mValue % 60);
            fn_80190E48(valueId, text);
        } else {
            fn_8018F618(nameId, 0);
            fn_8018F618(valueId, 0);
        }
    }
}

void Statistics::fn_8011DF20() {
    char text[0x20];
    UnkStatRow rows[12];
    int count = 0;
    for (int i = 0; i < 12; i++) {
        u32 value = fn_8022CD34(lbl_805FBBE0, i);
        if (value != 0) {
            UnkStatInfoC info = lbl_805FBBC0->mData->m3E60[i];
            rows[count].mName = GetText(info.mNameHash);
            rows[count].mValue = value;
            count++;
        }
    }
    UnkSortRows(rows, count);
    int index = mPage == 1 ? 10 : 0;
    for (int i = 0; i < 10; i++, index++) {
        int length = fn_80142EF8(text, "%d", i + 1);
        u32 nameId = fn_80143974(0xF9F39258, text, length);
        u32 valueId = fn_80143974(0xEA239A84, text, length);
        if (index < count) {
            fn_80190E48(nameId, rows[index].mName);
            fn_801911C8(valueId, rows[index].mValue);
        } else {
            fn_8018F618(nameId, 0);
            fn_8018F618(valueId, 0);
        }
    }
}

void Statistics::fn_8011E440() {
    char text[0x10];
    UnkStatRow rows[6];
    int count = 0;
    for (int i = 0; i < 6; i++) {
        u32 value = lbl_805FBBE0->GetA04(i);
        if (value != 0) {
            UnkStatInfo8 info = lbl_805FBBC0->mData->m7040[i];
            rows[count].mName = GetText(info.mNameHash);
            rows[count].mValue = value;
            count++;
        }
    }
    UnkSortRows(rows, count);
    for (int i = 0; i < 10; i++) {
        int length = fn_80142EF8(text, "%d", i + 1);
        u32 nameId = fn_80143974(0xF9F39258, text, length);
        u32 valueId = fn_80143974(0xEA239A84, text, length);
        if (i < count) {
            fn_80190E48(nameId, rows[i].mName);
            fn_801911C8(valueId, rows[i].mValue);
        } else {
            fn_8018F618(nameId, 0);
            fn_8018F618(valueId, 0);
        }
    }
}

int Statistics::fn_8011E94C() {
    return mTab == 0;
}

int Statistics::fn_8011E95C() {
    return mTab == 1;
}

int Statistics::fn_8011E970() {
    return mTab == 2;
}

int Statistics::fn_8011E984() {
    return mTab == 3;
}

int Statistics::fn_8011E998() {
    return mPage == 0;
}

void Statistics::fn_8011E9A8(u32 topHash, u32 bottomHash) {
    mProgress.fn_80116D40(topHash, bottomHash);
}

void Statistics::fn_8011E9B0(u32 hash) {
    mProgress.mBarHash = hash;
}

void Statistics::fn_8011E9B8() {
    mProgress.fn_80116E54(10, 12);
    mProgress.fn_80116EF0(mPage * 10);
}
