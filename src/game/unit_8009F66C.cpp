// Game unit at 0x8009F66C-0x800A34C0 (real file name unknown): the buzz saw experiment
// (classes BuzzSawExperiment, BuzzsawTrack and BuzzsawTileSetPool, names from RTTI)
// and the MSL tree functions of its maps. Bounds come from the static initializers of
// this unit and the previous one.
//
// Built with -ipa file: it is the only setting found that inlines the map destructors
// as the DOL does (the recursive node destruction three levels deep, the element
// destructor chains in full; see notes/MATCHING.md). NonMatching: see TASKS.md (five
// functions, the .bss order of sUnkVec3, the order of the weak functions).

// The static initializer addresses each object with its own lis.
#pragma pool_data off

#include "game/BuzzSawExperiment.h"
#include "game/unk_803C4298.h"
#include "game/unk_803E6D00.h"
#include "game/unk_vec3.h"

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

// Object returned by Entity::vf0C (real type unknown).
struct UnkEntityState {
    /* 0x0 */ u8 unk0[0x8];
    /* 0x8 */ u32 mFlags;
};

// Model data reached through Entity::vf10 (real names unknown).
struct UnkSawModelSub {
    /* 0x00 */ u8 unk0[0x20];
    /* 0x20 */ int m20;
};

struct UnkSawModelData {
    /* 0x00 */ u8 unk0[0x14];
    /* 0x14 */ UnkSawModelSub* m14;
};

// Joint returned by fn_8018C39C (real type unknown).
struct UnkSawJoint {
    /* 0x00 */ u8 unk0[0x24];
    /* 0x24 */ UnkVec3 mPos;
};

extern "C" {
BuzzSawExperiment* fn_8006FBDC(void);
int fn_80070458(void);
void fn_800C253C(ExWorldTileSetPool* pool, int arg);
int fn_800C2628(void* owner, ExWorldTile* tile);
ExWorldTile* fn_800C2FF0(ExWorldTrack* track);
void fn_8000C7F4(u32 hash, int arg1, int arg2);
void fn_8000FBA0(u32 hash, f32 value);
void fn_8020A398(int handle);
int fn_801AC2FC(void* set, u32 id);
void fn_80069350(Entity* entity, int index, int arg2, int arg3);
void fn_8018BEA4(UnkModel* model, f32 time);
f32 fn_8018BE30(UnkModel* model, f32 arg);
int fn_8018BDE0(UnkModel* model, u32 hash);
UnkSawJoint* fn_8018C39C(UnkModel* model, int index);
void fn_801B4F4C(Entity* entity, int arg);
void fn_800712BC(Experiment* experiment, int time, int arg);
void fn_80070FAC(Experiment* experiment, f32 value);
void fn_80075264(Experiment* experiment, int arg);
// Functions of the object at BuzzSawExperiment::mC8C (real names unknown).
void fn_800A6630(Entity* saw);
void fn_800A660C(Entity* saw);
void fn_800A6694(Entity* saw);
void fn_800A6704(Entity* saw, BuzzsawTrack* track);
void fn_800A7020(Entity* saw, BuzzsawTrack* track, UnkSawMultimap* tiles, f32 dt);
void fn_800A7064(Entity* saw, f32 dt);
void fn_800A71C8(Entity* saw, u32 hash);
void fn_800A75C8(Entity* saw);
void fn_800A76B4(Entity* saw);
void fn_800A7828(Entity* saw);
f32 fn_800A791C(Entity* saw);
// Functions of UnkSawPool (real names unknown).
void fn_800A3CAC(Entity* entity, UnkSawPool* pool, int type);
void fn_800A43C0(UnkSawPool* pool, u32 hash);
}

