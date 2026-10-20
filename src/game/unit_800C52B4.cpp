// Game unit at 0x800C52B4-0x800C5E90 (real file name unknown): the ExWorldCurvedTrack
// class (RTTI; its RTTI record is emitted by an earlier unit, as for
// ExWorldWireTrack, so this build adds RTTI data the DOL has elsewhere). Bounds
// come from the static initializers of this unit and the previous one. The tiles
// are walked from the ring's tail towards its head.

#include "game/unk_vec3.mch"
#include "game/ExWorldTrack.h"

extern "C" {
// ExWorldTrack and ExWorldTile functions, real names unknown.
f32 fn_800C3E84(ExWorldTrack* track);
ExWorldTile* fn_800C2FF0(ExWorldTrack* track);
UnkVec3 fn_800C176C(ExWorldTile* tile);
}

UnkVec3 fn_801452A8(const UnkQuat* rot);
UnkVec3 fn_801455E0(const UnkQuat* rot);

static inline bool UnkIsNearZero(f32 x) {
    return x > -0.0001f && x < 0.0001f;
}

// Clamp between two bounds given in any order.
static inline f32 UnkClampBetween(const f32& value, f32 a, const f32& b) {
    return a < b ? (value < a ? a : (value > b ? b : value))
                 : (value < b ? b : (value > a ? a : value));
}

ExWorldCurvedTrack::ExWorldCurvedTrack() {}

ExWorldCurvedTrack::~ExWorldCurvedTrack() {}

void ExWorldCurvedTrack::vf0C(void* owner) {
    ExWorldTrack::vf0C(owner);
    mD8 = 0.0f;
}

void ExWorldCurvedTrack::vf10() {
    ExWorldTrack::vf10();
    mD8 = 0.0f;
}

void ExWorldCurvedTrack::vf1C(f32 delta) {
    f32 length = fn_800C3E84(this);
    mD8 = UnkClampBetween(mD8 + delta, 0.0f, length);
}

f32 ExWorldCurvedTrack::vf20() {
    return mD8;
}

void ExWorldCurvedTrack::vf38(ExWorldTile* tile) {
    mD8 -= tile->vfD0();
}

// Distance travelled inside the current tile.
// NonMatching (98.9 %): register allocation in the inlined UnkTileRing::Contains
// (capacity, tail copy and result take other registers).
extern "C" f32 fn_800C54A0(ExWorldCurvedTrack* track) {
    f32 distance = track->vf20();
    ExWorldTile* tile = track->mTiles.Last();
    while (distance > tile->vfD0()) {
        distance -= tile->vfD0();
        if (UnkIsNearZero(distance)) {
            return tile->vfD0();
        }
        tile = track->mTiles.Prev();
    }
    return distance;
}

// NonMatching (96.5 %): the DOL loads mTiles.mHead above the prologue stores, and
// the same register allocation gap as fn_800C54A0.
UnkVec3 ExWorldCurvedTrack::vf24(f32 distance) {
    ExWorldTile* tile = mTiles.Last();
    while (distance > tile->vfD0()) {
        distance -= tile->vfD0();
        if (UnkIsNearZero(distance)) {
            return fn_800C176C(tile);
        }
        tile = mTiles.Prev();
    }
    return static_cast<ExWorldCurvedTile*>(tile)->vfD4(distance);
}

// NonMatching (95.9 %): same gaps as vf24.
extern "C" UnkQuat fn_800C597C(ExWorldCurvedTrack* track, f32 distance) {
    ExWorldTile* tile = track->mTiles.Last();
    while (distance > tile->vfD0()) {
        distance -= tile->vfD0();
        tile = track->mTiles.Prev();
    }
    return static_cast<ExWorldCurvedTile*>(tile)->vfD8(distance);
}

// NonMatching (99.3 %): register allocation of the inlined fn_800C54A0.
UnkVec3 ExWorldCurvedTrack::vf28() {
    f32 t = fn_800C54A0(this);
    UnkQuat rot = static_cast<ExWorldCurvedTile*>(fn_800C2FF0(this))->vfD8(t);
    UnkVec3 dir = fn_801452A8(&rot);
    UnkVec3 up = fn_801455E0(&rot);
    UnkVec3 result;
    result.x = -dir.x;
    result.y = -dir.y;
    result.z = -dir.z;
    return result;
}
