// Game unit at 0x800C5E90-0x800C5F84 (real file name unknown): the ExWorldTrackItem
// class. Bounds come from the static initializers of this unit and the previous
// one.

#include "game/Entity.h"
#include "game/unk_vec3.h"

ExWorldTrackItem::ExWorldTrackItem(int type, EntityInfo* info, int init) : Entity_Prop(type, info, init) {
    m264 = -1.0f;
    m268 = 0;
    m26C = 0;
    m270 = 0;
}

ExWorldTrackItem::~ExWorldTrackItem() {}

void ExWorldTrackItem::vfBC() {
    m268 = 0;
}

void ExWorldTrackItem::vfC0() {
    m270 = 0;
}

void ExWorldTrackItem::vfC4() {}

void ExWorldTrackItem::vfC8() {}
