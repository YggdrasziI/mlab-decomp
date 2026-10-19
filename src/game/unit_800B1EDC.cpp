// Game unit at 0x800B1EDC-0x800B2384 (real file name unknown): the
// Entity_GrowJuiceHeartVine class. Bounds come from the static initializers of this
// unit and the previous one.

#include "game/Entity.h"
#include "game/unk_math.h"
#include "game/unk_vec3.h"

// Moving sphere: the items of the entity's sphere groups and the object swept
// against them share this layout (real type unknown).
struct UnkSphere {
    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ int mC;
    /* 0x10 */ u8 unk10[0x14 - 0x10];
    /* 0x14 */ UnkVec3 mPos;
    /* 0x20 */ UnkVec3 mPrevPos;
    /* 0x2C */ u8 unk2C[0x3C - 0x2C];
    /* 0x3C */ f32 mRadius;
};

struct UnkSphereGroup {
    /* 0x00 */ u8 unk0[0x18];
    /* 0x18 */ UnkList mItems;
};

extern "C" void fn_800B179C(void* owner);
extern "C" void fn_801B7020(Entity* entity);

inline void* UnkListAt(UnkList* list, u32 i) {
    return i >= list->mCount ? 0 : list->mData[i];
}

// Same steps as UnkSqrtf, rounded to float before the caller uses it.
inline f32 UnkSqrtfRounded(f32 x) {
    if (x <= 0.0f) {
        return 0.0f;
    }
    double guess = __frsqrte(x);
    guess = 0.5 * (guess * (3.0 - x * guess * guess));
    guess = 0.5 * (guess * (3.0 - x * guess * guess));
    return (f32)(guess * x);
}

// Sweeps sphere A against sphere B, each moving by its velocity; t is the hit time
// in units of the velocities. fn_800B1AD0 of the previous unit has the same code, so
// this is probably a shared inline (real name unknown).
inline int SweepSpheres(const UnkVec3& posA, const UnkVec3& velA, f32 radiusA, const UnkVec3& posB,
                        const UnkVec3& velB, f32 radiusB, f32& t) {
    UnkVec3 rel = posB - posA;
    UnkVec3 dir = velB - velA;
    f32 radius = radiusB + radiusA;
    f32 c = rel.LengthSq() - radius * radius;
    if (c < 0.0f) {
        return 1;
    }
    f32 a = dir.LengthSq();
    if (a < 0.0001f) {
        return 0;
    }
    f32 b = dir.x * rel.x + dir.y * rel.y + dir.z * rel.z;
    if (b >= 0.0f) {
        return 0;
    }
    f32 disc = b * b; // b * b - a * c swaps two float registers
    disc -= a * c;
    if (disc < 0.0f) {
        return 0;
    }
    t = (-b - UnkSqrtfRounded(disc)) / a;
    return 1;
}

Entity_GrowJuiceHeartVine::Entity_GrowJuiceHeartVine(int type, EntityInfo* info, int init)
    : Entity_Prop(type, info, init) {
    m264 = 0;
}

Entity_GrowJuiceHeartVine::~Entity_GrowJuiceHeartVine() {}

void Entity_GrowJuiceHeartVine::fn_800B1F80(void* owner) {
    m264 = owner;
}

void Entity_GrowJuiceHeartVine::fn_800B1F88() {
    m264 = 0;
}

void Entity_GrowJuiceHeartVine::vf20(f32 dt, int arg) {
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

u32 Entity_GrowJuiceHeartVine::fn_800B2054(UnkSphere* other) {
    if (fn_800B20A4(other, 0)) {
        fn_800B179C(m264);
        return 0xDC2ED9B5;
    }
    return -1;
}

int Entity_GrowJuiceHeartVine::fn_800B20A4(UnkSphere* other, UnkVec3* hitPos) {
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