// Per-difficulty tables (index from fn_80070458). They are in .rodata between this
// unit's neighbours, but the .rodata of unit_80014230 comes after them while its
// .text comes first, so they cannot be split into this unit (see notes/SPLITS.md).
extern const f32 lbl_80346A58[4];
extern const int lbl_80346A68[4];
extern const f32 lbl_80346B18[4];
extern const f32 lbl_80346B28[4];
extern const f32 lbl_80346B38[4];
extern const f32 lbl_80346B48[4];
extern const f32 lbl_80346B58[4];

// Hashes of the three tile sets.
static u32 lbl_805F8B08 = 0x05C5F486;
static u32 lbl_805F8B0C = 0xEEA13AE2;
static u32 lbl_805F8B10 = 0xA93CF6EC;

// The running experiment and its two maps (used by the saw and tile units).
BuzzSawExperiment* lbl_805FB158;
UnkSawPool* lbl_805FB160;
UnkSawMultimap* lbl_805FB164;

// Registration of the experiment (constructor at 0x8006FA44, real type unknown).
struct UnkExperimentInfo {
    UnkExperimentInfo(u32 hash, int arg, u32 hash2, u32 hash3, u32 hash4);

    u32 m0[5];
};

UnkExperimentInfo lbl_803C6BF0(0x27A7F2C6, 3, 0x5AADD230, 0x85AB5092, 0x8D4B9D04);

static UnkTileSet lbl_803C6C08[3] = {
    UnkTileSet(&lbl_805F8B08, 1),
    UnkTileSet(&lbl_805F8B0C, 1),
    UnkTileSet(&lbl_805F8B10, 1),
};

// Stride of the UnkGlobals803E6D00Sub::mWireGroups array (real name unknown).
extern int lbl_805FB94C;

// Type hash of an entity (real name unknown).
static inline int UnkGetType(Entity* entity) {
    int type;
    if (entity->m9C != 0) {
        type = entity->m9C->m8;
    } else {
        type = 0;
    }
    return type;
}

static inline UnkWireGroup* GetWireGroup(int index) {
    return (UnkWireGroup*)(lbl_803E6D00.m0->mWireGroups + index * lbl_805FB94C);
}

void BuzzsawTileSetPool::vf1C() {
    if (fn_8006FBDC() == 0) {
        m14++;
        fn_800C253C(this, 0);
    } else if (fn_8006FBDC()->mCD0 > 0.0f) {
        m14++;
        fn_800C253C(this, 0);
    } else {
        fn_800C253C(this, 2);
    }
}

bool BuzzsawTileSetPool::vf20(u32 hash) {
    bool found = false;
    for (int i = 0; i < m10; i++) {
        if (hash == (&lbl_805F8B10)[i]) {
            found = true;
            break;
        }
    }
    return found;
}

static inline bool UnkRingContains(const UnkTileRing& r, int index) {
    if (index < 0 || index >= r.mCapacity) {
        return false;
    }
    int head = r.mHead;
    int tail = r.mTail;
    if (tail < head) {
        if (index <= tail) {
            index += r.mCapacity;
        }
        tail += r.mCapacity;
    }
    return head <= index && index <= tail;
}

static inline ExWorldTile* UnkRingFirst(UnkTileRing& r) {
    if (!r.IsEmpty()) {
        int head = r.GetHead();
        r.mCurrent = UnkRingContains(r, head) ? &r.mItems[head] : 0;
        if (r.mCurrent != 0) {
            return *r.mCurrent;
        }
    }
    return 0;
}

bool BuzzsawTrack::vf2C(ExWorldTile* tile) {
    if (fn_800C2628(mOwner, UnkRingFirst(mTiles)) != 0) {
        return false;
    }
    int count = mTiles.Count();
    if (count > lbl_80346A68[fn_80070458()]) {
        return false;
    }
    return true;
}

bool BuzzsawTrack::vf30(ExWorldTile* tile) {
    if (fn_800C2FF0(this) == tile) {
        return false;
    }
    return ExWorldTrack::vf30(tile);
}

