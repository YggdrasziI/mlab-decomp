// Game unit at 0x800A5BCC-0x800A6538 (real file name unknown): the items of the buzz
// saw experiment. Class names from RTTI: "BuzzsawItem" (abstract base derived from
// Entity_Prop; its vfC4 entry is 0, a pure virtual function), "BuzzsawObstacle",
// "BuzzsawQuicksand" and "BuzzsawGate". The implicit destructors of the three derived
// classes are emitted at the end. Bounds come from the static initializers of this
// unit and the previous one.

#include "game/unk_vec3.mch"

#include "game/BuzzSawExperiment.h"
#include "game/Entity.h"
#include "game/externs.h"
#include "game/unk_vec3.h"

// Object returned by Entity::vf0C (real type unknown).
struct UnkEntityView {
    /* 0x0 */ u8 unk0[0x8];
    /* 0x8 */ u32 mFlags;
};

// Global manager created on first use (see unit_8010FA48.cpp).
class UnkManager {
public:
    UnkManager();
    static int fn_80209E70(u32 hash, int arg1, int arg2, int arg3, int arg4, int arg5);

    static UnkManager* Instance() {
        if (sInstance == 0) {
            sInstance = new UnkManager;
        }
        return sInstance;
    }

    static UnkManager* sInstance;
};

extern "C" {
void fn_8006BB3C(Entity* entity, int anim, int arg);
BuzzSawExperiment* fn_8006FBDC(void);
u32 fn_80143460(void);
}

class BuzzsawItem : public Entity_Prop {
public:
    BuzzsawItem(int type, EntityInfo* info, int init) : Entity_Prop(type, info, init) {
        mActive = 0;
        mEnabled = 0;
        m15C |= 0x600000;
        if (vf0C() != 0) {
            ((UnkEntityView*)vf0C())->mFlags &= ~2;
        }
        mInfo->mFlags &= ~3;
    }
    virtual ~BuzzsawItem();
    virtual void vfBC();
    virtual void vfC0();
    virtual void vfC4() = 0;

    /* 0x264 */ u8 mActive;
    /* 0x265 */ u8 mEnabled;
    /* 0x266 */ u8 m266;
};

class BuzzsawObstacle : public BuzzsawItem {
public:
    BuzzsawObstacle(int type, EntityInfo* info, int init);
    virtual void vfBC();
    virtual void vfC4();
};

class BuzzsawQuicksand : public BuzzsawItem {
public:
    BuzzsawQuicksand(int type, EntityInfo* info, int init);
    virtual void vfBC();
    virtual void vfC4();
};

class BuzzsawGate : public BuzzsawItem {
public:
    BuzzsawGate(int type, EntityInfo* info, int init);
    virtual void vfBC();
    virtual void vfC4();

    void fn_800A63B4();
};

BuzzsawItem::~BuzzsawItem() {}

void BuzzsawItem::vfBC() {
    mActive = 1;
    mEnabled = 1;
    if (vf0C() != 0) {
        ((UnkEntityView*)vf0C())->mFlags |= 2;
    }
    mInfo->mFlags |= 2;
    m15C |= 0x600000;
}

void BuzzsawItem::vfC0() {
    mActive = 0;
    if (vf0C() != 0) {
        ((UnkEntityView*)vf0C())->mFlags &= ~2;
    }
    mInfo->mFlags &= ~2;
}

void BuzzsawQuicksand::vfBC() {
    BuzzsawItem::vfBC();
    m266 = 1;
}

void BuzzsawQuicksand::vfC4() {
    if (m266 != 0) {
        m266 = 0;
    }
}

BuzzsawObstacle::BuzzsawObstacle(int type, EntityInfo* info, int init) : BuzzsawItem(type, info, init) {
    m15C |= 0x600000;
    if (vf0C() != 0) {
        ((UnkEntityView*)vf0C())->mFlags &= ~2;
    }
    mInfo->mFlags &= ~3;
}

void BuzzsawObstacle::vfBC() {
    BuzzsawItem::vfBC();
    u32 r = fn_80143460();
    fn_8006BB3C(this, r % m1A4, 0);
    m266 = 1;
}

void BuzzsawObstacle::vfC4() {
    if (m266 != 0) {
        m266 = 0;
        UnkManager::Instance()->fn_80209E70(0x9189B7A9, 0, 0, 0, 0, 0);
        fn_8006FBDC()->fn_800A1758();
    }
}

BuzzsawQuicksand::BuzzsawQuicksand(int type, EntityInfo* info, int init) : BuzzsawItem(type, info, init) {
    m15C |= 0x600000;
    if (vf0C() != 0) {
        ((UnkEntityView*)vf0C())->mFlags &= ~2;
    }
    mInfo->mFlags &= ~3;
}

BuzzsawGate::BuzzsawGate(int type, EntityInfo* info, int init) : BuzzsawItem(type, info, init) {
    m15C |= 0x600000;
    if (vf0C() != 0) {
        ((UnkEntityView*)vf0C())->mFlags &= ~2;
    }
    mInfo->mFlags &= ~2;
}

void BuzzsawGate::vfBC() {
    BuzzsawItem::vfBC();
    fn_8006BB3C(this, 0, 0);
    m8 = UnkVec3(1.0f, 1.0f, 1.0f);
    mInfo->mFlags |= 0x8000;
    mInfo->mFlags |= 0x10000000;
    mInfo->mFlags |= 0x4000;
    mInfo->mFlags |= 0x10000000;
}

void BuzzsawGate::vfC4() {
    mEnabled = 0;
    fn_8006BB3C(this, 2, 0);
    fn_8006FBDC()->fn_800A17A0();
    fn_801BA190(lbl_805FB6C8, this, 0, 0xA768564F, 0, 0, 1, 0);
    UnkManager::Instance()->fn_80209E70(0x9904AB9B, 0, 0, 0, 0, 0);
}

void BuzzsawGate::fn_800A63B4() {
    mEnabled = 0;
    fn_8006BB3C(this, 1, 0);
    fn_8006FBDC()->fn_800A17F8();
}
