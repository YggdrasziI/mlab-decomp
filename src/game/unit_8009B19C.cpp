// Game unit at 0x8009B19C-0x8009B264 (real file name unknown): the
// Entity_AstralTile class. Bounds come from the static initializers of this unit and
// the previous one.

#include "game/unk_vec3.mch"
#include "game/Entity.h"
#include "game/unk_vec3.h"

Entity_AstralTile::Entity_AstralTile(int type, EntityInfo* info, int init)
    : ExWorldCurvedTile(type, info, init) {}

Entity_AstralTile::~Entity_AstralTile() {}

void Entity_AstralTile::vfBC() {
    ExWorldCurvedTile::vfBC();
}

void Entity_AstralTile::vfC0() {
    ExWorldTile::vfC0();
}

void Entity_AstralTile::vfC4() {
    ExWorldTile::vfC4();
}

void Entity_AstralTile::vfC8() {
    ExWorldTile::vfC8();
}

void Entity_AstralTile::vfCC() {
    ExWorldTile::vfCC();
}