// NonMatching (96.4 %): each multimap insert keeps five copies of the pair on the
// stack in the DOL, four here; the member initializers' zero is not shared with the
// loop counter's register.
BuzzSawExperiment::BuzzSawExperiment()
    : Experiment(2), mC8C(0), mC90(0.0f), mCAC(0), mCCC(0.0f), mCD4(0), mCF4(50.0f) {
    lbl_805FB158 = this;
    lbl_805FB160 = &mSawPool;
    lbl_805FB164 = &mTiles;
    fn_800A0FB4();
    UnkWireGroup* group = (UnkWireGroup*)(lbl_803E6D00.m0->mWireGroups + ((lbl_805FB94C << 1) - lbl_805FB94C));
    for (int i = 0; i < group->mEntityCount; i++) {
        Entity* entity = group->mEntities[i];
        switch (UnkGetType(entity)) {
        case 0xAB868DB8:
        case 0x8D6957B0:
            if (entity->vf0C() != 0) {
                ((UnkEntityState*)entity->vf0C())->mFlags &= ~2;
            }
            entity->mInfo->mFlags &= ~3;
            mTiles.insert(std::make_pair((u32)UnkGetType(entity), entity));
            break;
        case 0x677E3F1B:
            if (entity->vf0C() != 0) {
                ((UnkEntityState*)entity->vf0C())->mFlags &= ~2;
            }
            entity->mInfo->mFlags &= ~2;
            mTiles.insert(std::make_pair((u32)UnkGetType(entity), entity));
            break;
        }
    }
    for (int i = 0; i <= lbl_803E6D00.m0->m4; i++) {
        UnkWire* wires = GetWireGroup(i)->mWires;
        for (u8 j = 0; j < GetWireGroup(i)->mWireCount; j++) {
            wires[j].mFlags &= ~1;
        }
    }
    fn_800A71C8(mC8C, 0x2397C385);
    fn_800A11F4();
    mPool.m10 = 1;
    mPool.ExWorldTileSetPool::vf0C(lbl_803C6C08, 3);
    GetTrack()->vf0C(&mPool);
    GetTrack()->vf14(0.0f);
    GetTrack()->vf20();
    UnkVec3 pos = GetTrack()->vf24();
    mCB0 = mCAC->mInfo->mPos - pos;
    mCF8 = -1;
    mCFC = -1;
    fn_80075264(this, 0x3A2);
    m8 = 0x2A8;
    mC = 0x2A9;
    m30 = 0xFAB3B8A8;
    m34 = 0x63BAE912;
}

// NonMatching (99.9 %): the counter and the offset of the last loop swap r4 and r6.
BuzzSawExperiment::~BuzzSawExperiment() {
    lbl_805FB158 = 0;
    fn_8020A398(mCF8);
    fn_8020A398(mCFC);
    if (mC8C != 0) {
        fn_800A6630(mC8C);
        mC8C = 0;
    }
    GetTrack()->vf10();
    UnkWireGroup* group = GetWireGroup(1);
    for (int i = 0; i < group->mEntityCount; i++) {
        group->mEntities[i]->mInfo->mFlags |= 1;
    }
}

// Object at lbl_803C4298.m8 (real type unknown).
class UnkSawHud {
public:
    virtual void vf08();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C();
    virtual void vf30();
    virtual void vf34();
    virtual void vf38();
    virtual void vf3C();
    virtual void vf40();
    virtual void vf44();
    virtual void vf48();
    virtual void vf4C();
    virtual void vf50();
    virtual void vf54();
    virtual void vf58();
    virtual void vf5C();
    virtual void vf60();
    virtual void vf64();
    virtual void vf68();
    virtual void vf6C();
    virtual void vf70();
    virtual void vf74();
    virtual void vf78();
    virtual void vf7C();
    virtual void vf80();
    virtual void vf84();
    virtual void vf88();
    virtual void vf8C();
    virtual void vf90();
    virtual void vf94();
    virtual void vf98(f32 value);
};

