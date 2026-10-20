// Game unit at 0x8011A778-0x8011C460 (real file name unknown): the Parts section of the
// InventoryBook (RTTI). Lists the monster parts the player owns with their names, the
// ingredient and part of their recipe and icons ("ui/msa_parts/<name>") loaded in the
// background, filtered by type and element and paged three rows of four at a time.
// Also the part icon and name helpers used by the Recipes section. Bounds come from the
// static initializers of this unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/InventoryBook.h"
#include "game/Journal.h"
#include "game/unk_text.h"
#include "game/unk_vec3.h"

#include <string.h>

// Part owned by the player (real type unknown).
struct UnkPartRecord {
    /* 0x0 */ int mDescId;
    /* 0x4 */ int m4;
    /* 0x8 */ int m8;
};

// Player data (real type unknown).
struct UnkPartsPlayer {
    /* 0x0000 */ u8 unk0[0xA30];
    /* 0x0A30 */ u8 mA30[0xA58 - 0xA30];
    /* 0x0A58 */ UnkPartRecord* mParts[0x1A5];
    /* 0x10EC */ int mPartCount;
};

// Monster description returned by the database (0x50 bytes, real type unknown).
struct UnkMonsterDesc {
    /* 0x00 */ int mId;
    /* 0x04 */ u32 mNameHashes[4]; // one per slot
    /* 0x14 */ int mAbility;
    /* 0x18 */ int mElement;
    /* 0x1C */ int mType;
    /* 0x20 */ u8 unk20[0x2C - 0x20];
    /* 0x2C */ int mAbility2;
    /* 0x30 */ u8 unk30[0x50 - 0x30];
};

struct UnkTypeEntry {
    /* 0x0 */ int mFilter;
    /* 0x4 */ u32 m4;
};

struct UnkNameEntry {
    /* 0x0 */ u32 m0;
    /* 0x4 */ u32 mNameHash;
};

struct UnkAbilityEntry {
    /* 0x00 */ u32 m0;
    /* 0x04 */ int mInfo;
    /* 0x08 */ u32 m8;
    /* 0x0C */ u32 mC;
    /* 0x10 */ u32 m10;
};

struct UnkAbilityInfo {
    /* 0x00 */ u32 m0;
    /* 0x04 */ u32 m4;
    /* 0x08 */ u32 m8;
    /* 0x0C */ u32 mC;
    /* 0x10 */ u32 mNameHash;
    /* 0x14 */ u32 m14;
    /* 0x18 */ u32 m18;
    /* 0x1C */ u32 m1C;
    /* 0x20 */ u32 m20;
};

