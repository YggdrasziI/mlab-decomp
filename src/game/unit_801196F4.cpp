// Game unit at 0x801196F4-0x8011A3BC (real file name unknown): the IngredientSelection
// section of the InventoryBook (RTTI). The section itself does nothing; its static
// functions hold the ingredient and the part chosen for a recipe, their names and
// icons, the monster they make with its icon, name and statistic for the part's slot,
// and show them on the screen. Bounds come from the static initializers of this unit
// and the previous one.

#include "game/unk_vec3.mch"
#include "game/InventoryBook.h"
#include "game/Journal.h"
#include "game/unk_text.h"
#include "game/unk_vec3.h"

// Inventory entry of the player (0xC bytes, real type unknown; see unit_80117AE4.cpp).
struct UnkStatItem {
    /* 0x0 */ int mKind; // 0: ingredient, 1: part
    /* 0x4 */ int mIndex;
    /* 0x8 */ u8 mCount;
};

struct UnkSelectionPlayer {
    /* 0x000 */ u8 unk0[0xA30];
    /* 0xA30 */ u8 mA30[0xBE8 - 0xA30];
    /* 0xBE8 */ UnkStatItem mItems[105];
};

// Monster description returned by the database (0x50 bytes, real type unknown).
struct UnkMonsterDesc {
    /* 0x00 */ int mId;
    /* 0x04 */ u32 mNameHashes[4]; // one per slot
    /* 0x14 */ u8 unk14[0x40 - 0x14];
    /* 0x40 */ s16 m40;
    /* 0x42 */ s16 m42;
    /* 0x44 */ s16 m44;
    /* 0x46 */ u8 unk46[0x48 - 0x46];
    /* 0x48 */ s16 m48;
    /* 0x4A */ u8 unk4A[0x4C - 0x4A];
    /* 0x4C */ s8 m4C;
    /* 0x4D */ u8 unk4D[0x50 - 0x4D];
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

struct UnkSelectionData {
    /* 0x0000 */ u8 unk0[0x4F00];
    /* 0x4F00 */ UnkIngredientInfo* mIngredients;
    /* 0x4F04 */ u8 unk4F04[0x5328 - 0x4F04];
    /* 0x5328 */ UnkPartInfo* mParts;
    /* 0x532C */ u8 unk532C[0x6C18 - 0x532C];
    /* 0x6C18 */ UnkBreedInfo* mBreeds;
};

struct UnkSelectionDatabase {
    /* 0x0 */ u8 unk0[0xC];
    /* 0xC */ UnkSelectionData* mData;
};

// Lab state the selection is reported to (real type unknown).
struct UnkLab {
    /* 0x00 */ u8 unk0[0x58];
    /* 0x58 */ int mBreed;
};

extern "C" {
extern UnkSelectionPlayer* lbl_805FBBE0;
extern UnkSelectionDatabase* lbl_805FBBC0;
extern UnkLab* lbl_805FBBC8;

void fn_8018F618(u32 hash, int hidden);
void fn_801903CC(u32 hash, u32 color);
void fn_80190AFC(u32 hash);
void fn_80190BC8(u32 hash);
void fn_80191B68(u32 hash, UnkTexture* texture);
void fn_80195348(u32 hash, u32 target);
UnkMonsterDesc fn_80216838(UnkSelectionDatabase* db, int id);
int fn_8021828C(UnkLab* lab);
void fn_80218588(UnkLab* lab, int ingredient);
void fn_802186D0(UnkLab* lab, int part);
void fn_802188A8(UnkLab* lab, int monster);
int fn_80219030(UnkLab* lab);
void fn_802192B8(UnkLab* lab);
void fn_80219ED4(void* selection, int index, int kind, int arg);
int fn_802241F0(void* player, u32 breed, u8 slot);
int fn_8022397C(void* player, u32 breed, u8 slot);
// Monster icon (Parts section).
UnkTexture* fn_8011AD88(const UnkMonsterDesc* desc, int highlighted);
}

// Chosen ingredient and part (real names unknown).
UnkStatItem lbl_803E6B28;
UnkStatItem lbl_803E6B34;

// Current monster and the lab's state (real names unknown).
int lbl_805F8F20 = -1;
int lbl_805F8F24 = -1;

// Statistic of the monster for the part's slot, text indices of the names and icons
// (real names unknown).
s16 lbl_805FB268;
u32 lbl_805FB26C;
u32 lbl_805FB270;
u32 lbl_805FB274;
u32 lbl_805FB278;
UnkTexture* lbl_805FB27C;
UnkTexture* lbl_805FB280;
UnkTexture* lbl_805FB284;
u32 lbl_805FB288;
u32 lbl_805FB28C;
u8 lbl_805FB290;
u8 lbl_805FB291;

IngredientSelection::IngredientSelection() {}

IngredientSelection::~IngredientSelection() {}

void IngredientSelection::vf0C() {}

void IngredientSelection::vf10() {}

void IngredientSelection::vf14() {}

void IngredientSelection::vf18() {}

void IngredientSelection::fn_80119798(int monster) {
    lbl_805F8F20 = monster;
    lbl_803E6B28.mIndex = -1;
    lbl_803E6B28.mKind = 0;
    lbl_803E6B34.mIndex = -1;
    lbl_803E6B34.mKind = 1;
    lbl_805FB290 = 1;
    lbl_805FB291 = 0;
    fn_8011A04C();
    if (lbl_805FBBC8 != 0) {
        fn_802188A8(lbl_805FBBC8, monster);
    }
}

void IngredientSelection::fn_80119810(UnkInventoryItem* item) {
    lbl_803E6B28.mIndex = item->mIndex;
    if (lbl_805FBBC8 != 0) {
        fn_80218588(lbl_805FBBC8, item->mIndex);
    }
    fn_8011A04C();
}

void IngredientSelection::fn_80119858(UnkInventoryItem* item) {
    lbl_803E6B34.mIndex = item->mIndex;
    if (lbl_805FBBC8 != 0) {
        fn_802186D0(lbl_805FBBC8, item->mIndex);
    }
    fn_8011A04C();
}

void IngredientSelection::fn_801198A0(int breed, int slot) {
    UnkBreedInfo info = lbl_805FBBC0->mData->mBreeds[breed];
    for (int i = 0; i < 105 && lbl_803E6B28.mIndex == -1; i++) {
        UnkStatItem* item = &lbl_805FBBE0->mItems[i];
        if (item->mIndex == info.mIngredient && item->mKind == 0) {
            if (item->mCount != 0) {
                lbl_803E6B28.mIndex = i;
                if (lbl_805FBBC8 != 0) {
                    fn_80218588(lbl_805FBBC8, i);
                }
            }
            i = 105;
        }
    }
    if (lbl_803E6B28.mIndex != -1 && fn_802241F0(lbl_805FBBE0, breed, slot)) {
        int part = 60;
        for (int i = 0; i < 60; i++) {
            UnkPartInfo info2 = lbl_805FBBC0->mData->mParts[i];
            if (info2.mSlot == slot && info2.mType == info.mType && info2.mKey == info.mPartKey) {
                part = i;
                i = 60;
            }
        }
        for (int i = 0; i < 105 && lbl_803E6B34.mIndex == -1; i++) {
            UnkStatItem* item = &lbl_805FBBE0->mItems[i];
            if (item->mIndex == part && item->mKind == 1) {
                if (item->mCount != 0) {
                    lbl_803E6B34.mIndex = part;
                    if (lbl_805FBBC8 != 0) {
                        fn_802186D0(lbl_805FBBC8, part);
                    }
                }
                i = 105;
            }
        }
    }
    fn_8011A04C();
}

void IngredientSelection::fn_80119AE0() {
    lbl_803E6B28.mIndex = -1;
    lbl_803E6B34.mIndex = -1;
    fn_8011A04C();
}

int IngredientSelection::fn_80119B00() {
    if (lbl_803E6B28.mIndex == -1 || lbl_803E6B34.mIndex == -1) {
        return 0;
    }
    return 1;
}

int IngredientSelection::fn_80119B38() {
    if (lbl_805FBBC8 != 0 && fn_80219030(lbl_805FBBC8)) {
        return 0;
    }
    return 1;
}

int IngredientSelection::fn_80119B78() {
    return lbl_805F8F20 != -1;
}

int IngredientSelection::fn_80119B90() {
    return lbl_805FB290 != 1;
}

void IngredientSelection::fn_80119BA8() {
    lbl_805F8F20 = -1;
    lbl_803E6B28.mIndex = -1;
    lbl_803E6B34.mIndex = -1;
    fn_80218588(lbl_805FBBC8, -1);
    fn_802186D0(lbl_805FBBC8, -1);
}

void IngredientSelection::fn_80119BFC() {
    fn_80219ED4(lbl_805FBBE0->mA30, lbl_803E6B28.mIndex, lbl_803E6B28.mKind, 1);
    fn_80219ED4(lbl_805FBBE0->mA30, lbl_803E6B34.mIndex, lbl_803E6B34.mKind, 1);
}

void IngredientSelection::fn_80119C58() {
    lbl_805F8F20 = -1;
    lbl_805FB290 = 0;
}

void IngredientSelection::fn_80119C6C() {
    fn_8018F618(0xFB02A3B7, 0);
    if (lbl_805FB27C != 0) {
        fn_80191B68(0xB6B6D325, lbl_805FB27C);
        fn_8018F618(0x06BE9A4F, 0);
    } else {
        fn_8018F618(0xB6B6D325, 0);
        fn_8018F618(0x06BE9A4F, 1);
    }
    if (lbl_805FB280 != 0) {
        fn_80191B68(0xFF3A8971, lbl_805FB280);
        fn_8018F618(0xB2ACFB4F, 0);
    } else {
        fn_8018F618(0xFF3A8971, 0);
        fn_8018F618(0xB2ACFB4F, 1);
    }
    if (lbl_805FB284 != 0) {
        fn_80191B68(0x7F9A9D72, lbl_805FB284);
        fn_8018F618(0x96558200, 0);
    } else {
        fn_8018F618(0x7F9A9D72, 0);
        fn_8018F618(0x96558200, 1);
    }
    if (lbl_805FB27C != 0 && lbl_805FB280 != 0) {
        fn_80195348(0xEBFC9568, 0x0165E8A7);
        fn_80190BC8(0x0165E8A7);
    } else {
        fn_80195348(0x7CBE0C37, 0x0165E8A7);
        fn_80190AFC(0x0165E8A7);
    }
    if (fn_80119B38()) {
        fn_80195348(0xC86AAC8F, 0x7E1BF2E2);
        fn_801903CC(0x1C74EC52, 0x8FD4A14F);
    } else {
        fn_80195348(0xE96F9947, 0x7E1BF2E2);
        fn_801903CC(0x1C74EC52, 0xF07F4EDE);
    }
    switch (lbl_805F8F24) {
    case 0:
        fn_801903CC(0x89CCAE46, 0x121736A5);
        fn_801903CC(0x10C5FFFC, 0x65A15F69);
        fn_801903CC(0x67C2CF6A, 0x65A15F69);
        fn_801903CC(0xF9A65AC9, 0x65A15F69);
        break;
    case 1:
        fn_801903CC(0x89CCAE46, 0x8B1E671F);
        fn_801903CC(0x10C5FFFC, 0x8B1E671F);
        fn_801903CC(0x67C2CF6A, 0x65A15F69);
        fn_801903CC(0xF9A65AC9, 0x65A15F69);
        break;
    case 2:
        fn_801903CC(0x89CCAE46, 0xFC195789);
        fn_801903CC(0x10C5FFFC, 0xFC195789);
        fn_801903CC(0x67C2CF6A, 0xFC195789);
        fn_801903CC(0xF9A65AC9, 0x65A15F69);
        break;
    case 3:
        fn_801903CC(0x89CCAE46, 0x627DC22A);
        fn_801903CC(0x10C5FFFC, 0x627DC22A);
        fn_801903CC(0x67C2CF6A, 0x627DC22A);
        fn_801903CC(0xF9A65AC9, 0x627DC22A);
        break;
    default:
        fn_801903CC(0x89CCAE46, 0x65A15F69);
        fn_801903CC(0x10C5FFFC, 0x65A15F69);
        fn_801903CC(0x67C2CF6A, 0x65A15F69);
        fn_801903CC(0xF9A65AC9, 0x65A15F69);
        break;
    }
}

int IngredientSelection::fn_80119FF4() {
    return lbl_805F8F20;
}

u32 IngredientSelection::fn_80119FFC() {
    return lbl_805FB26C;
}

u32 IngredientSelection::fn_8011A004() {
    return lbl_805FB270;
}

u32 IngredientSelection::fn_8011A00C() {
    return lbl_805FB274;
}

u32 IngredientSelection::fn_8011A014() {
    return lbl_805FB278;
}

void IngredientSelection::fn_8011A01C() {
    lbl_805FB291 = 1;
}

void IngredientSelection::fn_8011A028() {
    lbl_805FB291 = 0;
}

u8 IngredientSelection::fn_8011A034() {
    return lbl_805FB291;
}

void IngredientSelection::fn_8011A03C(u32 value) {
    lbl_805FB28C = value;
}

u32 IngredientSelection::fn_8011A044() {
    return lbl_805FB28C;
}

void IngredientSelection::fn_8011A04C() {
    lbl_805FB268 = 0;
    lbl_805F8F24 = -1;
    lbl_805FB26C = 0;
    lbl_805FB270 = 0;
    lbl_805FB274 = 0;
    lbl_805FB278 = 0;
    lbl_805FB27C = 0;
    lbl_805FB280 = 0;
    lbl_805FB284 = 0;
    lbl_805FB288 = 0;
    if (lbl_803E6B28.mIndex != -1 && lbl_803E6B34.mIndex != -1) {
        UnkIngredientInfo ingredient = lbl_805FBBC0->mData->mIngredients[lbl_803E6B28.mIndex];
        UnkPartInfo part = lbl_805FBBC0->mData->mParts[lbl_803E6B34.mIndex];
        int breed = lbl_805FBBC8->mBreed;
        lbl_805FB26C = fn_80120D34(ingredient.mNameHash);
        lbl_805FB270 = fn_80120D34(part.mNameHash);
        lbl_805FB27C = Ingredients::fn_801186FC(&lbl_803E6B28);
        lbl_805FB280 = Ingredients::fn_801186FC(&lbl_803E6B34);
        UnkBreedInfo info = lbl_805FBBC0->mData->mBreeds[breed];
        UnkMonsterDesc desc = fn_80216838(lbl_805FBBC0, info.mDescId);
        s16 stats[4] = {desc.m44, desc.m42, desc.m40, desc.m48};
        lbl_805FB268 = desc.m4C * stats[part.mSlot - 1] / 100;
        lbl_805F8F24 = fn_8021828C(lbl_805FBBC8);
        if (fn_8022397C(lbl_805FBBE0, breed, part.mSlot)) {
            u32 hashes[4] = {desc.mNameHashes[0], desc.mNameHashes[1], desc.mNameHashes[2],
                             desc.mNameHashes[3]};
            lbl_805FB274 = fn_80120D34(hashes[part.mSlot - 1]);
            lbl_805FB284 = fn_8011AD88(&desc, 0);
        }
        fn_802192B8(lbl_805FBBC8);
    } else if (lbl_803E6B28.mIndex != -1) {
        UnkIngredientInfo ingredient = lbl_805FBBC0->mData->mIngredients[lbl_803E6B28.mIndex];
        lbl_805FB26C = fn_80120D34(ingredient.mNameHash);
        lbl_805FB27C = Ingredients::fn_801186FC(&lbl_803E6B28);
    } else if (lbl_803E6B34.mIndex != -1) {
        UnkPartInfo part = lbl_805FBBC0->mData->mParts[lbl_803E6B34.mIndex];
        lbl_805FB270 = fn_80120D34(part.mNameHash);
        lbl_805FB280 = Ingredients::fn_801186FC(&lbl_803E6B34);
    }
}