// Angle between two directions, as a fraction of a right angle (real name unknown).
static inline double UnkClampD(f32 value, f32 low, f32 high) {
    f32 result;
    if (value < low) {
        result = low;
    } else if (value > high) {
        result = high;
    } else {
        result = value;
    }
    return result;
}

static inline f32 UnkAngleFraction(const UnkVec3& a, const UnkVec3& b) {
    f32 dot = a.x * b.x + a.y * b.y + a.z * b.z;
    f32 lengths = (f32)UnkSqrtf(a.LengthSq()) * (f32)UnkSqrtf(b.LengthSq());
    bool zero = lengths <= 0.0f;
    if (zero) {
        return 0.0f;
    }
    return acosf(UnkClamp(UnkClampD(dot / lengths, -1.0f, 1.0f), -1.0f, 1.0f)) * 0.63661975f;
}

// NonMatching (98.9 %): float registers of the angle and rotation code.
void BuzzSawExperiment::vf54() {
    lbl_803C4298.mCamera->vf50(0);
    lbl_803C4298.mCamera->vf0C(6);
    UnkVec3 eye(0.0f, -11.0f, 8.0f);
    UnkVec3 target(0.0f, 8.0f, 0.0f);
    UnkVec3 dir = GetTrack()->vf28();
    UnkVec3 up(0.0f, 1.0f, 0.0f);
    UnkVec3 right(1.0f, 0.0f, 0.0f);
    f32 angle = UnkAngleFraction(dir, up);
    if (dir.x * right.x + dir.y * right.y + dir.z * right.z > 0.0f) {
        angle = -angle;
    }
    f32 a = 1.5707964f * angle;
    f32 c = cosf(a);
    f32 s = sinf(a);
    f32 x = target.x;
    target.x = x * c + target.y * -s;
    target.y = target.y * c + x * s;
    a = 1.5707964f * angle;
    c = cosf(a);
    s = sinf(a);
    x = eye.x;
    eye.x = x * c + eye.y * -s;
    eye.y = eye.y * c + x * s;
    GetTrack()->vf20();
    UnkVec3 pos = GetTrack()->vf24();
    target += pos;
    eye += pos;
    mC94 = eye;
    mCA0 = target;
    lbl_803C4298.mCamera->vf2C(&eye, &target);
    lbl_803C4298.mCamera->vf1C(&eye);
    ((UnkSawHud*)lbl_803C4298.m8)->vf98(0.0f);
}

// Same camera placement as vf54 (written out again: the DOL does not inline this
// function). NonMatching (98.9 %), as vf54.
void BuzzSawExperiment::fn_800A0BAC(f32 dt) {
    UnkVec3 eye(0.0f, -11.0f, 8.0f);
    UnkVec3 target(0.0f, 8.0f, 0.0f);
    UnkVec3 dir = GetTrack()->vf28();
    UnkVec3 up(0.0f, 1.0f, 0.0f);
    UnkVec3 right(1.0f, 0.0f, 0.0f);
    f32 angle = UnkAngleFraction(dir, up);
    if (dir.x * right.x + dir.y * right.y + dir.z * right.z > 0.0f) {
        angle = -angle;
    }
    f32 a = 1.5707964f * angle;
    f32 c = cosf(a);
    f32 s = sinf(a);
    f32 x = target.x;
    target.x = x * c + target.y * -s;
    target.y = target.y * c + x * s;
    a = 1.5707964f * angle;
    c = cosf(a);
    s = sinf(a);
    x = eye.x;
    eye.x = x * c + eye.y * -s;
    eye.y = eye.y * c + x * s;
    GetTrack()->vf20();
    UnkVec3 pos = GetTrack()->vf24();
    target += pos;
    eye += pos;
    mC94 = eye;
    mCA0 = target;
    lbl_803C4298.mCamera->vf2C(&eye, &target);
    lbl_803C4298.mCamera->vf1C(&eye);
    ((UnkSawHud*)lbl_803C4298.m8)->vf98(0.0f);
}

