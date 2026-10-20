// Game unit at 0x8011F410-0x80120AAC (real file name unknown): the Recipes section of
// the Journal (RTTI). Lists, for each monster breed and slot the player knows, the
// recipe with its monster, ingredient and part (names and icons loaded in the
// background), filtered by type and element and paged four by four. Bounds come from
// the static initializers of this unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/Journal.h"
#include "game/unk_text.h"
#include "game/unk_vec3.h"

#include <string.h>

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

// Monster description returned by the database (0x50 bytes, real type unknown).
struct UnkMonsterDesc {
    /* 0x00 */ u8 unk0[0x18];
    /* 0x18 */ int mElement;
    /* 0x1C */ int mType;
    /* 0x20 */ u8 unk20[0x50 - 0x20];
};

struct UnkIngredientInfo {
    /* 0x0 */ u32 m0;
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
    /* 0x12 */ u8 m12;
    /* 0x13 */ s8 mSlot;
    /* 0x14 */ u8 m14;
    /* 0x15 */ u8 m15;
};

struct UnkNameInfo {
    /* 0x0 */ u32 m0;
    /* 0x4 */ u32 mNameHash;
};

struct UnkTypeInfo {
    /* 0x0 */ u32 m0;
    /* 0x4 */ u32 m4;
    /* 0x8 */ int mTypeFilter;
};

struct UnkRecipeData {
    /* 0x0000 */ u8 unk0[0xC80];
    /* 0x0C80 */ UnkNameInfo* mElements;
    /* 0x0C84 */ u8 unkC84[0x3E60 - 0xC84];
    /* 0x3E60 */ UnkTypeInfo* mTypes;
    /* 0x3E64 */ u8 unk3E64[0x4F00 - 0x3E64];
    /* 0x4F00 */ UnkIngredientInfo* mIngredients;
    /* 0x4F04 */ u8 unk4F04[0x5328 - 0x4F04];
    /* 0x5328 */ UnkPartInfo* mParts;
    /* 0x532C */ int mPartCount;
    /* 0x5330 */ u8 unk5330[0x6C18 - 0x5330];
    /* 0x6C18 */ UnkBreedInfo* mBreeds;
};

struct UnkRecipeDatabase {
    /* 0x0 */ u8 unk0[0xC];
    /* 0xC */ UnkRecipeData* mData;
};

extern "C" {
extern void* lbl_805FBBE0;
extern UnkRecipeDatabase* lbl_805FBBC0;

// Whether the player knows the breed's recipe for the slot (real name unknown).
int fn_8022397C(void* stats, int breed, u8 slot);
u32 fn_802241F0(void* stats, u32 id, u8 slot);
int fn_8020E4E8(u32 hash);
// Monster description by id (real name unknown).
UnkMonsterDesc fn_80216838(UnkRecipeDatabase* db, int id);
// Monster name and icon, ingredient and part icons (other book sections, real names
// unknown).
const char* fn_8011AEE8(UnkMonsterDesc* desc, u8 slot);
UnkTexture* fn_8011AE30(UnkMonsterDesc* desc);
UnkTexture* fn_801189C4(int ingredient);
UnkTexture* fn_80118B1C(u32 part);
// Background texture loads (unit 0x801091EC).
int fn_80109600(UnkTexture** request);
int fn_801098DC(UnkTexture* request);
int fn_80109A0C(UnkTexture* request);
void fn_80184028(UnkTexture* texture);
}

// Set when a background icon load finished (real name unknown).
u8 lbl_805FB2B0;

static inline void ReleaseTexture(UnkTexture* texture) {
    int count = --texture->mRefCount;
    if (count <= 0 && count == 0) {
        fn_80184028(texture);
    }
}

// Text at index 0x43F of the main table (real name unknown).
static inline const char* GetTextByIndex43F() {
    UnkTextTable* table = lbl_805FB910;
    if (table->mTexts != 0 && table->mCount > 0x43F) {
        return table->mTexts[0x43F];
    }
    return lbl_805F8200;
}

// Clamp between two bounds given in any order.
static inline int UnkClampBetween(int value, int a, int b) {
    return a < b ? (value < a ? a : (value > b ? b : value))
                 : (value < b ? b : (value > a ? a : value));
}