// Ingredient description (0x10 bytes, real type unknown).
struct UnkIngredientInfo {
    /* 0x0 */ u32 mId;
    /* 0x4 */ u32 mNameHash;
    /* 0x8 */ u32 m8;
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

struct UnkPartsData {
    /* 0x0000 */ u8 unk0[0x8];
    /* 0x0008 */ UnkAbilityInfo* mAbilityInfos;
    /* 0x000C */ u8 unkC[0x430 - 0xC];
    /* 0x0430 */ UnkAbilityEntry* mAbilities;
    /* 0x0434 */ u8 unk434[0xC80 - 0x434];
    /* 0x0C80 */ UnkNameEntry* mElements;
    /* 0x0C84 */ u8 unkC84[0x4F00 - 0xC84];
    /* 0x4F00 */ UnkIngredientInfo* mIngredients;
    /* 0x4F04 */ u8 unk4F04[0x5328 - 0x4F04];
    /* 0x5328 */ UnkPartInfo* mParts;
    /* 0x532C */ u8 unk532C[0x6C18 - 0x532C];
    /* 0x6C18 */ UnkBreedInfo* mBreeds;
    /* 0x6C1C */ u8 unk6C1C[0x7890 - 0x6C1C];
    /* 0x7890 */ UnkTypeEntry* mTypes;
};

struct UnkPartsDatabase {
    /* 0x0 */ u8 unk0[0xC];
    /* 0xC */ UnkPartsData* mData;
};

extern "C" {
extern UnkPartsPlayer* lbl_805FBBE0;
extern UnkPartsDatabase* lbl_805FBBC0;
extern void* lbl_805FB654;

int fn_80142EF8(char* buf, const char* format, ...);
int fn_8020E4E8(u32 hash);
void* fn_8018D1E4(void* obj);
void* fn_80198F18(void* set, u32 hash);
void* fn_801A1208(void* actor);
void fn_802152FC(char* buf, int element, int id);
UnkMonsterDesc fn_80216838(UnkPartsDatabase* db, int id);
void fn_80219B94(void* list, UnkPartRecord* record);
int fn_8021EE94(UnkPartRecord* record);
u8 fn_8021EF58(UnkPartRecord* record);
s8 fn_8021EF60(UnkPartRecord* record);
s8 fn_8021EF80(UnkPartRecord* record);
int fn_8021F168(UnkPartRecord* record);
int fn_8022397C(void* player, u32 breed, u8 slot);
int fn_802241F0(void* player, u32 breed, u8 slot);
int fn_8022EC08(void* player, UnkPartRecord* record);
// Texture loads (unit 0x801091EC).
UnkTexture* fn_80109330(const char* name);
UnkTexture* fn_801094F4(const char* name, int arg);
int fn_80109600(UnkTexture** request);
int fn_801098DC(UnkTexture* request);
int fn_80109A0C(UnkTexture* request);
void fn_80184028(UnkTexture* texture);
// Ingredient and part icons (Ingredients section).
UnkTexture* fn_801189C4(int ingredient);
UnkTexture* fn_80118B1C(int part);
// Monster icon and name (also called by other units by these names).
UnkTexture* fn_8011AD88(const UnkMonsterDesc* desc, int highlighted);
UnkTexture* fn_8011AE30(const UnkMonsterDesc* desc, int highlighted);
const char* fn_8011AEE8(const UnkMonsterDesc* desc, u8 slot);
}

// Marks shown on the parts (real names unknown).
void* lbl_805FB298;
void* lbl_805FB29C;
void* lbl_805FB2A0;
// Set when a background icon load finished (real name unknown).
u8 lbl_805FB2A4;

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

// Clamp between two bounds given in any order (the macro form reads the value again in
// each branch).
#define UNK_CLAMP_BETWEEN(value, a, b)                                                          ((a) < (b) ? ((value) < (a) ? (a) : ((value) > (b) ? (b) : (value)))                                    : ((value) < (b) ? (b) : ((value) > (a) ? (a) : (value))))

// Clamp between two bounds given in any order.
static inline int UnkClampBetween(int value, int a, int b) {
    return a < b ? (value < a ? a : (value > b ? b : value))
                 : (value < b ? b : (value > a ? a : value));
}

inline void Parts::UpdateCategory() {
    if (mElement == 5) {
        m4 = 1;
    } else if (mElement < 3) {
        m4 = mElement + 2;
    } else {
        m4 = mElement + 1;
    }
}

Parts::Parts() : mCursor(0), mPage(0), mCount(0), mTypeFilter(3), mElement(5), mLoaded(0) {
    for (int i = 0; i < 112; i++) {
        mShown[i] = 0;
        mShownIcons[i] = 0;
        mShownMarks[i] = 0;
        mShownNewMarks[i] = 0;
    }
}

Parts::~Parts() {}

// NonMatching 99.6%: the item pointer and the record offset of the loop are in swapped
// saved registers.
void Parts::vf0C() {
    mTypeFilter = 3;
    m4 = 5;
    mElement = 5;
    UpdateCategory();
    mCount = lbl_805FBBE0->mPartCount;
    mLoaded = 0;
    for (int i = 0; i < mCount; i++) {
        fn_8011B060(lbl_805FBBE0->mParts[i], &mItems[i]);
        mItems[i].mIndex = i;
    }
    m8 = 5;
    mPage = 0;
    lbl_805FB2A4 = 0;
    mSelected = -1;
}

void Parts::vf10() {
    lbl_805FB298 = 0;
    lbl_805FB29C = 0;
    lbl_805FB2A0 = 0;
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
        if (mItems[i].mIngredientIcon != 0) {
            ReleaseTexture(mItems[i].mIngredientIcon);
            mItems[i].mIngredientIcon = 0;
        }
        if (mItems[i].mIngredientRequest != 0) {
            if (!fn_80109A0C(mItems[i].mIngredientRequest)) {
                fn_801098DC(mItems[i].mIngredientRequest);
            }
            mItems[i].mIngredientRequest = 0;
        }
        if (mItems[i].mPartIcon != 0) {
            ReleaseTexture(mItems[i].mPartIcon);
            mItems[i].mPartIcon = 0;
        }
        if (mItems[i].mPartRequest != 0) {
            if (!fn_80109A0C(mItems[i].mPartRequest)) {
                fn_801098DC(mItems[i].mPartRequest);
            }
            mItems[i].mPartRequest = 0;
        }
    }
}