void BuzzSawExperiment::fn_800A0FB4() {
    UnkWireGroup* group = GetWireGroup(1);
    for (int i = 0; i < group->mEntityCount; i++) {
        Entity* entity = group->mEntities[i];
        switch (UnkGetType(entity)) {
        case 0xF543EFD5:
            mC8C = entity;
            break;
        case 0x05C5F486:
            ((ExWorldTile*)entity)->vfBC();
            fn_800A43C0(&mSawPool, UnkGetType(entity));
            break;
        case 0xA93CF6EC:
        case 0xEEA13AE2:
            ((ExWorldTile*)entity)->vfBC();
            fn_800A43C0(&mSawPool, UnkGetType(entity));
            break;
        case 0xC2B3381E:
            mCAC = entity;
            ((Entity_Prop*)entity)->m15C |= 0x600000;
            if (mCAC->m9C != 0 && mCAC->m9C->mSet != 0) {
                fn_80069350(mCAC, fn_801AC2FC(mCAC->m9C->mSet, 0xE7CC3CFA), 0, 1);
                UnkSawModelSub* sub = ((UnkSawModelData*)mCAC->vf10()->mData)->m14;
                sub->m20 = 1;
            }
            break;
        case 0x124C6C9B:
            mCD4 = entity;
            if (entity->vf0C() != 0) {
                ((UnkEntityState*)entity->vf0C())->mFlags &= ~2;
            }
            entity->mInfo->mFlags &= ~2;
            break;
        }
    }
    fn_800A660C(mC8C);
}

void BuzzSawExperiment::fn_800A11F4() {
    UnkWireGroup* group = GetWireGroup(1);
    for (int i = 0; i < group->mEntityCount; i++) {
        Entity* entity = group->mEntities[i];
        if (entity->m9C == 0) {
            continue;
        }
        int type;
        if (entity->m9C != 0) {
            type = entity->m9C->m8;
        } else {
            type = 0;
        }
        switch (type) {
        case 0xCA48E4C0:
        case 0xA23D184E:
        case 0x98952D14:
        case 0xA5F504A4:
        case 0xC0F63D42:
        case 0xD24392AC:
        case 0xE59D629E:
        case 0xF728CD70:
        case 0xEF23BB1C:
        case 0xFD9614F2:
        case 0x4F94AA15:
        case 0x20294A71:
        case 0x1D4963C1:
        case 0x452A7397:
        case 0x579FDC79:
        case 0x60412C4B:
        case 0x5D2105FB:
        case 0x6AFFF5C9:
        case 0x72F483A5:
        case 0x784A5A27:
            fn_800A3CAC(entity, &mSawPool, 0);
            break;
        case 0x02916EC8:
        case 0x95F88FF3:
        case 0x8DF3F99F:
        case 0x874D201D:
        case 0x9F465671:
        case 0xA2267FC1:
        case 0xB093D02F:
        case 0xA898A643:
        case 0xBA2D09AD:
        case 0xD590E9C9:
        case 0xE8F0C079:
        case 0x1A9A18A4:
        case 0x082FB74A:
        case 0x27FA3114:
        case 0x354F9EFA:
        case 0x2D44E896:
        case 0x3FF14778:
        case 0x6AE49246:
        case 0x504CA71C:
        case 0x6D2C8EAC:
            fn_800A3CAC(entity, &mSawPool, 1);
            break;
        case 0x9587343C:
        case 0xA8E71D8C:
        case 0xC75AFDE8:
        case 0xE88F7BB6:
        case 0xD5EF5206:
        case 0xCDE4246A:
        case 0xE231A234:
        case 0xDF518B84:
        case 0xF0840DDA:
        case 0xFA3AD458:
        case 0x2D3B5359:
        case 0x105B7AE9:
        case 0x17936603:
        case 0x4286B33D:
        case 0x48386ABF:
        case 0x5A8DC551:
        case 0x50331CD3:
        case 0x67EDECE1:
        case 0x7558430F:
        case 0x7FE69A8D:
            fn_800A3CAC(entity, &mSawPool, 2);
            break;
        }
    }
}

