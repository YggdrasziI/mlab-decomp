// Game unit at 0x800B1498-0x800B1EDC (real file name unknown): the
// Entity_GrowJuiceHeart class and four of its named script callbacks
// ("GrowJuiceHeart_PlayEffectNear", "GrowJuiceHeart_StartGooBubble",
// "GrowJuiceHeart_StartHeartbeat", "GrowJuiceHeart_StopHeartbeat", registered at
// 0x800FE574). Bounds come from the static initializers of this unit and the
// previous one.
// The three per-difficulty tables at 0x80346BB0-0x80346BE0 belong here but stay in
// .rodata unsplit: unit_80014230 has .rodata after them while its .text comes first
// (dtk reports a link order cycle, as for unit_8009F66C).

#include "game/Entity.h"
#include "game/unk_sphere.h"
#include "game/unk_vec3.h"

extern "C" {
int fn_80070458(void);
void fn_800A7D88(Entity* entity, u32 hash);
void fn_801B7020(Entity* entity);
void fn_801EC8C4(UnkEntity94* model, int bone, u32 hash);
void fn_8020A398(int handle);

void fn_800B182C(Entity* source, Entity* target, u32 hash);
void fn_800B1964(u32 hash);
void fn_800B19A8(void);
void fn_800B19DC(u32 hash);
}

class UnkManager {
public:
    static int fn_80209E70(u32 hash, int arg1, int arg2, int arg3, int arg4, int arg5);
};

// Level changes per difficulty (index from fn_80070458), divided by 100.
extern const f32 lbl_80346BB0[4];
extern const f32 lbl_80346BC0[4];
extern const f32 lbl_80346BD0[4];

// Sound handles of the heartbeat and goo bubble callbacks (-1 when stopped).
static int lbl_805F8BB8 = -1;
static int lbl_805F8BBC = -1;

// Entity whose position is replaced by a point at 0x27C (anim type 0x861FD133; real
// type unknown).
struct UnkGrowJuiceSource {
    /* 0x000 */ u8 unk0[0x27C];
    /* 0x27C */ UnkVec3 m27C;
};

Entity_GrowJuiceHeart::Entity_GrowJuiceHeart(int type, EntityInfo* info, int init)
    : Entity_Prop(type, info, init) {
    mVine = 0;
    mLevel = 0.0f;
    mState = -1;
}

Entity_GrowJuiceHeart::~Entity_GrowJuiceHeart() {}

void Entity_GrowJuiceHeart::fn_800B154C(Entity_GrowJuiceHeartVine* vine) {
    mVine = vine;
    mLevel = 0.5f;
    mState = 0;
}

void Entity_GrowJuiceHeart::fn_800B1564() {
    mVine = 0;
    mLevel = 0.5f;
    mState = -1;
    if (lbl_805F8BBC != -1) {
        fn_8020A398(lbl_805F8BBC);
        lbl_805F8BBC = -1;
    }
}

void Entity_GrowJuiceHeart::vf20(f32 dt, int arg) {
    UnkEntityNode* node;
    for (node = mB0; node != 0; node = node->mNext) {
        if (node->mEntity != 0) {
            node->mEntity->mInfo->mFlags |= 1;
        }
    }
    Entity_Prop::vf20(dt, arg);
    vf74(dt);
    for (node = mB0; node != 0; node = node->mNext) {
        if (node->mEntity != 0) {
            node->mEntity->mInfo->mFlags &= ~1;
        }
    }
    fn_801B7020(this);
}

void Entity_GrowJuiceHeart::fn_800B1674(f32 dt) {
    mLevel += dt * (lbl_80346BB0[fn_80070458()] / 100.0f);
    mLevel = UnkClamp(mLevel, 0.0f, 1.0f);
    fn_800B1D98();
}

void Entity_GrowJuiceHeart::fn_800B170C() {
    mLevel += lbl_80346BC0[fn_80070458()] / 100.0f;
    mLevel = UnkClamp(mLevel, 0.0f, 1.0f);
    fn_800A7D88(this, 0x3C15A048);
    fn_800B1D98();
}

void Entity_GrowJuiceHeart::fn_800B179C() {
    mLevel += lbl_80346BD0[fn_80070458()] / 100.0f;
    mLevel = UnkClamp(mLevel, 0.0f, 1.0f);
    fn_800A7D88(this, 0xC8F251D4);
    fn_800B1D98();
}