void Parts::vf14() {
    lbl_805FB2A4 = 0;
    mLoaded = 0;
    bool stop = false;
    for (u32 i = 0; i < mCount; i++) {
        if (mItems[i].mRequest != 0) {
            if (!fn_80109A0C(mItems[i].mRequest)) {
                fn_80109600(&mItems[i].mRequest);
            } else if (i <= mLoaded) {
                mItems[i].mIcon = mItems[i].mRequest;
                mItems[i].mRequest = 0;
                lbl_805FB2A4 = 1;
            }
        }
        if (!stop) {
            if (mItems[i].mIcon != 0) {
                mLoaded = i + 1;
            } else {
                stop = true;
            }
        }
        if (mItems[i].mIngredientRequest != 0 && !fn_80109A0C(mItems[i].mIngredientRequest) &&
            fn_80109600(&mItems[i].mIngredientRequest)) {
            mItems[i].mIngredientIcon = mItems[i].mIngredientRequest;
            mItems[i].mIngredientRequest = 0;
            lbl_805FB2A4 = 1;
        }
        if (mItems[i].mPartRequest != 0 && !fn_80109A0C(mItems[i].mPartRequest) &&
            fn_80109600(&mItems[i].mPartRequest)) {
            mItems[i].mPartIcon = mItems[i].mPartRequest;
            mItems[i].mPartRequest = 0;
            lbl_805FB2A4 = 1;
        }
    }
}

void Parts::vf18() {}

// A const description: the DOL loads desc->mId above the prologue stores.
UnkTexture* fn_8011AD88(const UnkMonsterDesc* desc, int highlighted) {
    const char* dir = "ui/msa_parts/";
    char name[0x100];
    char path[0x100];
    fn_802152FC(name, desc->mElement, desc->mId);
    strcpy(path, dir);
    strcat(path, name);
    if (highlighted) {
        strcat(path, "_h");
    }
    UnkTexture* texture = fn_80109330(path);
    if (texture == 0) {
        strcpy(path, dir);
        strcat(path, "placeholder");
        texture = fn_80109330(path);
    }
    return texture;
}

UnkTexture* fn_8011AE30(const UnkMonsterDesc* desc, int highlighted) {
    const char* dir = "ui/msa_parts/";
    char name[0x100];
    char path[0x100];
    fn_802152FC(name, desc->mElement, desc->mId);
    strcpy(path, dir);
    strcat(path, name);
    if (highlighted) {
        strcat(path, "_h");
    }
    UnkTexture* texture = fn_801094F4(path, 0);
    if (texture == 0) {
        strcpy(path, dir);
        strcat(path, "placeholder");
        texture = fn_801094F4(path, 0);
    }
    return texture;
}

u8 Parts::fn_8011AEE0() {
    return lbl_805FB2A4;
}

const char* fn_8011AEE8(const UnkMonsterDesc* desc, u8 slot) {
    u32 hash = desc->mNameHashes[0];
    switch (slot) {
    case 1:
        hash = desc->mNameHashes[0];
        break;
    case 2:
        hash = desc->mNameHashes[1];
        break;
    case 3:
        hash = desc->mNameHashes[2];
        break;
    case 4:
        hash = desc->mNameHashes[3];
        break;
    }
    return GetText(hash);
}

const char* Parts::fn_8011AFEC() {
    fn_80142EF8(mText, GetTextByIndex(0x594), mShown[mCursor]->mName);
    return mText;
}

static inline UnkAbilityInfo GetAbilityInfo(int index) {
    UnkAbilityEntry ability = lbl_805FBBC0->mData->mAbilities[index];
    return lbl_805FBBC0->mData->mAbilityInfos[ability.mInfo];
}

