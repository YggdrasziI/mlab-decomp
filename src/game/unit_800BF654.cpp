// Game unit at 0x800BF654-0x800BF97C (real file name unknown): the
// Entity_MysticBrewVortex class. Bounds come from the static initializers of this
// unit and the previous one.

#include "game/Entity.h"
#include "game/externs.h"
#include "game/unk_math.h"
#include "game/unk_vec3.h"

// Objects reached by fn_800BF7F0 (real types unknown).
struct UnkVortexItem {
    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ int mC;
    /* 0x10 */ u8 unk10[0x18 - 0x10];
    /* 0x18 */ f32 m18;
    /* 0x1C */ u8 unk1C[0x24 - 0x1C];
    /* 0x24 */ f32 m24;
};

struct UnkVortexGroup {
    /* 0x00 */ u8 unk0[0x18];
    /* 0x18 */ UnkList mItems;
};

Entity_MysticBrewVortex::Entity_MysticBrewVortex(int type, EntityInfo* info, int init)
    : Entity_Prop(type, info, init) {
    mLevel = 50.0f;
    mPrevLevel = 50.0f;
}

Entity_MysticBrewVortex::~Entity_MysticBrewVortex() {}

void Entity_MysticBrewVortex::fn_800BF6FC() {}

void Entity_MysticBrewVortex::fn_800BF700() {
    mLevel = 50.0f;
    mPrevLevel = 50.0f;
}

void Entity_MysticBrewVortex::fn_800BF710(f32 delta) {
    if (delta < 0.0f) {
        fn_801BA190(lbl_805FB6C8, this, 0, 0x80E3B472, 0, 0, 1, 0);
        mPrevLevel = mLevel;
    }
    mLevel = UnkClamp(delta + mLevel, 0.0f, 100.0f);
    if (mLevel > mPrevLevel) {
        fn_801BA190(lbl_805FB6C8, this, 0, 0xAE21DFE0, 0, 0, 1, 0);
    }
}

void* Entity_MysticBrewVortex::fn_800BF7F0() {
    UnkList* list = m8C;
    if (list != 0 && list->mCount == 1) {
        UnkVortexGroup* group = (UnkVortexGroup*)list->First();
        if (group != 0 && group->mItems.mCount == 1) {
            UnkVortexItem* item = (UnkVortexItem*)group->mItems.First();
            if (item->mC == 0) {
                item->m18 = item->m24 = 0.0f;
                return item;
            }
        }
    }
    return 0;
}

f32 Entity_MysticBrewVortex::fn_800BF870() {
    return mLevel;
}

f32 Entity_MysticBrewVortex::fn_800BF878(f32 delta) {
    if (delta < 0.0f) {
        fn_801BA190(lbl_805FB6C8, this, 0, 0x80E3B472, 0, 0, 1, 0);
        mPrevLevel = mLevel;
    }
    mLevel = UnkClamp(delta + mLevel, 0.0f, 100.0f);
    if (mLevel > mPrevLevel) {
        fn_801BA190(lbl_805FB6C8, this, 0, 0xAE21DFE0, 0, 0, 1, 0);
    }
    return mLevel;
}
