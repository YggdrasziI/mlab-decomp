#ifndef GAME_JOURNAL_H
#define GAME_JOURNAL_H

// Class "Journal" (RTTI) and its four sections "Badges", "Statistics", "QuestLog"
// and "Recipes" (RTTI names; sizes from the allocations in the Journal
// constructor). Badges is game unit 0x8011C460, Statistics 0x8011D5C4, QuestLog
// 0x8011EA20 and Recipes 0x8011F410. Real file names unknown.

#include "game/book.h"
#include "game/section.h"
#include "game/UnkProgressBar.h"

// Texture returned by fn_80109330 (real type unknown; reference counted).
struct UnkTexture {
    /* 0x00 */ u8 unk0[0x24];
    /* 0x24 */ int mRefCount;
};

// Owned badge shown by the Badges section (real type unknown).
struct UnkBadge {
    UnkBadge() : mNameHash(0), mName(0), mDescHash(0), mDesc(0), mCategory(-1), mTexture(0), mRequest(0) {}
    void Init(const struct UnkBadgeInfo& info);

    /* 0x00 */ u32 mNameHash;
    /* 0x04 */ const char* mName;
    /* 0x08 */ u32 mDescHash;
    /* 0x0C */ const char* mDesc;
    /* 0x10 */ int mCategory;
    /* 0x14 */ UnkTexture* mTexture;
    /* 0x18 */ UnkTexture* mRequest; // texture still loading
};

// Badges counted in a category (index 3: all categories).
struct UnkBadgeCount {
    /* 0x0 */ int mTotal;
    /* 0x4 */ int mOwned;
};

// Unit 0x8011C460 (game/unit_8011C460.cpp).
class Badges : public section {
public:
    Badges();
    virtual ~Badges();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();

    static UnkTexture* fn_8011CC5C(int index);
    u8 fn_8011CD10();
    void fn_8011CD18();
    void fn_8011CDD4(int rows);
    void fn_8011CE04(int delta);
    int fn_8011CF64(int index);
    UnkTexture* fn_8011CF8C(int index);
    const char* fn_8011CFC0(int index);
    const char* fn_8011CFF4(int index);
    void fn_8011D028(u32 topHash, u32 bottomHash);
    void fn_8011D030(u32 hash);
    const char* fn_8011D038();
    const char* fn_8011D150();

    // Reads the member back: the DOL keeps the dead branch after a constant category.
    void SetCategory(int category) {
        mCategory = category;
        m4 = mCategory;
        if (mCategory == 3) {
            m4 = 1;
        } else {
            m4 = mCategory + 2;
        }
    }

    /* 0x001C */ UnkBadge mBadges[150];
    /* 0x1084 */ int mShownCount;
    /* 0x1088 */ UnkBadge* mShown[153];
    /* 0x12EC */ int mCursor;
    /* 0x12F0 */ int mCount;
    /* 0x12F4 */ int mCategory; // 3: all
    /* 0x12F8 */ UnkBadgeCount mCounts[4];
    /* 0x1318 */ UnkProgressBar mProgress;
    /* 0x1344 */ char mCountText[8];
};

// Unit 0x8011D5C4 (game/unit_8011D5C4.cpp): four tabs of player statistics.
class Statistics : public section {
public:
    Statistics();
    virtual ~Statistics();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();

    void fn_8011D69C(int delta);
    void fn_8011D6F4();
    void fn_8011D708();
    u32 fn_8011D71C();
    u32 fn_8011D728();
    const char* fn_8011D79C();
    u32 fn_8011D894();
    int fn_8011D930();
    u32 fn_8011D97C();
    u32 fn_8011D988();
    u32 fn_8011D994();
    u32 fn_8011D9A0();
    u32 fn_8011D9A8();
    u32 fn_8011D9B4();
    void fn_8011D9C0();
    void fn_8011DF20();
    void fn_8011E440();
    int fn_8011E94C();
    int fn_8011E95C();
    int fn_8011E970();
    int fn_8011E984();
    int fn_8011E998();
    void fn_8011E9A8(u32 topHash, u32 bottomHash);
    void fn_8011E9B0(u32 hash);
    void fn_8011E9B8();

    /* 0x1C */ int mTab;
    /* 0x20 */ int mPage;
    /* 0x24 */ UnkProgressBar mProgress;
};

// Quest of the quest manager (real type unknown).
struct UnkQuest {
    /* 0x00 */ u32 mNameHash;
    /* 0x04 */ u8 unk4[0x8 - 0x4];
    /* 0x08 */ int mState;
    /* 0x0C */ int mProgress;
    /* 0x10 */ int mGoal;
};

// Unit 0x8011EA20 (game/unit_8011EA20.cpp).
class QuestLog : public section {
public:
    QuestLog();
    virtual ~QuestLog();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();