void Parts::fn_8011B060(UnkPartRecord* record, UnkPartItem* item) {
    // Read through a reference: the DOL reloads the fields after each call.
    UnkMonsterDesc temp = fn_80216838(lbl_805FBBC0, record->mDescId);
    UnkMonsterDesc& desc = temp;
    UnkTypeEntry type = lbl_805FBBC0->mData->mTypes[desc.mType];
    item->mNameHash = desc.mNameHashes[0];
    item->mName = fn_8011AEE8(&desc, fn_8021EF58(record));
    item->mType = type.mFilter;
    item->mElement = desc.mElement;
    item->m30 = fn_8021F168(record);
    item->m34 = fn_8021EF60(record);
    item->m38 = fn_8021EF80(record);
    item->mAbilityHash = GetAbilityInfo(desc.mAbility).mNameHash;
    item->mAbilityName = GetText(item->mAbilityHash);
    item->mAbility2Hash = GetAbilityInfo(desc.mAbility2).mNameHash;
    item->mAbility2Name = GetText(item->mAbility2Hash);
    item->mRequest = fn_8011AE30(&desc, 0);
    item->mBreed = -1;
    for (int i = 0; i < 156 && item->mBreed == -1; i++) {
        UnkBreedInfo breed = lbl_805FBBC0->mData->mBreeds[i];
        if (breed.mDescId == record->mDescId &&
            fn_8022397C(lbl_805FBBE0, breed.mId, fn_8021EF58(record))) {
            item->mBreed = i;
        }
    }
    item->mPartIcon = 0;
    item->mIngredientIcon = 0;
    if (item->mBreed != -1) {
        UnkBreedInfo breed = lbl_805FBBC0->mData->mBreeds[item->mBreed];
        UnkIngredientInfo ingredient = lbl_805FBBC0->mData->mIngredients[breed.mIngredient];
        item->mIngredientRequest = fn_801189C4(breed.mIngredient);
        if (fn_80109A0C(item->mIngredientRequest)) {
            item->mIngredientIcon = item->mIngredientRequest;
            item->mIngredientRequest = 0;
        }
        item->mIngredientName = GetText(ingredient.mNameHash);
        item->mHasPart = 0;
        if (fn_802241F0(lbl_805FBBE0, breed.mId, fn_8021EF58(record))) {
            int part = Recipes::fn_80120990(breed.mType, breed.mPartKey, fn_8021EF58(record));
            if (part != -1) {
                UnkPartInfo info = lbl_805FBBC0->mData->mParts[part];
                item->mHasPart = 1;
                item->mPartRequest = fn_80118B1C(part);
                if (fn_80109A0C(item->mPartRequest)) {
                    item->mPartIcon = item->mPartRequest;
                    item->mPartRequest = 0;
                }
                item->mPartName = GetText(info.mNameHash);
            }
        }
    }
    item->mMark = 0;
    item->mNewMark = 0;
}

void Parts::fn_8011B898() {
    memset(mShownIcons, 0, sizeof(mShownIcons));
    memset(mShownMarks, 0, sizeof(mShownMarks));
    memset(mShownNewMarks, 0, sizeof(mShownNewMarks));
    mShownCount = 0;
    for (int i = 0; i < mCount; i++) {
        UnkPartItem* item = &mItems[i];
        if ((item->mType == mTypeFilter || mTypeFilter == 3) &&
            (item->mElement == mElement || mElement == 5)) {
            mShown[mShownCount] = item;
            mShownIcons[mShownCount] = item->mIcon;
            if (item->mIcon != 0) {
                mShownMarks[mShownCount] = item->mMark;
                mShownNewMarks[mShownCount] = item->mNewMark;
            }
            mShownCount++;
        }
    }
    int pages = mShownCount / 4;
    if (mShownCount % 4 > 0) {
        pages++;
    }
    mProgress.fn_80116E54(3, pages);
    mPage = UNK_CLAMP_BETWEEN(mPage, 0, pages);
    mProgress.fn_80116EF0(mPage);
}

void Parts::fn_8011BA48(int index) {
    mCursor = index + mPage * 4;
}

// The DOL subtracts the cursor times four from the cursor itself.
int Parts::fn_8011BA5C() {
    return mCursor - mCursor * 4;
}

void Parts::fn_8011BA6C(u32 topHash, u32 bottomHash) {
    mProgress.fn_80116D40(topHash, bottomHash);
}

void Parts::fn_8011BA74(u32 hash) {
    mProgress.mBarHash = hash;
}

UnkTexture** Parts::fn_8011BA7C() {
    return &mShownIcons[mPage * 4];
}

void** Parts::fn_8011BA90() {
    return &mShownMarks[mPage * 4];
}

void** Parts::fn_8011BAA4() {
    return &mShownNewMarks[mPage * 4];
}

void Parts::fn_8011BAB8(u32 hash) {
    int filter = fn_8020E4E8(hash);
    int old = mTypeFilter;
    if (filter >= 0 && filter < 3) {
        mTypeFilter = filter;
    } else {
        mTypeFilter = 3;
    }
    if (old != mTypeFilter) {
        mPage = 0;
    }
}

int Parts::fn_8011BB20() {
    return mTypeFilter;
}

const char* Parts::fn_8011BB28() {
    if (mElement != 5) {
        UnkNameEntry element = lbl_805FBBC0->mData->mElements[mElement];
        return GetText(element.mNameHash);
    }
    return GetTextByIndex(0x43F);
}

int Parts::fn_8011BC40(int* out, int element) {
    int count = 0;
    for (int i = 0; i < lbl_805FBBE0->mPartCount; i++) {
        UnkMonsterDesc desc = fn_80216838(lbl_805FBBC0, lbl_805FBBE0->mParts[i]->mDescId);
        if (desc.mElement == element) {
            out[count++] = i;
        }
    }
    return count;
}

