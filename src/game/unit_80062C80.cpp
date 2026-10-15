// Game unit at 0x80062C80-0x8006307C (real file name unknown): the Entity_Game
// class. Bounds come from the static initializers of this unit and the previous one
// (the previous unit ends with an inline constructor emitted after its static
// initializer, at 0x80062C7C).

#include "game/Entity.h"
#include "game/unk_vec3.h"

Entity_Game::Entity_Game(int type, EntityInfo* info, int init) : Entity(type, info, init) {
    SetType(1);
}

Entity_Game::~Entity_Game() {}

void Entity_Game::vf20(f32 dt, int arg) {
    Entity::vf20(dt, 0);
    UnkEntitySlot* slot = vfB8();
    if (slot != 0) {
        slot->mPrev = slot->mCur;
    }
}

int Entity_Game::vf30(void* arg1, void* arg2) {
    UnkEntitySlot* slot = vfB8();
    if (slot != 0) {
        slot->Reset();
    }
    m14C = 0xDC1BFEC5;
    m150 = 1;
    m154 = -1;
    mInfo->mFlags |= 0x200000;
    return Entity::vf30(arg1, arg2);
}

void Entity_Game::vf14(void* arg) {
    UnkEntitySlot* slot = vfB8();
    if (slot != 0) {
        slot->Reset();
        slot->fn_80122A10(this);
    }
    Entity::vf14(arg);
}

void Entity_Game::vf18(void* arg) {
    Entity::vf18(arg);
    UnkEntitySlot* slot = vfB8();
    if (slot != 0) {
        slot->fn_80122B00(this);
    }
}

void Entity_Game::vf78(void* arg) {
    void* old = m88;
    UnkEntitySlot* slot = vfB8();
    if (slot != 0 && old != arg) {
        slot->Clear();
    }
    Entity::vf78(arg);
}

void Entity_Game::vf40() {
    Entity::vf40();
}
