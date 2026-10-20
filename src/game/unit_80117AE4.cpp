// Game unit at 0x80117AE4-0x801196F4 (real file name unknown): the Ingredients section
// of the InventoryBook (RTTI). Lists the ingredients or parts the player owns (one tab
// each), with names, counts and icons ("ui/msa_ingredients/inventory_icon_%03d",
// "..._sec_%03d" for parts) loaded in the background, filtered by type and paged three
// rows of four at a time. Also the icon loaders used by the Recipes section. Bounds
// come from the static initializers of this unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/InventoryBook.h"
#include "game/Journal.h"
#include "game/unk_text.h"
#include "game/unk_vec3.h"

#include <string.h>

// Inventory entry of the player (0xC bytes, real type unknown).
struct UnkStatItem {
    /* 0x0 */ int mKind; // 0: ingredient, 1: part
    /* 0x4 */ int mIndex;
    /* 0x8 */ u8 mCount;
};

struct UnkInventoryStats {
    /* 0x000 */ u8 unk0[0xA30];
    /* 0xA30 */ u8 mA30[0xBE8 - 0xA30];
    /* 0xBE8 */ UnkStatItem mItems[105];
};

// Ingredient description (0x10 bytes, real type unknown).
struct UnkIngredientInfo {
    /* 0x0 */ int mId;
    /* 0x4 */ u32 mNameHash;
    /* 0x8 */ u8 m8;
    /* 0x9 */ u8 m9;
    /* 0xA */ s8 mIcon;
    /* 0xB */ u8 mB;
    /* 0xC */ u32 mC;
};

// Part description (0x18 bytes, real type unknown).
struct UnkPartInfo {
    /* 0x00 */ u32 mId;
    /* 0x04 */ u32 mNameHash;
    /* 0x08 */ int mKey;
    /* 0x0C */ int mType;
    /* 0x10 */ s16 m10;
    /* 0x12 */ s8 mIcon;
    /* 0x13 */ s8 mSlot;
    /* 0x14 */ u8 m14;
    /* 0x15 */ u8 m15;
};

// Breed description (0x24 bytes, real type unknown).
struct UnkBreedInfo {
    /* 0x00 */ u32 mId;
    /* 0x04 */ u32 m4;
    /* 0x08 */ int mType;
    /* 0x0C */ int mGroup;
    /* 0x10 */ int mIngredient;
    /* 0x14 */ int mPartKey;
    /* 0x18 */ u32 m18;
    /* 0x1C */ u32 m1C;
    /* 0x20 */ int mDescId;
};

struct UnkTypeInfo {
    /* 0x0 */ u32 m0;
    /* 0x4 */ u32 m4;
    /* 0x8 */ int mTypeFilter;
};

struct UnkInventoryData {
    /* 0x0000 */ u8 unk0[0x3E60];
    /* 0x3E60 */ UnkTypeInfo* mTypes;
    /* 0x3E64 */ u8 unk3E64[0x4F00 - 0x3E64];
    /* 0x4F00 */ UnkIngredientInfo* mIngredients;
    /* 0x4F04 */ u8 unk4F04[0x5328 - 0x4F04];
    /* 0x5328 */ UnkPartInfo* mParts;
    /* 0x532C */ u8 unk532C[0x6C18 - 0x532C];
    /* 0x6C18 */ UnkBreedInfo* mBreeds;
};

struct UnkInventoryDatabase {
    /* 0x0 */ u8 unk0[0xC];
    /* 0xC */ UnkInventoryData* mData;
};

extern "C" {
extern UnkInventoryStats* lbl_805FBBE0;
extern UnkInventoryDatabase* lbl_805FBBC0;
extern void* lbl_805FB654;

int fn_80142EF8(char* buf, const char* format, ...);
int fn_8020E4E8(u32 hash);
void fn_80219ED4(void* selection, int index, int kind, int arg);
void* fn_8018D1E4(void* obj);
void* fn_80198F18(void* set, u32 hash);
void* fn_801A1208(void* actor);
// Texture loads (unit 0x801091EC).
UnkTexture* fn_80109330(const char* name);
UnkTexture* fn_801094F4(const char* name, int arg);
int fn_80109600(UnkTexture** request);
int fn_801098DC(UnkTexture* request);
int fn_80109A0C(UnkTexture* request);
void fn_80184028(UnkTexture* texture);
// Icons of an ingredient and of a part, loaded in the background (also called by the
// Recipes section by these names).
UnkTexture* fn_801189C4(int ingredient);
UnkTexture* fn_80118B1C(int part);
}

