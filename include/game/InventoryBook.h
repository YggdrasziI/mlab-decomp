#ifndef GAME_INVENTORYBOOK_H
#define GAME_INVENTORYBOOK_H

// Class "InventoryBook" (RTTI) and its four sections "Creatures", "Ingredients",
// "IngredientSelection" and "Parts" (RTTI names; sizes from the allocations in the
// InventoryBook constructor). Creatures is game unit 0x80117090; the other sections'
// functions are at 0x80117AE4-0x8011A3BC and from 0x8011A778 (units not split yet).
// Real file names unknown.

#include "game/book.h"
#include "game/section.h"

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

class Ingredients : public section {
public:
    Ingredients();
    virtual ~Ingredients();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();

    u8 unk[0x14E0 - sizeof(section)];
};

class IngredientSelection : public section {
public:
    IngredientSelection();
    virtual ~IngredientSelection();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();

    u8 unk[0x1C - sizeof(section)];
};

class Parts : public section {
public:
    Parts();
    virtual ~Parts();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();

    u8 unk[0x2DE8 - sizeof(section)];
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
