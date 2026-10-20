#ifndef GAME_INVENTORYBOOK_H
#define GAME_INVENTORYBOOK_H

// Class "InventoryBook" (RTTI) and its four sections "Creatures", "Ingredients",
// "IngredientSelection" and "Parts" (RTTI names; sizes from the allocations in the
// InventoryBook constructor). Creatures is game unit 0x80117090; the other sections'
// functions are at 0x80117AE4-0x8011A3BC and 0x8011A778-0x8011C460.
// Real file names unknown.

#include "game/book.h"
#include "game/section.h"
#include "game/UnkProgressBar.h"

class UnkActor;
struct UnkStands;

// Creatures section (game unit 0x80117090): pages through the player's creatures and
// shows the current one on the stands model.
class Creatures : public section {
public:
    Creatures();
    virtual ~Creatures();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();

    void fn_8011726C(int delta);
    void fn_80117320(int index);
    f32 fn_801173A0();
    f32 fn_801173B8();
    f32 fn_801173D0();
    f32 fn_801173E8();
    f32 fn_80117400();
    f32 fn_80117418();
    void fn_80117430();
    void fn_80117844();
    int fn_80117948();
    const char* fn_80117970();
    int fn_801179C0();
    void fn_801179C8();
    int fn_80117ABC();

    /* 0x1C */ u8 unk1C[0x20 - 0x1C];
    /* 0x20 */ void* mCreature;
    /* 0x24 */ UnkActor* mActor;
    /* 0x28 */ UnkStands* mStands;
    /* 0x2C */ int mIndex;
    /* 0x30 */ int mShownIndex;
    /* 0x34 */ int mCount;
    /* 0x38 */ int mShown;
    /* 0x3C */ int mOpened;
    /* 0x40 */ char mName[0x10];
    /* 0x50 */ int mTimer;
};

struct UnkTexture;
struct UnkStatItem;

// Item of the player's inventory shown by the Ingredients section (0x20 bytes, real
// type unknown).
struct UnkInventoryItem {
    UnkInventoryItem() : mType(0), mKind(-1), mIndex(0), mIcon(0), mRequest(0), mNameHash(0), mName(0) {}

    /* 0x00 */ int mType;
    /* 0x04 */ int mKind; // 0: ingredient, 1: part
    /* 0x08 */ int mIndex;
    /* 0x0C */ UnkTexture* mIcon;
    /* 0x10 */ UnkTexture* mRequest; // icon still loading
    /* 0x14 */ u32 mNameHash;
    /* 0x18 */ const char* mName;
    /* 0x1C */ char mCountText[4];
};

// Ingredients section (game unit 0x80117AE4): the ingredients and parts the player
// owns, by tab, filtered by type and paged three rows of four at a time.
class Ingredients : public section {
public:
    Ingredients();
    virtual ~Ingredients();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();

    void fn_8011813C();
    static UnkTexture* fn_801186FC(const UnkStatItem* item);
    static UnkTexture* fn_80118858(const UnkStatItem* item);
    static UnkTexture* fn_80118A6C(int part);
    u8 fn_80118BD4();
    void fn_80118BDC();
    UnkInventoryItem* fn_80118C24();
    int fn_80118C54(int type, int tab);
    void fn_80118E88(int type, int tab);
    void fn_80118ED4(u32 hash);
    int fn_80118F3C();
    const char* fn_80118F44();
    void fn_80118FC0(int delta);
    void fn_801190E8(int rows);
    int fn_801191AC();
    bool fn_801191C0();
    void fn_801191C8();
    void fn_801194E0(int index);
    int fn_80119538();
    void fn_8011954C(int delta);
    void fn_801195F0(u32 topHash, u32 bottomHash);
    void fn_801195F8(u32 hash);
    int fn_80119600(int index);
    UnkTexture** fn_80119630();
    char** fn_80119644();
    void** fn_80119658();
    const char* fn_8011966C();
    UnkTexture* fn_801196A8();

    void ReleaseIcons();
    void ScrollToRow(int row);

    /* 0x001C */ int mType;
    /* 0x0020 */ int mTypeFilter; // 3: all
    /* 0x0024 */ u8 unk24[0x28 - 0x24];
    /* 0x0028 */ int mTab;
    /* 0x002C */ int mCursor;
    /* 0x0030 */ int mPage;
    /* 0x0034 */ int mCount;
    /* 0x0038 */ UnkInventoryItem mItems[105];
    /* 0x0D58 */ int mShownCount;
    /* 0x0D5C */ UnkInventoryItem* mShown[117];
    /* 0x0F30 */ UnkTexture* mShownIcons[117];
    /* 0x1104 */ char* mShownTexts[117];
    /* 0x12D8 */ void* mShownMarks[117];
    /* 0x14AC */ UnkProgressBar mProgress;
    /* 0x14D8 */ bool mWasPartsTab;
    /* 0x14DC */ u32 mLoaded;
};

// IngredientSelection section (game unit 0x801196F4): no state of its own; its static
// functions hold the ingredient and the part chosen for a recipe, their names and
// icons, and the monster they make.
class IngredientSelection : public section {
public:
    IngredientSelection();
    virtual ~IngredientSelection();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();