static inline UnkPartInfo GetPartInfo(int index) {
    return lbl_805FBBC0->mData->mParts[index];
}

// Built with -inline auto,deferred (configure.py): fn_80120990 is inlined into vf0C
// although it is defined after it, and the compiler then emits the functions in
// reverse order of definition, so they are defined here from the last to the first.

u8 Recipes::fn_80120A84() {
    return lbl_805FB2B0;
}

// NonMatching (60.8 %): the DOL copies each part twice on the stack, first word by
// word, then member by member (lha/lbz), keeps both copies and reloads mData every
// iteration; no spelling tried (copy constructor, assignment, second type, by-value
// or member predicate) keeps the copies.
int Recipes::fn_80120990(int type, int key, int slot) {
    for (int i = 0; i < lbl_805FBBC0->mData->mPartCount; i++) {
        UnkPartInfo part = GetPartInfo(i);
        if (part.mType == type && part.mKey == key && part.mSlot == slot) {
            return i;
        }
    }
    return -1;
}

void Recipes::fn_80120984(u32 hash) {
    mProgress.mBarHash = hash;
}

void Recipes::fn_80120978(u32 topHash, u32 bottomHash) {
    mProgress.fn_80116D40(topHash, bottomHash);
}

int Recipes::fn_8012096C() {
    return mTypeFilter;
}

