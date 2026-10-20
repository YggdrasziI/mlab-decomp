// Game unit at 0x8009BE14-0x8009C2C8 (real file name unknown): the AstralTrack class
// (RTTI "AstralTrack", derived from "ExWorldCurvedTrack" and "ExWorldTrack"). Bounds
// come from the static initializers of this unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/ExWorldTrack.h"

extern "C" {
// Owner (tile set pool) functions, real names unknown.
int fn_800C2628(void* owner, ExWorldTile* tile);
void fn_8009BBCC(void* owner, ExWorldTile* tile, AstralTrack* track);
f32 fn_800C54A0(ExWorldCurvedTrack* track);
ExWorldTile* fn_800C2FF0(ExWorldTrack* track);
}

static inline u32 GetTileHash(ExWorldTile* tile) {
    return tile->m9C != 0 ? tile->m9C->m8 : 0;
}

AstralTrack::AstralTrack() {
    mDC = 4;
    mE0 = 0;
    mD0 = 3;
}

AstralTrack::~AstralTrack() {}

void AstralTrack::vf0C(void* owner) {
    ExWorldCurvedTrack::vf0C(owner);
    mE0 = 0;
    fn_8009BBCC(mOwner, mTiles.First(), this);
}

void AstralTrack::vf10() {
    ExWorldCurvedTrack::vf10();
    mE0 = 0;
}

// NonMatching (99.2 %): two registers swapped in the inlined UnkTileRing::Contains
// (the base ExWorldTrack::vf2C has the DOL's allocation).
bool AstralTrack::vf2C(ExWorldTile* tile) {
    if (fn_800C2628(mOwner, mTiles.First())) {
        return false;
    }
    return mTiles.Count() <= mD0;
}

bool AstralTrack::vf30(ExWorldTile* tile) {
    if (mTiles.Count() > mD0) {
        return mD8 > 5.0f + tile->vfD0();
    }
    return false;
}

extern "C" UnkQuat fn_8009C1E8(ExWorldCurvedTrack* track) {
    f32 t = fn_800C54A0(track);
    return static_cast<ExWorldCurvedTile*>(fn_800C2FF0(track))->vfD8(t);
}

void AstralTrack::vf34(ExWorldTile* tile, ExWorldTile* other) {
    if (GetTileHash(tile) == 0x8EA754DA) {
        return;
    }
    if (GetTileHash(tile) == 0x05B0C2D7) {
        return;
    }
    fn_8009BBCC(mOwner, tile, this);
}
