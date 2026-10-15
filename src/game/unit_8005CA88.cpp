// Game unit at 0x8005CA88-0x8005CBC4 (real file name unknown): the Entity_Dynamic
// class. Bounds come from the static initializers of this unit and the previous
// one.

#include "game/Entity.h"
#include "game/unk_vec3.h"

Entity_Dynamic::Entity_Dynamic(int type, EntityInfo* info, int init) : Entity_Game(type, info, init) {
    SetType(3);
}

Entity_Dynamic::~Entity_Dynamic() {}

void Entity_Dynamic::vf20(f32 dt, int arg) {
    if (mInfo->mFlags & 1) {
        m94.fn_801EC080(this);
        Entity_Game::vf20(dt, 0);
    }
}

void Entity_Dynamic::vf40() {
    Entity::vf40();
}