void Recipes::fn_801208F8(u32 hash) {
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

void Recipes::fn_801207DC(int type) {
    mType = type;
    UnkTypeInfo typeInfo = lbl_805FBBC0->mData->mTypes[type];
    mTypeFilter = typeInfo.mTypeFilter;
    for (int breed = 0; breed < 156; breed++) {
        UnkBreedInfo info = lbl_805FBBC0->mData->mBreeds[breed];
        if (info.mGroup == type) {
            UnkMonsterDesc desc = fn_80216838(lbl_805FBBC0, info.mDescId);
            mElement = desc.mElement;
        }
    }
}

void Recipes::fn_801207D0(int index) {
    mSelected = index;
}

int Recipes::fn_801207B8(int index) {
    return index == mSelected;
}

int Recipes::fn_8012078C(int index) {
    return index + mPage * 4 < mShownCount;
}

int Recipes::fn_801206A0(int type) {
    for (int breed = 0; breed < 156; breed++) {
        UnkBreedInfo info = lbl_805FBBC0->mData->mBreeds[breed];
        if (info.mGroup == type) {
            for (int slot = 0; slot < 4; slot++) {
                if (fn_8022397C(lbl_805FBBE0, breed, slot)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void Recipes::fn_801205EC(int rows) {
    int page;
    int pages = mShownCount / 4;
    if (mShownCount % 4 > 0) {
        pages++;
    }
    int delta = rows * 3;
    if (pages > 0) {
        page = mPage + delta;
        if (pages > page) {
            mPage = page;
            mPage = UnkClampBetween(page, 0, pages);
        }
        mProgress.fn_80116EF0(mPage);
    }
}

void Recipes::fn_80120534(int delta) {
    int step;
    int element = mElement;
    element += delta;
    if (delta < 0) {
        step = -1;
    } else {
        step = 1;
    }
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
    mPage = 0;
    UpdateTab();
}

int Recipes::fn_80120504() {
    if (mCursor < mShownCount) {
        return mShown[mCursor]->mBreed;
    }
    return 0;
}

const char* Recipes::fn_801204D4() {
    if (mCursor < mShownCount) {
        return mShown[mCursor]->mPartName;
    }
    return 0;
}

UnkTexture* Recipes::fn_801204A4() {
    if (mCursor < mShownCount) {
        return mShown[mCursor]->mPartIcon;
    }
    return 0;
}

const char* Recipes::fn_80120474() {
    if (mCursor < mShownCount) {
        return mShown[mCursor]->mIngredientName;
    }
    return 0;
}

UnkTexture* Recipes::fn_80120444() {
    if (mCursor < mShownCount) {
        return mShown[mCursor]->mIngredientIcon;
    }
    return 0;
}

UnkTexture* Recipes::fn_80120418() {
    if (mCursor < mShownCount) {
        return mShownIcons[mCursor];
    }
    return 0;
}

int Recipes::fn_801203F0() {
    return mShown[mCursor]->mOwned != 0;
}

u32 Recipes::fn_801203C0() {
    if (mCursor < mShownCount) {
        return mShown[mCursor]->mSlot;
    }
    return 0;
}

int Recipes::fn_80120390() {
    if (mCursor < mShownCount) {
        return mShown[mCursor]->mDescId;
    }
    return 0;
}

const char* Recipes::fn_8012029C(int index) {
    int i = index + mPage * 4;
    if (i >= 0 && i < mShownCount) {
        return mShown[i]->mName;
    }
    return GetText(0x557);
}

const char* Recipes::fn_801201B4() {
    if (mCursor < mShownCount) {
        return mShown[mCursor]->mName;
    }
    return GetText(0x557);
}

const char* Recipes::fn_80120098() {
    if (mElement != 5) {
        UnkNameInfo info = lbl_805FBBC0->mData->mElements[mElement];
        return GetText(info.mNameHash);
    }
    return GetTextByIndex43F();
}

int Recipes::fn_80120084() {
    return mCursor - mCursor * 4;
}

void Recipes::fn_8012006C(int index) {
    mCursor = index + mPage * 4;
}

UnkTexture** Recipes::fn_80120054() {
    return &mShownIcons[mPage * 4];
}

void Recipes::fn_8011FF28() {
    memset(mShownIcons, 0, sizeof(mShownIcons));
    mShownCount = 0;
    for (int i = 0; i < mCount; i++) {
        UnkRecipe* recipe = &mRecipes[i];
        if ((recipe->mType == mTypeFilter || mTypeFilter == 3) &&
            (recipe->mElement == mElement || mElement == 5)) {
            mShown[mShownCount] = recipe;
            mShownIcons[mShownCount] = recipe->mIcon;
            mShownCount++;
        }
    }
    int pages = 0;
    if (mShownCount > 0) {
        pages = mShownCount / 4;
        if (mShownCount % 4 > 0) {
            pages++;
        }
    }
    mProgress.fn_80116E54(3, pages);
    mProgress.fn_80116EF0(mPage);
}

void Recipes::vf18() {}

void Recipes::vf14() {
    lbl_805FB2B0 = 0;
    mLoaded = 0;
    bool stop = false;
    for (u32 i = 0; i < mCount; i++) {
        if (mRecipes[i].mIconRequest != 0) {
            if (!fn_80109A0C(mRecipes[i].mIconRequest)) {
                fn_80109600(&mRecipes[i].mIconRequest);
            } else if (i <= mLoaded) {
                mRecipes[i].mIcon = mRecipes[i].mIconRequest;
                mRecipes[i].mIconRequest = 0;
                lbl_805FB2B0 = 1;
            }
        }
        if (!stop) {
            if (mRecipes[i].mIcon != 0) {
                mLoaded = i + 1;
            } else {
                stop = true;
            }
        }
        if (mRecipes[i].mIngredientRequest != 0 && !fn_80109A0C(mRecipes[i].mIngredientRequest) &&
            fn_80109600(&mRecipes[i].mIngredientRequest)) {
            mRecipes[i].mIngredientIcon = mRecipes[i].mIngredientRequest;
            mRecipes[i].mIngredientRequest = 0;
            lbl_805FB2B0 = 1;
        }
        if (mRecipes[i].mPartRequest != 0 && !fn_80109A0C(mRecipes[i].mPartRequest) &&
            fn_80109600(&mRecipes[i].mPartRequest)) {
            mRecipes[i].mPartIcon = mRecipes[i].mPartRequest;
            mRecipes[i].mPartRequest = 0;
            lbl_805FB2B0 = 1;
        }
    }
}

void Recipes::vf10() {
    for (int i = 0; i < mCount; i++) {
        if (mRecipes[i].mIcon != 0) {
            ReleaseTexture(mRecipes[i].mIcon);
            mRecipes[i].mIcon = 0;
        }
        if (mRecipes[i].mIconRequest != 0) {
            if (!fn_80109A0C(mRecipes[i].mIconRequest)) {
                fn_801098DC(mRecipes[i].mIconRequest);
            } else {
                ReleaseTexture(mRecipes[i].mIconRequest);
            }
            mRecipes[i].mIconRequest = 0;
        }
        if (mRecipes[i].mIngredientIcon != 0) {
            ReleaseTexture(mRecipes[i].mIngredientIcon);
            mRecipes[i].mIngredientIcon = 0;
        }
        if (mRecipes[i].mIngredientRequest != 0) {
            if (!fn_80109A0C(mRecipes[i].mIngredientRequest)) {
                fn_801098DC(mRecipes[i].mIngredientRequest);
            }
            mRecipes[i].mIngredientRequest = 0;
        }
        if (mRecipes[i].mPartIcon != 0) {
            ReleaseTexture(mRecipes[i].mPartIcon);
            mRecipes[i].mPartIcon = 0;
        }
        if (mRecipes[i].mPartRequest != 0) {
            if (!fn_80109A0C(mRecipes[i].mPartRequest)) {
                fn_801098DC(mRecipes[i].mPartRequest);
            }
            mRecipes[i].mPartRequest = 0;
        }
    }
}

// NonMatching (94.6 %): the inlined copy of fn_80120990 has the same gap (and the
// frame is 0x10 bytes smaller).
void Recipes::vf0C() {
    mCount = 0;
    mCursor = 0;
    mLoaded = 0;
    if (mType == -1) {
        mTypeFilter = 3;
        mElement = 5;
    }
    m8 = 5;
    mPage = 0;
    lbl_805FB2B0 = 0;
    mSelected = -1;
    UpdateTab();
    for (int breed = 0; breed < 156; breed++) {
        for (u8 slot = 1; slot <= 4; slot++) {
            if (!fn_8022397C(lbl_805FBBE0, breed, slot)) {
                continue;
            }
            UnkBreedInfo info = lbl_805FBBC0->mData->mBreeds[breed];
            UnkMonsterDesc desc = fn_80216838(lbl_805FBBC0, info.mDescId);
            if (mTypeFilter != 3 && info.mType != mTypeFilter) {
                continue;
            }
            if (mElement != 5 && desc.mElement != mElement) {
                if ((u32)(desc.mElement - 2) > 1) {
                    continue;
                }
                if (mElement != 2 && mElement != 3) {
                    continue;
                }
            }
            mRecipes[mCount].mName = fn_8011AEE8(&desc, slot);
            mRecipes[mCount].mSlot = slot;
            mRecipes[mCount].mIconRequest = fn_8011AE30(&desc);
            mRecipes[mCount].mType = desc.mType;
            mRecipes[mCount].mElement = desc.mElement;
            mRecipes[mCount].mBreed = breed;
            mRecipes[mCount].mDescId = info.mDescId;
            UnkIngredientInfo ingredient = lbl_805FBBC0->mData->mIngredients[info.mIngredient];
            mRecipes[mCount].mIngredientRequest = fn_801189C4(info.mIngredient);
            if (fn_80109A0C(mRecipes[mCount].mIngredientRequest)) {
                mRecipes[mCount].mIngredientIcon = mRecipes[mCount].mIngredientRequest;
                mRecipes[mCount].mIngredientRequest = 0;
            }
            mRecipes[mCount].mIngredientName = GetText(ingredient.mNameHash);
            int part = fn_80120990(info.mType, info.mPartKey, slot);
            if (part != -1) {
                mRecipes[mCount].mOwned = fn_802241F0(lbl_805FBBE0, info.mId, slot);
            } else {
                mRecipes[mCount].mOwned = 0;
            }
            if (mRecipes[mCount].mOwned != 0) {
                UnkPartInfo partInfo = lbl_805FBBC0->mData->mParts[part];
                mRecipes[mCount].mPartRequest = fn_80118B1C(partInfo.mId);
                if (fn_80109A0C(mRecipes[mCount].mPartRequest)) {
                    mRecipes[mCount].mPartIcon = mRecipes[mCount].mPartRequest;
                    mRecipes[mCount].mPartRequest = 0;
                }
                mRecipes[mCount].mPartName = GetText(partInfo.mNameHash);
            } else {
                mRecipes[mCount].mPartIcon = 0;
            }
            mCount++;
        }
    }
}

Recipes::~Recipes() {}

Recipes::Recipes() : mCursor(0), mCount(0), mType(-1), mTypeFilter(3), mElement(5) {
    for (int i = 0; i < 636; i++) {
        mShown[i] = 0;
        mShownIcons[i] = 0;
    }
}