const char* lbl_805F8F10 = "ui/msa_ingredients/";

// Set when a background icon load finished (real name unknown).
u8 lbl_805FB260;

static inline void ReleaseTexture(UnkTexture* texture) {
    int count = --texture->mRefCount;
    if (count <= 0 && count == 0) {
        fn_80184028(texture);
    }
}

// Text at a fixed index of the main table (real name unknown).
static inline const char* GetTextByIndex(u32 index) {
    UnkTextTable* table = lbl_805FB910;
    if (table->mTexts != 0 && table->mCount > index) {
        return table->mTexts[index];
    }
    return lbl_805F8200;
}

// Clamp between two bounds given in any order.
static inline int UnkClampBetween(int value, int a, int b) {
    return a < b ? (value < a ? a : (value > b ? b : value))
                 : (value < b ? b : (value > a ? a : value));
}

inline void Ingredients::ReleaseIcons() {
    for (int i = 0; i < mCount; i++) {
        if (mItems[i].mIcon != 0) {
            ReleaseTexture(mItems[i].mIcon);
            mItems[i].mIcon = 0;
        }
        if (mItems[i].mRequest != 0) {
            if (!fn_80109A0C(mItems[i].mRequest)) {
                fn_801098DC(mItems[i].mRequest);
            } else {
                ReleaseTexture(mItems[i].mRequest);
            }
            mItems[i].mRequest = 0;
        }
    }
}

inline void Ingredients::ScrollToRow(int row) {
    if (row < mPage) {
        mPage = row;
    } else if (row >= mPage + 3) {
        while (row >= mPage + 3) {
            mPage++;
        }
    }
}

// Built with -inline auto,deferred (configure.py): fn_80118858 is inlined into
// fn_8011813C although it is defined after it, and the compiler then emits the
// functions in reverse order of definition, so they are defined here from the last to
// the first.

UnkTexture* Ingredients::fn_801196A8() {
    if (mCursor >= 0 && mCursor < 117) {
        return mShownIcons[mCursor];
    }
    return 0;
}

const char* Ingredients::fn_8011966C() {
    UnkInventoryItem* item = 0;
    if (mCursor >= 0 && mCursor < 117) {
        item = mShown[mCursor];
    }
    if (item != 0) {
        return item->mName;
    }
    return 0;
}

void** Ingredients::fn_80119658() {
    return &mShownMarks[mPage * 4];
}

char** Ingredients::fn_80119644() {
    return &mShownTexts[mPage * 4];
}

UnkTexture** Ingredients::fn_80119630() {
    return &mShownIcons[mPage * 4];
}

int Ingredients::fn_80119600(int index) {
    int i = index + mPage * 4;
    if (i < mShownCount && i >= 0) {
        return 1;
    }
    return 0;
}

void Ingredients::fn_801195F8(u32 hash) {
    mProgress.mBarHash = hash;
}

void Ingredients::fn_801195F0(u32 topHash, u32 bottomHash) {
    mProgress.fn_80116D40(topHash, bottomHash);
}

void Ingredients::fn_8011954C(int delta) {
    if (mShownCount <= 0) {
        return;
    }
    mCursor += delta;
    while (mCursor < 0) {
        mCursor += mShownCount;
    }
    if (mCursor >= mShownCount) {
        mCursor %= mShownCount;
    }
    ScrollToRow(mCursor / 4);
}

int Ingredients::fn_80119538() {
    int cursor = mCursor;
    cursor -= mPage * 4;
    return cursor;
}

void Ingredients::fn_801194E0(int index) {
    mCursor = index + mPage * 4;
    ScrollToRow(mCursor / 4);
}