    static UnkTexture* fn_8011ECCC(s8 icon);
    static UnkTexture* fn_8011ED38(s8 icon);
    int fn_8011EDAC();
    const char* fn_8011EDE0();
    const char* fn_8011EEF4();
    int fn_8011EFB8();
    int fn_8011EFE0(int index);
    int fn_8011EFFC(int index);
    int fn_8011F080(int index);
    char* fn_8011F104(int index);
    int fn_8011F1C4();
    UnkTexture* fn_8011F1D8();
    const char* fn_8011F1E0(int index);
    int fn_8011F2FC();
    const char* fn_8011F32C();

    /* 0x1C */ char mProgressText[3][5];
    /* 0x2B */ u8 mHasMainQuest;
    /* 0x2C */ int mQuestCount;
    /* 0x30 */ UnkTexture* mIcon;
};

// Recipe shown by the Recipes section (0x3C bytes, real type unknown): a monster
// breed for one slot, with three icons loaded in the background.
struct UnkRecipe {
    UnkRecipe()
        : mIcon(0), mIconRequest(0), mName(0), mType(-1), mElement(-1), mBreed(0), mDescId(-1),
          mIngredientIcon(0), mIngredientRequest(0), mIngredientName(0), mPartIcon(0),
          mPartRequest(0), mPartName(0), mOwned(0), mSlot(0.0f) {}

    /* 0x00 */ UnkTexture* mIcon;
    /* 0x04 */ UnkTexture* mIconRequest;
    /* 0x08 */ const char* mName;
    /* 0x0C */ int mType;
    /* 0x10 */ int mElement;
    /* 0x14 */ int mBreed;
    /* 0x18 */ int mDescId;
    /* 0x1C */ UnkTexture* mIngredientIcon;
    /* 0x20 */ UnkTexture* mIngredientRequest;
    /* 0x24 */ const char* mIngredientName;
    /* 0x28 */ UnkTexture* mPartIcon;
    /* 0x2C */ UnkTexture* mPartRequest;
    /* 0x30 */ const char* mPartName;
    /* 0x34 */ u32 mOwned;
    /* 0x38 */ f32 mSlot;
};

// Unit 0x8011F410 (game/unit_8011F410.cpp): the recipes of the monster breeds, by
// type (3: all) and element (5: all), four per page.
class Recipes : public section {
public:
    Recipes();
    virtual ~Recipes();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();

    void fn_8011FF28();
    UnkTexture** fn_80120054();
    void fn_8012006C(int index);
    int fn_80120084();
    const char* fn_80120098();
    const char* fn_801201B4();
    const char* fn_8012029C(int index);
    int fn_80120390();
    u32 fn_801203C0();
    int fn_801203F0();
    UnkTexture* fn_80120418();
    UnkTexture* fn_80120444();
    const char* fn_80120474();
    UnkTexture* fn_801204A4();
    const char* fn_801204D4();
    int fn_80120504();
    void fn_80120534(int delta);
    void fn_801205EC(int rows);
    int fn_801206A0(int type);
    int fn_8012078C(int index);
    int fn_801207B8(int index);
    void fn_801207D0(int index);
    void fn_801207DC(int type);
    void fn_801208F8(u32 hash);
    int fn_8012096C();
    void fn_80120978(u32 topHash, u32 bottomHash);
    void fn_80120984(u32 hash);
    static int fn_80120990(int type, int key, int slot);
    u8 fn_80120A84();

    // Reads the member back: the DOL keeps the element in a register.
    void UpdateTab() {
        m4 = mElement;
        if (mElement == 5) {
            m4 = 1;
        } else if (mElement < 3) {
            m4 = mElement + 2;
        } else {
            m4 = mElement + 1;
        }
    }

    /* 0x001C */ UnkRecipe mRecipes[624];
    /* 0x925C */ int mCursor;
    /* 0x9260 */ int mCount;
    /* 0x9264 */ UnkRecipe* mShown[636];
    /* 0x9C54 */ UnkTexture* mShownIcons[636];
    /* 0xA644 */ int mShownCount;
    /* 0xA648 */ int mPage;
    /* 0xA64C */ int mType;
    /* 0xA650 */ int mTypeFilter;
    /* 0xA654 */ int mElement;
    /* 0xA658 */ UnkProgressBar mProgress;
    /* 0xA684 */ u32 mLoaded;
    /* 0xA688 */ int mSelected;
};

class Journal : public book {
public:
    Journal();
    virtual ~Journal();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual int vf1C(u32 hash);
    virtual void vf20(u32 hash);
    virtual int vf24(u32 hash);
    virtual section* vf28(u32 hash);

    /* 0x08 */ section* mSections[4];
    /* 0x18 */ int mCurrent;
    /* 0x1C */ u8 mOpen;
};

#endif // GAME_JOURNAL_H
