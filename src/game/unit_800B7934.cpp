// Game unit at 0x800B7934-0x800B7A08 (real file name unknown): the
// Entity_ArmWrestleBolt class. Bounds come from the static initializers of this unit
// and the previous one.

#include "game/unk_vec3.mch"
#include "game/Entity.h"
#include "game/unk_vec3.h"

Entity_ArmWrestleBolt::Entity_ArmWrestleBolt(int type, EntityInfo* info, int init)
    : Entity_Prop(type, info, init) {
    m264 = 0.0f;
}

Entity_ArmWrestleBolt::~Entity_ArmWrestleBolt() {}

void Entity_ArmWrestleBolt::fn_800B79D8(f32 value) {
    m264 += value;
}
