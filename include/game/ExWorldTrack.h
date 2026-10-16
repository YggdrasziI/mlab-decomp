#ifndef GAME_EXWORLDTRACK_H
#define GAME_EXWORLDTRACK_H

// Class "ExWorldTrack" (RTTI; functions at 0x800C2670-0x800C3E84, unit not split
// yet) and its subclass "ExWorldWireTrack" (RTTI; unit at 0x800C42B8-0x800C4530).
// Method names unknown (vfXX = vtable offset).

#include "game/Entity.h"
#include "game/unk_vec3.h"

// Ring of up to 20 tiles built into ExWorldTrack (real type unknown). Its accessors
// are inlined in several units.
struct UnkTileRing {
    // Read through a const accessor: the DOL reuses this load for the index.
    int GetHead() const { return mHead; }
    bool IsEmpty() const { return mHead < 0 || mTail < 0; }
    bool Contains(int index) const {
        if (index < 0 || index >= mCapacity) {
            return false;
        }
        int head = mHead;
        int tail = mTail;
        if (tail < head) {
            if (index <= tail) {
                index += mCapacity;
            }
            tail += mCapacity;
        }
        return head <= index && index <= tail;
    }
    int Count() const {
        if (IsEmpty()) {
            return 0;
        }
        int tail = mTail;
        if (tail < mHead) {
            tail += mCapacity;
        }
        return tail - mHead + 1;
    }
    ExWorldTile** At(const int& index) {
        if (Contains(index)) {
            return &mItems[index];
        }
        return 0;
    }
    ExWorldTile* First() {
        if (!IsEmpty()) {
            mCurrent = At(GetHead());
            if (mCurrent != 0) {
                return *mCurrent;
            }
        }
        return 0;
    }

    /* 0x00 */ int mHead;
    /* 0x04 */ int mTail;
    /* 0x08 */ int mCapacity;
    /* 0x0C */ ExWorldTile** mItems;
    /* 0x10 */ ExWorldTile** mCurrent;
    /* 0x14 */ ExWorldTile* mStorage[20];
};

class ExWorldTrack {
public:
    /* 0x00 */ int m0;
    /* 0x04 */ int m4;
    /* 0x08 */ int m8;
    /* 0x0C */ u8 unkC[0x60 - 0xC];
    /* 0x60 */ void* mOwner;
    /* 0x64 */ u8 unk64[0x68 - 0x64];
    /* 0x68 */ int m68;
    /* 0x6C */ UnkTileRing mTiles;
    /* 0xD0 */ int mD0;

    ExWorldTrack();
    virtual ~ExWorldTrack();
    virtual void vf0C(void* owner);
    virtual void vf10();
    virtual void vf14(f32 arg);
    virtual void vf18();
    virtual void vf1C() = 0;
    virtual void vf20() = 0;
    virtual UnkVec3 vf24() = 0;
    virtual UnkVec3 vf28() = 0;
    virtual bool vf2C(ExWorldTile* tile);
    virtual bool vf30(ExWorldTile* tile);
    virtual void vf34(ExWorldTile* tile, ExWorldTile* other) {}
    virtual void vf38(ExWorldTile* tile) {}
    /* 0xD4 */ // vtable (placed after the data members declared first)
};

// Object built into ExWorldWireTrack (real type unknown; functions at 0x8010B4AC).
struct UnkWirePath {
    UnkWirePath();
    ~UnkWirePath();

    /* 0x000 */ u8 unk0[0xB0C];
};

class ExWorldWireTrack : public ExWorldTrack {
public:
    ExWorldWireTrack();
    virtual ~ExWorldWireTrack();
    virtual void vf0C(void* owner);
    virtual void vf10();
    virtual void vf1C();
    virtual void vf20();
    virtual UnkVec3 vf24();
    virtual UnkVec3 vf28();
    virtual void vf38(ExWorldTile* tile);
    virtual void vf34(ExWorldTile* tile, ExWorldTile* other);

    /* 0xD8 */ UnkWirePath mPath;
};

// Class "ExWorldCurvedTrack" (RTTI; functions at 0x800C52B4-0x800C5xxx, unit not
// split yet).
class ExWorldCurvedTrack : public ExWorldTrack {
public:
    ExWorldCurvedTrack();
    virtual ~ExWorldCurvedTrack();
    virtual void vf0C(void* owner);
    virtual void vf10();
    virtual void vf1C();
    virtual void vf20();
    virtual UnkVec3 vf24();
    virtual UnkVec3 vf28();
    virtual void vf38(ExWorldTile* tile);

    /* 0xD8 */ f32 mD8;
};

// Class "AstralTrack" (RTTI; unit at 0x8009BE14-0x8009C2C8).
class AstralTrack : public ExWorldCurvedTrack {
public:
    AstralTrack();
    virtual ~AstralTrack();
    virtual void vf0C(void* owner);
    virtual void vf10();
    virtual bool vf2C(ExWorldTile* tile);
    virtual bool vf30(ExWorldTile* tile);
    virtual void vf34(ExWorldTile* tile, ExWorldTile* other);

    /* 0xDC */ int mDC;
    /* 0xE0 */ int mE0;
};

#endif // GAME_EXWORLDTRACK_H
