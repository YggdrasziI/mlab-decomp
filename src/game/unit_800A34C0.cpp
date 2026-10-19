// Game unit at 0x800A34C0-0x800A38F4 (real file name unknown): the Entity_BuzzsawTile
// class (name from RTTI at 0x8035D1F4); the equal_range of the experiment's tile
// multimap is instantiated here (the next unit calls it too). Bounds come from the
// static initializers of this unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/BuzzSawExperiment.h"
#include "game/unk_vec3.h"

// Model data reached through Entity::vf10 (real names unknown).
struct UnkBuzzsawModelSub {
    /* 0x00 */ u8 unk0[0x20];
    /* 0x20 */ int m20;
};

struct UnkBuzzsawModelData {
    /* 0x00 */ u8 unk0[0x14];
    /* 0x14 */ UnkBuzzsawModelSub* m14;
};


extern "C" {
void* fn_800A4EE4(UnkSawPool* pool, u32 hash);
void fn_800A3E08(Entity* saw, Entity* tile);
void fn_800A424C(Entity* saw);
void fn_800A1850(BuzzSawExperiment* experiment, Entity* tile);
BuzzSawExperiment* fn_8006FBDC(void);
void fn_801B5338(Entity* entity, Entity* child, u32 bone, const UnkVec3& offset, int arg);
int fn_801AC2FC(void* set, u32 id);
void fn_80069350(Entity* entity, int index, int arg2, int arg3);
}

// Key passed to the tree search (real name and type unknown; the DOL symbol is 8
// bytes, which may be padding before the RTTI objects).
#pragma explicit_zero_data on
static u32 lbl_805F8B38 = 0;
#pragma explicit_zero_data off

class Entity_BuzzsawTile : public ExWorldWireTile {
public:
    Entity_BuzzsawTile(int type, EntityInfo* info, int init);
    virtual ~Entity_BuzzsawTile();
    virtual void vf20(f32 dt, int arg);
    virtual void vfBC();
    virtual void vfC4();
    virtual void vfC8();
    virtual void vfCC();

    void PlayAnim() {
        if (m9C != 0 && m9C->mSet != 0) {
            fn_80069350(this, fn_801AC2FC(m9C->mSet, 0xECFDEE97), 0, 1);
            UnkBuzzsawModelSub* sub = ((UnkBuzzsawModelData*)vf10()->mData)->m14;
            sub->m20 = 1;
        }
    }

    /* 0x2A4 */ Entity* m2A4;
};

Entity_BuzzsawTile::Entity_BuzzsawTile(int type, EntityInfo* info, int init) : ExWorldWireTile(type, info, init) {
    m2A4 = 0;
}

Entity_BuzzsawTile::~Entity_BuzzsawTile() {}

void Entity_BuzzsawTile::vfC8() {
    ExWorldTile::vfC8();
    u32 hash;
    if (m9C != 0) {
        hash = m9C->m8;
    } else {
        hash = 0;
    }
    m2A4 = (Entity*)fn_800A4EE4(lbl_805FB160, hash);
    if (m2A4 != 0) {
        UnkVec3 offset = UnkVec3Zero();
        fn_801B5338(this, m2A4, 0x206114EF, offset, 0);
        fn_800A3E08(m2A4, this);
    }
    lbl_805FB164->equal_range(lbl_805F8B38);
    UnkBuzzsawModelSub* sub = ((UnkBuzzsawModelData*)vf10()->mData)->m14;
    sub->m20 = 0;
    if (fn_8006FBDC() != 0) {
        fn_800A1850(fn_8006FBDC(), this);
    }
}

void Entity_BuzzsawTile::vfCC() {
    PlayAnim();
    if (m2A4 != 0) {
        fn_801B50B0(m2A4);
        fn_800A424C(m2A4);
    }
    ExWorldTile::vfCC();
}

void Entity_BuzzsawTile::vfBC() {
    ExWorldWireTile::vfBC();
    PlayAnim();
}

void Entity_BuzzsawTile::vfC4() {}

void Entity_BuzzsawTile::vf20(f32 dt, int arg) {
    for (UnkEntityNode* node = mB0; node != 0; node = node->mNext) {
        if (node->mEntity != 0) {
            node->mEntity->mInfo->mFlags |= 1;
        }
    }
    Entity_Prop::vf20(dt, arg);
    vf74(dt);
    for (UnkEntityNode* node = mB0; node != 0; node = node->mNext) {
        if (node->mEntity != 0) {
            node->mEntity->mInfo->mFlags &= ~1;
        }
    }
}