// "GrowJuiceHeart_PlayEffectNear": plays an effect on the bone of target nearest to
// source.
void fn_800B182C(Entity* source, Entity* target, u32 hash) {
    if (source == 0) {
        return;
    }
    if (target == 0) {
        return;
    }
    UnkEntity94* model = &target->m94;
    UnkVec3 pos = source->mInfo->mPos;
    int index = -1;
    f32 best = 1000000.0f;
    if ((source->m9C != 0 ? source->m9C->m8 : 0) == 0x861FD133) {
        pos = UnkVec3(((UnkGrowJuiceSource*)source)->m27C);
    }
    for (int i = 0; i < (int)model->m0; i++) {
        UnkVec3 bonePos = model->mBones[i].mPos;
        f32 distSq = (bonePos - pos).LengthSq();
        if (distSq < best) {
            index = i;
            best = distSq;
        }
    }
    if (index != -1) {
        fn_801EC8C4(model, index, hash);
    }
}

// "GrowJuiceHeart_StartHeartbeat"
void fn_800B1964(u32 hash) {
    if (lbl_805F8BB8 == -1) {
        lbl_805F8BB8 = UnkManager::fn_80209E70(hash, 0, 0, 0, 0, 1);
    }
}

// "GrowJuiceHeart_StopHeartbeat"
void fn_800B19A8(void) {
    if (lbl_805F8BB8 != -1) {
        fn_8020A398(lbl_805F8BB8);
        lbl_805F8BB8 = -1;
    }
}

// "GrowJuiceHeart_StartGooBubble"
void fn_800B19DC(u32 hash) {
    if (lbl_805F8BBC == -1) {
        lbl_805F8BBC = UnkManager::fn_80209E70(hash, 0, 0, 0, 0, 1);
    }
}

u32 Entity_GrowJuiceHeart::fn_800B1A20(UnkSphere* other) {
    if (fn_800B1AD0(other, 0)) {
        fn_800B179C();
        return 0xDC2ED9B5;
    }
    return -1;
}

// Same code as Entity_GrowJuiceHeartVine::fn_800B20A4.
int Entity_GrowJuiceHeart::fn_800B1AD0(UnkSphere* other, UnkVec3* hitPos) {
    UnkVec3 move = other->mPos - other->mPrevPos;
    UnkList* groups = m8C;
    if (groups != 0) {
        for (u32 i = 0; i < groups->mCount; i++) {
            // The void* local gives the DOL's register numbers.
            void* p = UnkListAt(groups, i);
            UnkSphereGroup* group = (UnkSphereGroup*)p;
            if (p == 0) {
                continue;
            }
            for (u32 j = 0; j < group->mItems.mCount; j++) {
                UnkSphere* item = (UnkSphere*)UnkListAt(&group->mItems, j);
                if (item->mC != 0) {
                    continue;
                }
                f32 t = 0.0f;
                int hit = SweepSpheres(other->mPrevPos, move, other->mRadius, item->mPos, UnkVec3Zero(),
                                       item->mRadius, t);
                if (hit && t <= 1.0f) {
                    if (hitPos != 0) {
                        *hitPos = item->mPos;
                    }
                    return 1;
                }
            }
        }
    }
    if (hitPos != 0) {
        *hitPos = UnkVec3Zero();
    }
    return 0;
}

f32 Entity_GrowJuiceHeart::fn_800B1D90() {
    return mLevel;
}

// Moves between three states as the level crosses 0.6 / 0.9 upwards and 0.5 / 0.7
// downwards, sending the same event to the heart and its vine.
void Entity_GrowJuiceHeart::fn_800B1D98() {
    int state = mState;
    if (state == 0) {
        if (mLevel >= 0.6f) {
            fn_800A7D88(this, 0x8EAA2203);
            fn_800A7D88(mVine, 0x8EAA2203);
            mState = 1;
        }
    } else if (state == 1) {
        if (mLevel >= 0.9f) {
            fn_800A7D88(this, 0x8EAA2203);
            fn_800A7D88(mVine, 0x8EAA2203);
            mState = 2;
        } else if (mLevel <= 0.5f) {
            fn_800A7D88(this, 0x36BCBA77);
            fn_800A7D88(mVine, 0x36BCBA77);
            mState = 0;
        }
    } else if (state == 2) {
        if (mLevel <= 0.7f) {
            fn_800A7D88(this, 0x36BCBA77);
            fn_800A7D88(mVine, 0x36BCBA77);
            mState = 1;
        }
    }
}
