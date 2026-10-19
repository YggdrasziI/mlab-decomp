// Game unit at 0x800B1EDC-0x800B2384 (real file name unknown): the
// Entity_GrowJuiceHeartVine class. Bounds come from the static initializers of this
// unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/Entity.h"
#include "game/unk_sphere.h"
#include "game/unk_vec3.h"

extern "C" void fn_801B7020(Entity* entity);

Entity_GrowJuiceHeartVine::Entity_GrowJuiceHeartVine(int type, EntityInfo* info, int init)
    : Entity_Prop(type, info, init) {
    m264 = 0;
}

Entity_GrowJuiceHeartVine::~Entity_GrowJuiceHeartVine() {}

void Entity_GrowJuiceHeartVine::fn_800B1F80(Entity_GrowJuiceHeart* heart) {
    m264 = heart;
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
        m264->fn_800B179C();
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