// NonMatching 99.57%: register numbering (r3/r4 swapped) in the breed loop.
void Ingredients::fn_801191C8() {
    mCursor = -1;
    void* mark = fn_801A1208(fn_80198F18(fn_8018D1E4(lbl_805FB654), 0x40703B82));
    memset(mShownIcons, 0, sizeof(mShownIcons));
    memset(mShown, 0, sizeof(mShown));
    memset(mShownTexts, 0, sizeof(mShownTexts));
    memset(mShownMarks, 0, sizeof(mShownMarks));
    mShownCount = 0;
    if (mType == -1 || mTab == 1) {
        for (int i = 0; i < mCount; i++) {
            UnkInventoryItem* item = &mItems[i];
            if (item->mKind != mTab) {
                continue;
            }
            if (mTypeFilter != 3 && item->mType != mTypeFilter && item->mType != -1) {
                continue;
            }
            mShown[mShownCount] = item;
            mShownIcons[mShownCount] = item->mIcon;
            if (item->mIcon != 0) {
                mShownTexts[mShownCount] = item->mCountText;
                mShownMarks[mShownCount] = mark;
            }
            mShownCount++;
        }
    } else if (mTab == 0) {
        UnkBreedInfo breeds[156];
        int count = 0;
        for (int i = 0; i < 156; i++) {
            UnkBreedInfo breed = lbl_805FBBC0->mData->mBreeds[i];
            breeds[count] = breed;
            if (mType == breed.mGroup) {
                count++;
            }
        }
        for (int i = 0; i < mCount; i++) {
            UnkInventoryItem* item = &mItems[i];
            if (item->mKind != mTab) {
                continue;
            }
            for (int j = 0; j < count; j++) {
                if (item->mIndex == breeds[j].mIngredient) {
                    mShown[mShownCount] = item;
                    mShownIcons[mShownCount] = item->mIcon;
                    if (item->mIcon != 0) {
                        mShownTexts[mShownCount] = item->mCountText;
                        mShownMarks[mShownCount] = mark;
                    }
                    mShownCount++;
                    break;
                }
            }
        }
    }
    int pages = mShownCount / 4;
    if (mShownCount % 4 > 0) {
        pages++;
    }
    mProgress.fn_80116E54(3, pages);
    mProgress.fn_80116EF0(mPage);
}

bool Ingredients::fn_801191C0() {
    return mWasPartsTab;
}

int Ingredients::fn_801191AC() {
    return mTab == 1;
}

void Ingredients::fn_801190E8(int rows) {
    int page;
    int old;
    int pages = mShownCount / 4;
    if (mShownCount % 4 > 0) {
        pages++;
    }
    int delta = rows * 3;
    old = mPage;
    if (pages > 0) {
        page = old + delta;
        if (pages > page) {
            mPage = page;
            mPage = UnkClampBetween(page, 0, pages);
        }
        if (old != mPage) {
            mCursor += (mPage - old) * 4;
        }
        mProgress.fn_80116EF0(mPage);
    }
}

void Ingredients::fn_80118FC0(int delta) {
    ReleaseIcons();
    mWasPartsTab = mTab == 1;
    mTab += delta;
    while (mTab < 0) {
        mTab += 2;
    }
    mTab %= 2;
    mPage = 0;
    m4 = mTab + 1;
    fn_8011813C();
}

const char* Ingredients::fn_80118F44() {
    switch (mTab) {
    case 0:
        return GetTextByIndex(0x466);
    case 1:
        return GetTextByIndex(0x483);
    }
    return 0;
}

int Ingredients::fn_80118F3C() {
    return mTab;
}

void Ingredients::fn_80118ED4(u32 hash) {
    int filter = fn_8020E4E8(hash);
    int old = mTypeFilter;
    if (filter >= 0 && filter < 3) {
        mTypeFilter = filter;
    } else {
        mTypeFilter = 3;
    }
    if (mTypeFilter != old) {
        mPage = 0;
    }
}

void Ingredients::fn_80118E88(int type, int tab) {
    mType = type;
    mTab = tab;
    m4 = tab + 1;
    UnkTypeInfo typeInfo = lbl_805FBBC0->mData->mTypes[type];
    mTypeFilter = typeInfo.mTypeFilter;
}

