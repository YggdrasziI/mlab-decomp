// Game unit at 0x8011A3BC-0x8011A778 (real file name unknown): the InventoryBook
// class (RTTI), a book of four sections chosen by name hash. Bounds come from the
// static initializers of this unit and the previous one.

#include "game/InventoryBook.h"
#include "game/unk_vec3.h"

InventoryBook::InventoryBook() {
    mCurrent = -1;
    mSections[0] = 0;
    mSections[1] = 0;
    mSections[2] = 0;
    mSections[3] = 0;
    mSections[0] = new Creatures;
    mSections[1] = new Ingredients;
    mSections[2] = new IngredientSelection;
    mSections[3] = new Parts;
}

InventoryBook::~InventoryBook() {
    for (int i = 0; i < 4; i++) {
        if (mSections[i] != 0) {
            delete mSections[i];
            mSections[i] = 0;
        }
    }
}

void InventoryBook::vf0C() {}

void InventoryBook::vf10() {
    if (mCurrent != -1) {
        mSections[mCurrent]->vf10();
    }
    mCurrent = -1;
}

void InventoryBook::vf14() {}

void InventoryBook::vf18() {}

int InventoryBook::vf1C(u32 hash) {
    switch ((int)hash) {
    case (int)0xE5368B8B:
        return 0;
    case (int)0xC66383A7:
        return 1;
    case (int)0xEBD0DA39:
        return 2;
    case (int)0x9E14654E:
        return 3;
    default:
        return -1;
    }
}

void InventoryBook::vf20(u32 hash) {
    if (vf24(hash) == 0) {
        m4 = 0;
        if (mCurrent != -1) {
            mSections[mCurrent]->vf10();
        }
        mCurrent = vf1C(hash);
        if (mCurrent != -1) {
            m4 = mSections[mCurrent];
            m4->vf0C();
        }
    }
}

int InventoryBook::vf24(u32 hash) {
    int index = vf1C(hash);
    return index == mCurrent;
}

section* InventoryBook::vf28(u32 hash) {
    int index = vf1C(hash);
    if (index >= 0) {
        return mSections[index];
    }
    return 0;
}
