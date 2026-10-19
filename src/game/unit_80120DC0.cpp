// Game unit at 0x80120DC0-0x801210E4 (real file name unknown): the load progress
// item LoadProgressProjectile (RTTI), derived from LoadProgressItem (RTTI; abstract,
// no vtable of its own in the DOL). It moves an actor along six markers as the load
// progress goes from 0 to 100. Bounds come from the static initializers of this
// unit and the previous one. Method names unknown (vfXX = vtable offset).

#include "game/unk_vec3.mch"
#include "game/unk_actor.h"
#include "game/unk_vec3.h"

class LoadProgressItem {
public:
    virtual ~LoadProgressItem() {}
    virtual void vf0C(f32 progress) = 0;
    virtual void vf10() = 0;

    /* 0x04 */ f32 mProgress;
};

class LoadProgressProjectile : public LoadProgressItem {
public:
    virtual ~LoadProgressProjectile() {}
    virtual void vf0C(f32 progress);
    virtual void vf10() {}

    UnkActor* GetNext() {
        if (mIndex < 5) {
            return mMarkers[mIndex + 1];
        }
        return 0;
    }

    /* 0x08 */ UnkActor* mMarkers[6];
    /* 0x20 */ u8 mIndex;
    /* 0x24 */ UnkVec3 mFrom;
    /* 0x30 */ f32 mStartX;
    /* 0x34 */ f32 mEndX;
};

static inline f32 InvLerp(f32 a, f32 b, f32 x) {
    return (x - a) / (b - a);
}

static inline f32 Lerp(f32 a, f32 b, f32 t) {
    return a + t * (b - a);
}

static inline void SetPos(UnkActor* actor, UnkVec3 pos) {
    actor->mPos = pos;
}

// NonMatching (94.6 %): in the DOL the marker position is copied from mFrom to the
// stack (reusing the mFrom.y load of the interpolation) before x and y are
// overwritten; float registers differ.
void LoadProgressProjectile::vf0C(f32 progress) {
    if (mMarkers[5] == 0) {
        return;
    }
    mProgress = progress;
    f32 x = mStartX + progress * ((mEndX - mStartX) / 100.0f);
    if (mIndex < 5 && mMarkers[mIndex + 1]->mPos.x < x) {
        mMarkers[mIndex]->vf30(2);
        mIndex++;
        mMarkers[mIndex]->vf2C(2);
        mFrom = mMarkers[mIndex]->mPos;
    }
    if (mIndex < 5) {
        f32 t = InvLerp(mFrom.x, GetNext()->mPos.x, x);
        f32 y = Lerp(mFrom.y, GetNext()->mPos.y, t);
        UnkVec3 pos(x, y, mFrom.z);
        SetPos(mMarkers[mIndex], pos);
        mMarkers[mIndex]->vf2C(8);
    }
}

extern "C" LoadProgressProjectile* fn_80120FD8(UnkActor** markers) {
    LoadProgressProjectile* item = new LoadProgressProjectile;
    for (int i = 0; i < 6; i++) {
        item->mMarkers[i] = markers[i];
        if (i != 0) {
            markers[i]->vf30(2);
        }
    }
    item->mProgress = 0.0f;
    item->mIndex = 0;
    item->mFrom = item->mMarkers[0]->mPos;
    item->mStartX = item->mMarkers[0]->mPos.x;
    item->mEndX = item->mMarkers[5]->mPos.x;
    return item;
}
