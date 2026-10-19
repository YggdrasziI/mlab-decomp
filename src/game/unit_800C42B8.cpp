// Game unit at 0x800C42B8-0x800C4530 (real file name unknown): the ExWorldWireTrack
// class (RTTI; its RTTI record is emitted by an earlier unit). Bounds come from the
// static initializers of this unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/ExWorldTrack.h"

extern "C" {
void fn_8010B5A0(UnkWirePath* path, UnkWire* wire);
void fn_8010BB54(UnkWirePath* path);
void fn_8010BFB8(UnkWirePath* path);
void fn_8010C194(UnkWirePath* path);
UnkVec3 fn_8010C354(UnkWirePath* path);
UnkVec3 fn_8010B50C(UnkWirePath* path);
void fn_8010BF6C(UnkWirePath* path, int count);
void fn_8010BB64(UnkWirePath* path, UnkWire* wire, const UnkMtx34& mtx);
// Matrix of the tile's current bone (ExWorldTile method, real name unknown).
UnkMtx34 fn_800C1798(ExWorldTile* tile);
}

ExWorldWireTrack::ExWorldWireTrack() {}

ExWorldWireTrack::~ExWorldWireTrack() {}

void ExWorldWireTrack::vf0C(void* owner) {
    ExWorldTrack::vf0C(owner);
    fn_8010B5A0(&mPath, static_cast<ExWorldWireTile*>(mTiles.First())->mWire);
}

void ExWorldWireTrack::vf10() {
    ExWorldTrack::vf10();
    fn_8010BB54(&mPath);
}

void ExWorldWireTrack::vf1C() {
    fn_8010BFB8(&mPath);
}

void ExWorldWireTrack::vf20() {
    fn_8010C194(&mPath);
}

UnkVec3 ExWorldWireTrack::vf24() {
    return fn_8010C354(&mPath);
}

UnkVec3 ExWorldWireTrack::vf28() {
    return fn_8010B50C(&mPath);
}

void ExWorldWireTrack::vf38(ExWorldTile* tile) {
    if (tile != 0) {
        fn_8010BF6C(&mPath, static_cast<ExWorldWireTile*>(tile)->mWire->mPointCount - 1);
    }
}

void ExWorldWireTrack::vf34(ExWorldTile* tile, ExWorldTile* other) {
    UnkWire* wire = static_cast<ExWorldWireTile*>(tile)->mWire;
    fn_8010BB64(&mPath, wire, fn_800C1798(other));
}