void BuzzSawExperiment::fn_800A1758() {
    mCF4 += lbl_80346B58[fn_80070458()];
}

void BuzzSawExperiment::fn_800A17A0() {
    mCF4 += lbl_80346B38[fn_80070458()];
    if (mCF4 > 100.0f) {
        mCF4 = 100.0f;
    }
}

void BuzzSawExperiment::fn_800A17F8() {
    mCF4 += lbl_80346B48[fn_80070458()];
    if (mCF4 > 100.0f) {
        mCF4 = 100.0f;
    }
}

extern "C" void fn_800A1850(BuzzSawExperiment* experiment, Entity* tile) {
    UnkVec3 pos = tile->mInfo->mPos;
    pos.x = 40.0f + pos.x;
    Entity* saw = experiment->mCD4;
    saw->SetPosition(pos);
    saw = experiment->mCD4;
    if (saw->vf0C() != 0) {
        ((UnkEntityState*)saw->vf0C())->mFlags |= 2;
    }
    saw->mInfo->mFlags |= 2;
    if (experiment->mCD4->m9C != 0 && experiment->mCD4->m9C->mSet != 0) {
        fn_80069350(experiment->mCD4, fn_801AC2FC(experiment->mCD4->m9C->mSet, 0xECFDEE97), 0, 1);
    }
}

extern "C" void fn_800A1988(void) {
    fn_800A6704(lbl_805FB158->mC8C, &lbl_805FB158->mTrack);
}

extern "C" void fn_800A1998(int on) {
    lbl_805FB158->mCC8 = on != 0;
}

void BuzzSawExperiment::vf60() {
    fn_8000FBA0(0x8D4B9D04, 1.0f);
}

// Moves the camera entity with the track (real name unknown).
inline void BuzzSawExperiment::MoveCameraEntity() {
    mCAC->SetPosition(mCB0 + TrackPos());
    f32 dt = lbl_803E6D00.mDeltaTime;
    mCCC += dt * fn_800A791C(mC8C);
    if (mCCC > 1.0f) {
        mCCC -= 1.0f;
    }
    fn_8018BEA4(mCAC->vf10(), mCCC);
    fn_801B4F4C(mCAC, 0);
}

void BuzzSawExperiment::vf0C() {
    Experiment::vf0C();
    fn_800A71C8(mC8C, 0xCA90298E);
    mCF8 = UnkManager::Instance()->fn_80209E70(0xE2DCC1BA, 0, 0, 0, 0, 0);
    mCFC = UnkManager::Instance()->fn_80209E70(0xE7E15D02, 0, 0, 0, 0, 0);
    fn_800A6694(mC8C);
    f32 time = lbl_80346A58[fn_80070458()];
    mCD0 = time;
    fn_800712BC(this, time, 0);
    MoveCameraEntity();
}

void BuzzSawExperiment::vf14() {
    Experiment::vf14();
}

void BuzzSawExperiment::vf10() {}

void BuzzSawExperiment::vf18() {
    fn_800A71C8(mC8C, 0x437B3D65);
    fn_800A75C8(mC8C);
}