// NonMatching 97%: register numbering and count handling in the breed copy loop.
int Ingredients::fn_80118C54(int type, int tab) {
    UnkTypeInfo typeInfo = lbl_805FBBC0->mData->mTypes[type];
    if (tab == 1) {
        for (int i = 0; i < 105; i++) {
            UnkStatItem* item = &lbl_805FBBE0->mItems[i];
            if (item->mCount != 0 && item->mKind == 1) {
                UnkPartInfo part = lbl_805FBBC0->mData->mParts[item->mIndex];
                if (part.mType == typeInfo.mTypeFilter) {
                    return 1;
                }
            }
        }
    } else if (tab == 0) {
        UnkBreedInfo breeds[156];
        int count = 0;
        for (int i = 0; i < 156; i++) {
            UnkBreedInfo breed = lbl_805FBBC0->mData->mBreeds[i];
            breeds[count] = breed;
            if (type == breed.mGroup) {
                count++;
            }
        }
        for (int i = 0; i < 105; i++) {
            UnkStatItem* item = &lbl_805FBBE0->mItems[i];
            if (item->mCount != 0 && item->mKind == 0) {
                UnkIngredientInfo ingredient = lbl_805FBBC0->mData->mIngredients[item->mIndex];
                for (int j = 0; j < count; j++) {
                    if (ingredient.mId == breeds[j].mIngredient) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

UnkInventoryItem* Ingredients::fn_80118C24() {
    if (mCursor >= 0 && mCursor < mCount) {
        return mShown[mCursor];
    }
    return 0;
}

void Ingredients::fn_80118BDC() {
    if (mCursor >= 0 && mCursor < mCount) {
        UnkInventoryItem* item = mShown[mCursor];
        if (item != 0) {
            fn_80219ED4(lbl_805FBBE0->mA30, item->mIndex, item->mKind, 1);
        }
    }
}

u8 Ingredients::fn_80118BD4() {
    return lbl_805FB260;
}

extern "C" UnkTexture* fn_80118B1C(int part) {
    char name[0x100];
    UnkPartInfo info = lbl_805FBBC0->mData->mParts[part];
    fn_80142EF8(name, "%sinventory_icon_sec_%03d", lbl_805F8F10, info.mIcon);
    UnkTexture* texture = fn_801094F4(name, 0);
    if (texture == 0) {
        fn_80142EF8(name, "%sinventory_icon_placeholder", lbl_805F8F10);
        texture = fn_801094F4(name, 0);
    }
    return texture;
}

UnkTexture* Ingredients::fn_80118A6C(int part) {
    char name[0x100];
    UnkPartInfo info = lbl_805FBBC0->mData->mParts[part];
    fn_80142EF8(name, "%sinventory_icon_sec_%03d", lbl_805F8F10, info.mIcon);
    UnkTexture* texture = fn_80109330(name);
    if (texture == 0) {
        fn_80142EF8(name, "%sinventory_icon_placeholder", lbl_805F8F10);
        texture = fn_80109330(name);
    }
    return texture;
}

extern "C" UnkTexture* fn_801189C4(int ingredient) {
    char name[0x100];
    UnkIngredientInfo info = lbl_805FBBC0->mData->mIngredients[ingredient];
    fn_80142EF8(name, "%sinventory_icon_%03d", lbl_805F8F10, info.mIcon);
    UnkTexture* texture = fn_801094F4(name, 0);
    if (texture == 0) {
        fn_80142EF8(name, "%sinventory_icon_placeholder", lbl_805F8F10);
        texture = fn_801094F4(name, 0);
    }
    return texture;
}

// NonMatching 95.6%: one load scheduled differently; written as calls to
// fn_801189C4/fn_80118B1C so -inline auto accepts it into fn_8011813C.
UnkTexture* Ingredients::fn_80118858(const UnkStatItem* item) {
    if (item->mKind == 0) {
        return fn_801189C4(item->mIndex);
    } else if (item->mKind == 1) {
        return fn_80118B1C(item->mIndex);
    }
    return 0;
}

// NonMatching 95.4%: format strings addressed from the path string (@381) base
// instead of the unit's .data base, plus one load scheduled differently.
UnkTexture* Ingredients::fn_801186FC(const UnkStatItem* item) {
    if (item->mKind == 0) {
        char name[0x100];
        UnkIngredientInfo info = lbl_805FBBC0->mData->mIngredients[item->mIndex];
        fn_80142EF8(name, "%sinventory_icon_%03d", lbl_805F8F10, info.mIcon);
        UnkTexture* texture = fn_80109330(name);
        if (texture == 0) {
            fn_80142EF8(name, "%sinventory_icon_placeholder", lbl_805F8F10);
            texture = fn_80109330(name);
        }
        return texture;
    } else if (item->mKind == 1) {
        char name[0x100];
        UnkPartInfo info = lbl_805FBBC0->mData->mParts[item->mIndex];
        fn_80142EF8(name, "%sinventory_icon_sec_%03d", lbl_805F8F10, info.mIcon);
        UnkTexture* texture = fn_80109330(name);
        if (texture == 0) {
            fn_80142EF8(name, "%sinventory_icon_placeholder", lbl_805F8F10);
            texture = fn_80109330(name);
        }
        return texture;
    }
    return 0;
}

// NonMatching 99.2%: in case 0 the entry and index loads are swapped and the
// ingredient/part copies use swapped stack slots.
void Ingredients::fn_8011813C() {
    mCount = 0;
    for (int i = 0; i < 105; i++) {
        UnkStatItem* item = &lbl_805FBBE0->mItems[i];
        if (item->mKind != mTab) {
            continue;
        }
        if (item->mKind == 1) {
            UnkPartInfo part = lbl_805FBBC0->mData->mParts[item->mIndex];
            if (part.mType != mTypeFilter && mTypeFilter != 3) {
                continue;
            }
        }
        if (item->mCount == 0) {
            continue;
        }
        switch (item->mKind) {
        case 0: {
            UnkInventoryItem* entry = &mItems[mCount];
            UnkIngredientInfo ingredient = lbl_805FBBC0->mData->mIngredients[item->mIndex];
            entry->mType = -1;
            entry->mKind = item->mKind;
            entry->mNameHash = ingredient.mNameHash;
            entry->mName = GetText(ingredient.mNameHash);
            entry->mRequest = fn_80118858(item);
            break;
        }
        case 1: {
            UnkPartInfo part = lbl_805FBBC0->mData->mParts[item->mIndex];
            UnkInventoryItem* entry = &mItems[mCount];
            entry->mType = part.mType;
            entry->mKind = item->mKind;
            entry->mNameHash = part.mNameHash;
            entry->mName = GetText(part.mNameHash);
            entry->mRequest = fn_80118858(item);
            break;
        }
        }
        mItems[mCount].mIndex = item->mIndex;
        fn_80142EF8(mItems[mCount].mCountText, "%d", item->mCount);
        mCount++;
    }
}

void Ingredients::vf18() {}

void Ingredients::vf14() {
    lbl_805FB260 = 0;
    mLoaded = 0;
    bool stop = false;
    for (u32 i = 0; i < mCount; i++) {
        if (mItems[i].mRequest != 0) {
            if (!fn_80109A0C(mItems[i].mRequest)) {
                fn_80109600(&mItems[i].mRequest);
            } else if (i <= mLoaded) {
                mItems[i].mIcon = mItems[i].mRequest;
                mItems[i].mRequest = 0;
                lbl_805FB260 = 1;
            }
        }
        if (!stop) {
            if (mItems[i].mIcon != 0) {
                mLoaded = i + 1;
            } else {
                stop = true;
            }
        }
    }
}

void Ingredients::vf10() {
    mType = -1;
    ReleaseIcons();
}

void Ingredients::vf0C() {
    if (mType == -1) {
        mTab = 0;
        mTypeFilter = 3;
    }
    mCursor = -1;
    mShownCount = 0;
    mWasPartsTab = 1;
    mCount = 0;
    mLoaded = 0;
    m8 = 2;
    m4 = mTab + 1;
    mPage = 0;
    lbl_805FB260 = 0;
    fn_8011813C();
}

Ingredients::~Ingredients() {}

Ingredients::Ingredients() : mType(-1), mTab(0) {
    mCursor = 0;
    mCount = 0;
    mLoaded = 0;
    for (int i = 0; i < 117; i++) {
        mShown[i] = 0;
        mShownIcons[i] = 0;
        mShownTexts[i] = 0;
        mShownMarks[i] = 0;
    }
}
