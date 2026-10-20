// Game unit at 0x8011C460-0x8011D1C0 (real file name unknown): the Badges section of
// the Journal (RTTI). Lists the owned badges with their names, descriptions and icons
// ("ui/msa_badges/inventory_badge_%03d", loaded in the background), counts the badges
// of each category, filters them by category and pages through them with a progress
// bar. Bounds come from the static initializers of this unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/Journal.h"
#include "game/externs.h"
#include "game/unk_text.h"
#include "game/unk_vec3.h"

#include <string.h>

// Badge description (0x18 bytes, real type unknown).
struct UnkBadgeInfo {
    /* 0x00 */ int mIndex;
    /* 0x04 */ u32 mNameHash;
    /* 0x08 */ u32 mDescHash;
    /* 0x0C */ int mCategory;
    /* 0x10 */ u32 m10;
    /* 0x14 */ s16 mIcon;
};

// Badge category description (real type unknown).
struct UnkBadgeCategory {
    /* 0x0 */ u32 m0;
    /* 0x4 */ u32 mNameHash;
};

struct UnkBadgeData {
    /* 0x0000 */ u8 unk0[0x10A8];
    /* 0x10A8 */ UnkBadgeInfo* mBadges;
    /* 0x10AC */ int mBadgeCount;
    /* 0x10B0 */ u8 unk10B0[0x14D0 - 0x10B0];
    /* 0x14D0 */ UnkBadgeCategory* mCategories;
};

struct UnkBadgeDatabase {
    /* 0x0 */ u8 unk0[0xC];
    /* 0xC */ UnkBadgeData* mData;
};

extern "C" {
extern void* lbl_805FBBE0;
extern UnkBadgeDatabase* lbl_805FBBC0;

// Whether the player owns the badge (real name unknown).
int fn_80223308(void* quests, int index);
// Background texture loads (unit 0x801091EC).
UnkTexture* fn_801094F4(const char* name, int arg);
int fn_80109600(UnkTexture** request);
int fn_801098DC(UnkTexture* request);
int fn_80109A0C(UnkTexture* request);
void fn_80184028(UnkTexture* texture);
}

const char* lbl_805F8F50 = "ui/msa_badges/";

// Set when a background icon load finished (real name unknown).
u8 lbl_805FB2A8;

// Text at index 0x43F of the main table (real name unknown).
inline void UnkBadge::Init(const UnkBadgeInfo& info) {
    mNameHash = info.mNameHash;
    mName = GetText(info.mNameHash);
    mDescHash = info.mDescHash;
    mDesc = GetText(info.mDescHash);
    mCategory = info.mCategory;
    mRequest = Badges::fn_8011CC5C(info.mIndex);
    if (fn_80109A0C(mRequest)) {
        mTexture = mRequest;
        mRequest = 0;
    }
}

static inline const char* GetTextByIndex43F() {
    UnkTextTable* table = lbl_805FB910;
    if (table->mTexts != 0 && table->mCount > 0x43F) {
        return table->mTexts[0x43F];
    }
    return lbl_805F8200;
}

static inline void ReleaseTexture(UnkTexture* texture) {
    int count = --texture->mRefCount;
    if (count <= 0 && count == 0) {
        fn_80184028(texture);
    }
}

// With -inline auto,deferred (configure.py; fn_8011CC5C is inlined into vf0C although
// it is defined after it) the compiler emits the functions in reverse order of
// definition, so they are defined here from the last to the first.

const char* Badges::fn_8011D150() {
    fn_80142EF8(mCountText, "%d/%d", mCounts[mCategory].mOwned, mCounts[mCategory].mTotal);
    return mCountText;
}

const char* Badges::fn_8011D038() {
    if (mCategory < 3) {
        UnkBadgeCategory category = lbl_805FBBC0->mData->mCategories[mCategory];
        return GetText(category.mNameHash);
    }
    return GetTextByIndex43F();
}

void Badges::fn_8011D030(u32 hash) {
    mProgress.mBarHash = hash;
}

void Badges::fn_8011D028(u32 topHash, u32 bottomHash) {
    mProgress.fn_80116D40(topHash, bottomHash);
}

const char* Badges::fn_8011CFF4(int index) {
    int i = mCursor + index;
    if (i >= 0 && i < mShownCount) {
        return mShown[i]->mDesc;
    }
    return 0;
}