// NonMatching (99.98 %): the discarded track position and the camera entity position
// temporaries take each other's stack slots.
void BuzzSawExperiment::vf58(f32 dt) {
    GetTrack()->vf14(dt);
    mCF4 = mCF4 + dt * lbl_80346B28[fn_80070458()];
    fn_80070FAC(this, 0.01f * mCF4);
    if (mCF4 <= 0.0f) {
        fn_8000C7F4(0x117741C3, 0, 0);
        fn_800A71C8(mC8C, 0x50B782D3);
    }
    if (mCD0 <= 0.0f) {
        fn_8000C7F4(0xC608C842, 0, 0);
        fn_800A71C8(mC8C, 0x50B782D3);
    }
    if (mCC8 != 0) {
        fn_800A7020(mC8C, &mTrack, &mTiles, lbl_803E6D00.mDeltaTime);
        TrackPos();
        MoveCameraEntity();
        fn_800A76B4(mC8C);
    }
    fn_800A7828(mC8C);
    fn_800A75C8(mC8C);
    fn_800A0BAC(dt);
    fn_800C2FF0(&mTrack);
    mCD0 -= dt;
    if (mCD0 < 0.0f) {
        mCD0 = 0.0f;
    }
    fn_800712BC(this, mCD0, mCD0 < 10.0f);
}

void BuzzSawExperiment::vf1C() {}

// Places the saw on the robot, raised by a constant offset (real name unknown).
inline void BuzzSawExperiment::PlaceSaw() {
    UnkVec3 pos = mC8C->mInfo->mPos;
    UnkVec3 offset(lbl_80346B18[0], lbl_80346B18[1], lbl_80346B18[2]);
    mCD4->SetPosition(pos + offset);
}

void BuzzSawExperiment::vf2C() {
    mD00 = 0.0f;
    PlaceSaw();
    if (mCD4->m9C != 0 && mCD4->m9C->mSet != 0) {
        fn_80069350(mCD4, fn_801AC2FC(mCD4->m9C->mSet, 0x0312511A), 0, 1);
    }
}

void BuzzSawExperiment::vf34(f32 dt) {
    mD00 += dt;
    if (mD00 >= 3.0f) {
        fn_8000C7F4(0x88C6E941, 0, 0);
    }
    static UnkVec3 lbl_803C6C20;
    if (mCC8 != 0) {
        fn_800A7064(mC8C, dt);
        fn_800A75C8(mC8C);
        PlaceSaw();
        if (fn_8018BE30(mCD4->vf10(), 0.0f) > 0.3f) {
            mCC8 = 0;
            mCBC = mC8C->mInfo->mPos;
            UnkModel* model = mCD4->vf10();
            UnkSawJoint* joint = fn_8018C39C(model, fn_8018BDE0(model, 0x3FD0CD13));
            mCBC.x -= joint->mPos.x;
            mCBC.y -= joint->mPos.y;
            mCBC.z -= joint->mPos.z;
        }
    } else {
        UnkModel* model = mCD4->vf10();
        UnkSawJoint* joint = fn_8018C39C(model, fn_8018BDE0(model, 0x3FD0CD13));
        mC8C->SetPosition(joint->mPos + lbl_803C6C20);
    }
}

void BuzzSawExperiment::vf30() {}

void BuzzSawExperiment::vf38() {
    mD00 = 0.0f;
}

void BuzzSawExperiment::vf40(f32 dt) {
    mD00 += dt;
    if (mD00 >= 0.5f) {
        fn_8000C7F4(0x88C6E941, 0, 0);
    }
}

void BuzzSawExperiment::vf3C() {}

void BuzzSawExperiment::vf5C() {
    mCF4 = 50.0f;
    fn_800A75C8(mC8C);
    mPool.m14 = 0;
    GetTrack()->vf10();
    GetTrack()->vf0C(&mPool);
    fn_800A71C8(mC8C, 0x9B51AA64);
    fn_800A0BAC(0.0f);
    f32 time = lbl_80346A58[fn_80070458()];
    mCD0 = time;
    fn_800712BC(this, time, 0);
}

f32 BuzzSawExperiment::vf64() {
    return mCF4;
}

void BuzzsawTileSetPool::vf18() {
    fn_800C253C(this, 2);
}

void BuzzsawTileSetPool::vf14() {
    fn_800C253C(this, 1);
}