    static void fn_80119798(int monster);
    static void fn_80119810(UnkInventoryItem* item);
    static void fn_80119858(UnkInventoryItem* item);
    static void fn_801198A0(int breed, int slot);
    static void fn_80119AE0();
    static int fn_80119B00();
    static int fn_80119B38();
    static int fn_80119B78();
    static int fn_80119B90();
    static void fn_80119BA8();
    static void fn_80119BFC();
    static void fn_80119C58();
    static void fn_80119C6C();
    static int fn_80119FF4();
    static u32 fn_80119FFC();
    static u32 fn_8011A004();
    static u32 fn_8011A00C();
    static u32 fn_8011A014();
    static void fn_8011A01C();
    static void fn_8011A028();
    static u8 fn_8011A034();
    static void fn_8011A03C(u32 value);
    static u32 fn_8011A044();
    static void fn_8011A04C();
};

struct UnkPartRecord;

// Part shown by the Parts section (0x60 bytes, real type unknown): the monster it
// comes from, the ingredient and part of its recipe, with icons loaded in the
// background.
struct UnkPartItem {
    UnkPartItem()
        : mIcon(0), mRequest(0), mMark(0), mNewMark(0), mIngredientIcon(0), mIngredientRequest(0),
          mIngredientName(0), mPartIcon(0), mPartRequest(0), mPartName(0), mNameHash(0), mName(0),
          m30(0), m34(0), m38(0), mType(-1), mElement(-1), mAbilityHash(0), mAbilityName(0),
          mAbility2Hash(0), mAbility2Name(0), mIndex(0), mBreed(-1), mHasPart(0) {}

    /* 0x00 */ UnkTexture* mIcon;
    /* 0x04 */ UnkTexture* mRequest; // icon still loading
    /* 0x08 */ void* mMark;
    /* 0x0C */ void* mNewMark;
    /* 0x10 */ UnkTexture* mIngredientIcon;
    /* 0x14 */ UnkTexture* mIngredientRequest;
    /* 0x18 */ const char* mIngredientName;
    /* 0x1C */ UnkTexture* mPartIcon;
    /* 0x20 */ UnkTexture* mPartRequest;
    /* 0x24 */ const char* mPartName;
    /* 0x28 */ u32 mNameHash;
    /* 0x2C */ const char* mName;
    /* 0x30 */ int m30;
    /* 0x34 */ int m34;
    /* 0x38 */ int m38;
    /* 0x3C */ int mType;
    /* 0x40 */ int mElement;
    /* 0x44 */ u32 mAbilityHash;
    /* 0x48 */ const char* mAbilityName;
    /* 0x4C */ u32 mAbility2Hash;
    /* 0x50 */ const char* mAbility2Name;
    /* 0x54 */ int mIndex;
    /* 0x58 */ int mBreed;
    /* 0x5C */ int mHasPart;
};

// Parts section (game unit 0x8011A778): the parts the player owns, filtered by type and
// element and paged three rows of four at a time.
class Parts : public section {
public:
    Parts();
    virtual ~Parts();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();

    static u8 fn_8011AEE0();
    const char* fn_8011AFEC();
    static void fn_8011B060(UnkPartRecord* record, UnkPartItem* item);
    void fn_8011B898();
    void fn_8011BA48(int index);
    int fn_8011BA5C();
    void fn_8011BA6C(u32 topHash, u32 bottomHash);
    void fn_8011BA74(u32 hash);
    UnkTexture** fn_8011BA7C();
    void** fn_8011BA90();
    void** fn_8011BAA4();
    void fn_8011BAB8(u32 hash);
    int fn_8011BB20();
    const char* fn_8011BB28();
    static int fn_8011BC40(int* out, int element);
    void fn_8011BCF8(int delta);
    void fn_8011BDA8(int rows);
    int fn_8011BE4C();
    UnkTexture* fn_8011BE84();
    const char* fn_8011BEB0();
    UnkTexture* fn_8011BEDC();
    const char* fn_8011BF08();
    const char* fn_8011BF34();
    const char* fn_8011C018(int index);
    UnkTexture* fn_8011C108();
    int fn_8011C130();
    int fn_8011C168();
    int fn_8011C1D0(int index);
    int fn_8011C1F8(int index);
    void fn_8011C20C(int index);
    int fn_8011C214();
    void fn_8011C22C();
    int fn_8011C2B4();
    void fn_8011C318();

    void UpdateCategory();

    /* 0x001C */ UnkPartItem mItems[100];
    /* 0x259C */ int mCursor;
    /* 0x25A0 */ int mPage;
    /* 0x25A4 */ int mCount;
    /* 0x25A8 */ UnkPartItem* mShown[112];
    /* 0x2768 */ UnkTexture* mShownIcons[112];
    /* 0x2928 */ void* mShownMarks[112];
    /* 0x2AE8 */ void* mShownNewMarks[112];
    /* 0x2CA8 */ int mShownCount;
    /* 0x2CAC */ int mTypeFilter;  // 3: all
    /* 0x2CB0 */ int mElement;     // 5: all
    /* 0x2CB4 */ UnkProgressBar mProgress;
    /* 0x2CE0 */ char mText[0x100];
    /* 0x2DE0 */ u32 mLoaded;
    /* 0x2DE4 */ int mSelected;
};

class InventoryBook : public book {
public:
    InventoryBook();
    virtual ~InventoryBook();
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
};

#endif // GAME_INVENTORYBOOK_H