void Parts::fn_8011BCF8(int delta) {
    int element = mElement;
    element += delta;
    int step = delta < 0 ? -1 : 1;
    while (element < 0) {
        element += 6;
    }
    if (element >= 6) {
        element %= 6;
    }
    if (element == 3) {
        element += step;
    }
    mElement = element;
    m4 = element;
    UpdateCategory();
    mPage = 0;
}

void Parts::fn_8011BDA8(int rows) {
    int pages = mShownCount / 4;
    if (mShownCount % 4 > 0) {
        pages++;
    }
    int delta = rows * 3;
    if (pages > 0) {
        int page = mPage + delta;
        if (pages > page) {
            mPage = page;
            mPage = UnkClampBetween(page, 0, pages);
        }
        mProgress.fn_80116EF0(mPage);
    }
}

int Parts::fn_8011BE4C() {
    if (mCursor < mShownCount && mShown[mCursor]->mHasPart != 0) {
        return 1;
    }
    return 0;
}

UnkTexture* Parts::fn_8011BE84() {
    if (mCursor < mShownCount) {
        return mShown[mCursor]->mIngredientIcon;
    }
    return 0;
}

const char* Parts::fn_8011BEB0() {
    if (mCursor < mShownCount) {
        return mShown[mCursor]->mIngredientName;
    }
    return 0;
}

UnkTexture* Parts::fn_8011BEDC() {
    if (mCursor < mShownCount) {
        return mShown[mCursor]->mPartIcon;
    }
    return 0;
}

const char* Parts::fn_8011BF08() {
    if (mCursor < mShownCount) {
        return mShown[mCursor]->mPartName;
    }
    return 0;
}

const char* Parts::fn_8011BF34() {
    if (mCursor < mShownCount) {
        return mShown[mCursor]->mName;
    }
    return GetText(0x557);
}

const char* Parts::fn_8011C018(int index) {
    int i = index + mPage * 4;
    if (i >= 0 && i < mShownCount) {
        return mShown[i]->mName;
    }
    return GetText(0x557);
}

UnkTexture* Parts::fn_8011C108() {
    if (mCursor < mShownCount) {
        return mShownIcons[mCursor];
    }
    return 0;
}

int Parts::fn_8011C130() {
    if (mCursor < mShownCount && mShown[mCursor]->mBreed != -1) {
        return 1;
    }
    return 0;
}

int Parts::fn_8011C168() {
    if (mCursor < mShownCount &&
        fn_8022EC08(lbl_805FBBE0, lbl_805FBBE0->mParts[mShown[mCursor]->mIndex])) {
        return 1;
    }
    return 0;
}

int Parts::fn_8011C1D0(int index) {
    return index + mPage * 4 < mShownCount;
}

int Parts::fn_8011C1F8(int index) {
    return index == mSelected;
}

void Parts::fn_8011C20C(int index) {
    mSelected = index;
}

int Parts::fn_8011C214() {
    return mShown[mCursor]->mIndex;
}

void Parts::fn_8011C22C() {
    if (mCursor < mShownCount) {
        fn_80219B94(lbl_805FBBE0->mA30, lbl_805FBBE0->mParts[mShown[mCursor]->mIndex]);
        vf10();
        vf0C();
    }
}

int Parts::fn_8011C2B4() {
    if (mCursor < mShownCount && fn_8021EE94(lbl_805FBBE0->mParts[mShown[mCursor]->mIndex])) {
        return 1;
    }
    return 0;
}

void Parts::fn_8011C318() {
    void* set = fn_8018D1E4(lbl_805FB654);
    void* mark = fn_80198F18(set, 0xDCAEC94C);
    if (mark != 0) {
        lbl_805FB298 = fn_801A1208(mark);
    }
    mark = fn_80198F18(set, 0xCA64FF01);
    if (mark != 0) {
        lbl_805FB29C = fn_801A1208(mark);
    }
    mark = fn_80198F18(set, 0x1C6314F7);
    if (mark != 0) {
        lbl_805FB2A0 = fn_801A1208(mark);
    }
    mCount = lbl_805FBBE0->mPartCount;
    for (int i = 0; i < mCount; i++) {
        UnkPartRecord* record = lbl_805FBBE0->mParts[i];
        if (record->m8 != -1) {
            mItems[i].mMark = lbl_805FB298;
        } else if (record->m4 != -1) {
            mItems[i].mMark = lbl_805FB29C;
        }
        if (fn_8021EE94(record)) {
            mItems[i].mNewMark = lbl_805FB2A0;
        }
    }
}
