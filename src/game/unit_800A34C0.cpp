// Game unit at 0x800A34C0-0x800A38F4 (real file name unknown): the Entity_BuzzsawTile
// class (name from RTTI at 0x8035D1F4) and a weak binary tree search used by the
// next unit too. The end is the static initializer; the start is the end of the
// previous unit's data (the tree error strings of the unit at 0x8009F66C), a
// hypothesis since that unit is not split.

#include "game/Entity.h"
#include "game/unk_vec3.h"

// Node of a binary search tree keyed by a u32 (probably an MSL map instance; real
// type unknown).
struct UnkTreeNode {
    /* 0x0 */ UnkTreeNode* mLeft;
    /* 0x4 */ UnkTreeNode* mRight;
    /* 0x8 */ u32 m8;
    /* 0xC */ u32 mKey;
};

struct UnkTreeRange;

struct UnkTree {
    UnkTreeNode* End() { return (UnkTreeNode*)&mRoot; }
    UnkTreeRange fn_800A3638(const u32& key);

    /* 0x0 */ u32 m0;
    /* 0x4 */ UnkTreeNode* mRoot; // first word of the end node
};

// The DOL returns it through a hidden pointer: without the copy constructor this build
// returns the 8 bytes in r3/r4.
struct UnkTreeRange {
    UnkTreeRange(UnkTreeNode* first, UnkTreeNode* second) : mFirst(first), mSecond(second) {}
    UnkTreeRange(const UnkTreeRange& other) : mFirst(other.mFirst), mSecond(other.mSecond) {}

    UnkTreeNode* mFirst;
    UnkTreeNode* mSecond;
};

// Model data reached through Entity::vf10 (real names unknown).
struct UnkBuzzsawModelSub {
    /* 0x00 */ u8 unk0[0x20];
    /* 0x20 */ int m20;
};

struct UnkBuzzsawModelData {
    /* 0x00 */ u8 unk0[0x14];
    /* 0x14 */ UnkBuzzsawModelSub* m14;
};

// Globals defined by the unit at 0x8009F66C (real names unknown).
extern void* lbl_805FB160;
extern UnkTree* lbl_805FB164;

extern "C" {
void* fn_800A4EE4(void* pool, u32 hash);
void fn_800A3E08(Entity* saw, Entity* tile);
void fn_800A424C(Entity* saw);
void fn_800A1850(void* mode, Entity* tile);
void* fn_8006FBDC(void);
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
    lbl_805FB164->fn_800A3638(lbl_805F8B38);
    UnkBuzzsawModelSub* sub = ((UnkBuzzsawModelData*)vf10()->mData)->m14;
    sub->m20 = 0;
    if (fn_8006FBDC() != 0) {
        fn_800A1850(fn_8006FBDC(), this);
    }
}

// NonMatching (98.5 %): the key value and `lo` swap r4 and r6 (the DOL keeps the key
// in r6). The declaration order, a separate lower/upper bound inline pair and the
// libstdc++ form did not fix it.
UnkTreeRange UnkTree::fn_800A3638(const u32& key) {
    UnkTreeNode* x = mRoot;
    UnkTreeNode* y = End();
    while (x != 0) {
        if (key < x->mKey) {
            y = x;
            x = x->mLeft;
        } else if (x->mKey < key) {
            x = x->mRight;
        } else {
            UnkTreeNode* hi;
            UnkTreeNode* lo;
            lo = x->mLeft;
            hi = x->mRight;
            UnkTreeNode* first = x;
            UnkTreeNode* last = y;
            while (lo != 0) {
                if (!(lo->mKey < key)) {
                    first = lo;
                    lo = lo->mLeft;
                } else {
                    lo = lo->mRight;
                }
            }
            while (hi != 0) {
                if (key < hi->mKey) {
                    last = hi;
                    hi = hi->mLeft;
                } else {
                    hi = hi->mRight;
                }
            }
            return UnkTreeRange(first, last);
        }
    }
    return UnkTreeRange(y, y);
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
