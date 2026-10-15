// Game unit at 0x8006F61C-0x8006F6BC (real file name unknown): the Entity_Static
// class. Bounds come from the static initializers of this unit and the previous
// one.

#include "game/Entity.h"
#include "game/unk_vec3.h"

Entity_Static::Entity_Static(int type, EntityInfo* info, int init) : Entity_Game(type, info, init) {
    SetType(2);
}

void Entity_Static::vf20(f32 dt, int arg) {
    if (mInfo->mFlags & 1) {
        Entity_Game::vf20(dt, 0);
    }
}

void Entity_Static::vf40() {
    Entity::vf40();
}