const char* Badges::fn_8011CFC0(int index) {
    int i = mCursor + index;
    if (i >= 0 && i < mShownCount) {
        return mShown[i]->mName;
    }
    return 0;
}

UnkTexture* Badges::fn_8011CF8C(int index) {
    int i = mCursor + index;
    if (i >= 0 && i < mShownCount) {
        return mShown[i]->mTexture;
    }
    return 0;
}

int Badges::fn_8011CF64(int index) {
    int i = mCursor + index;
    if (i >= 0 && i < mShownCount) {
        return 1;
    }
    return 0;
}

void Badges::fn_8011CE04(int delta) {
    int category = mCategory + delta;
    while (category < 0) {
        category += 4;
    }
    if (category >= 4) {
        category %= 4;
    }
    SetCategory(category);
}

void Badges::fn_8011CDD4(int rows) {
    int cursor = mCursor + rows * 3;
    if (cursor < 0) {
        return;
    }
    if (cursor >= mShownCount) {
        return;
    }
    mCursor = cursor;
    mProgress.fn_80116EF0(cursor);
}

void Badges::fn_8011CD18() {
    mCursor = 0;
    memset(mShown, 0, sizeof(mShown));
    mShownCount = 0;
    for (int i = 0; i < mCount; i++) {
        UnkBadge* badge = &mBadges[i];
        if (mCategory == badge->mCategory || mCategory == 3) {
            mShown[mShownCount] = badge;
            mShownCount++;
        }
    }
    mProgress.fn_80116E54(3, mShownCount);
    mProgress.fn_80116EF0(mCursor);
}

u8 Badges::fn_8011CD10() {
    return lbl_805FB2A8;
}

UnkTexture* Badges::fn_8011CC5C(int index) {
    // Real size unknown.
    char name[0xFF];
    UnkBadgeInfo info = lbl_805FBBC0->mData->mBadges[index];
    fn_80142EF8(name, "%sinventory_badge_%03d", lbl_805F8F50, info.mIcon);
    UnkTexture* texture = fn_801094F4(name, 0);
    if (texture == 0) {
        fn_80142EF8(name, "%sinventory_badge_placeholder", lbl_805F8F50);
        texture = fn_801094F4(name, 0);
    }
    return texture;
}

void Badges::vf18() {}

void Badges::vf14() {
    lbl_805FB2A8 = 0;
    for (int i = 0; i < mCount; i++) {
        if (mBadges[i].mRequest != 0 && !fn_80109A0C(mBadges[i].mRequest) &&
            fn_80109600(&mBadges[i].mRequest)) {
            mBadges[i].mTexture = mBadges[i].mRequest;
            mBadges[i].mRequest = 0;
            lbl_805FB2A8 = 1;
        }
    }
}

void Badges::vf10() {
    for (int i = 0; i < mCount; i++) {
        if (mBadges[i].mTexture != 0) {
            ReleaseTexture(mBadges[i].mTexture);
            mBadges[i].mTexture = 0;
        }
        if (mBadges[i].mRequest != 0) {
            if (!fn_80109A0C(mBadges[i].mRequest)) {
                fn_801098DC(mBadges[i].mRequest);
            }
            mBadges[i].mRequest = 0;
        }
    }
}

void Badges::vf0C() {
    mCount = 0;
    m4 = 3;
    m8 = 4;
    SetCategory(3);
    mCounts[0].mTotal = 0;
    mCounts[0].mOwned = 0;
    mCounts[1].mTotal = 0;
    mCounts[1].mOwned = 0;
    mCounts[2].mTotal = 0;
    mCounts[2].mOwned = 0;
    mCounts[3].mTotal = 0;
    mCounts[3].mOwned = 0;
    for (int i = 0; i < lbl_805FBBC0->mData->mBadgeCount; i++) {
        UnkBadgeInfo info = lbl_805FBBC0->mData->mBadges[i];
        if (fn_80223308(lbl_805FBBE0, i)) {
            mBadges[mCount].Init(info);
            mCount++;
            mCounts[info.mCategory].mOwned++;
            mCounts[3].mOwned++;
        }
        mCounts[info.mCategory].mTotal++;
        mCounts[3].mTotal++;
    }
    lbl_805FB2A8 = 0;
}

Badges::~Badges() {}

Badges::Badges() : mShownCount(0), mCursor(0), mCount(0) {
    for (int i = 0; i < 153; i++) {
        mShown[i] = 0;
    }
}
