#ifndef GAME_INVENTORYBOOK_H
#define GAME_INVENTORYBOOK_H

// Class "InventoryBook" (RTTI) and its four sections "Creatures", "Ingredients",
// "IngredientSelection" and "Parts" (RTTI names; sizes from the allocations in the
// InventoryBook constructor). The sections' functions are at 0x80117090-0x8011A3BC
// and from 0x8011A778 (units not split yet). Real file names unknown.

#include "game/book.h"
#include "game/section.h"

class Creatures : public section {
public:
    Creatures();
    virtual ~Creatures();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();

    u8 unk[0x54 - sizeof(section)];
};

class Ingredients : public section {
public:
    Ingredients();
    virtual ~Ingredients();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();

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
    virtual void vf1C();

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
    virtual void vf1C();

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
