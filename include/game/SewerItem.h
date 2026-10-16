#ifndef GAME_SEWERITEM_H
#define GAME_SEWERITEM_H

// Class "SewerItem" (RTTI), defined in the game unit at 0x80030228-0x80030604, and
// its subclasses "SewerBottle", "SewerMetal", "SewerRing", "SewerBubble" and
// "SewerFish" (RTTI), defined in the unit at 0x80030604-0x8003107C. Real file
// names unknown; method names unknown (vfXX = vtable offset).

#include "game/unk_vec3.h"

struct UnkVec2 {
    f32 x, y;
};

// Actor that shows a sewer item: its vtable pointer is at 0x138 (real type unknown).
struct UnkSewerActorBase {
    /* 0x00 */ void* m0;
    /* 0x04 */ u8 unk4[0x10 - 0x4];
    /* 0x10 */ int m10;
    /* 0x14 */ u8 unk14[0x68 - 0x14];
    /* 0x68 */ UnkVec3 mPos;
    /* 0x74 */ UnkVec3 m74;
    /* 0x80 */ u8 unk80[0x138 - 0x80];
};

class UnkSewerActor : public UnkSewerActorBase {
public:
    virtual void vf08();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C(int arg);
};

class SewerItem {
public:
    SewerItem();
    virtual ~SewerItem();
    // Whether pos is near the item.
    virtual int vf0C(UnkVec3 pos);
    virtual void vf10();
    // Unlinks and hides the item; returns the next item.
    virtual SewerItem* vf14();
    virtual void vf18();
    virtual void vf1C(f32 arg);

    void fn_800302B0(UnkSewerActor* actor);
    void fn_80030348(SewerItem* other);
    void fn_80030414(const UnkVec2* pos, f32 arg);

    /* 0x00 */ // vtable
    /* 0x04 */ UnkSewerActor* mActor;
    /* 0x08 */ SewerItem* mPrev;
    /* 0x0C */ SewerItem* mNext;
    /* 0x10 */ int m10;
    /* 0x14 */ int mTimer;
    /* 0x18 */ f32 mInterval;
    /* 0x1C */ int mType;
    /* 0x20 */ u8 mActive;
    /* 0x24 */ int m24;
    /* 0x28 */ f32 mRadiusSq;
    /* 0x2C */ UnkVec3 mHome;
};

class SewerBottle : public SewerItem {
public:
    SewerBottle();
    virtual ~SewerBottle() {}
    virtual int vf0C(UnkVec3 pos) { return SewerItem::vf0C(pos); }
    virtual void vf10() { SewerItem::vf10(); }
    virtual void vf18();
    virtual void vf1C(f32 arg) { SewerItem::vf1C(arg); }
    virtual SewerItem* vf14() { return SewerItem::vf14(); }
};

class SewerMetal : public SewerItem {
public:
    SewerMetal();
    virtual ~SewerMetal() {}
    virtual int vf0C(UnkVec3 pos) { return SewerItem::vf0C(pos); }
    virtual void vf10() { SewerItem::vf10(); }
    virtual void vf18();
    virtual void vf1C(f32 arg) { SewerItem::vf1C(arg); }
    virtual SewerItem* vf14() { return SewerItem::vf14(); }
};

class SewerRing : public SewerItem {
public:
    SewerRing();
    virtual ~SewerRing() {}
    virtual int vf0C(UnkVec3 pos) { return SewerItem::vf0C(pos); }
    virtual void vf10() { SewerItem::vf10(); }
    virtual void vf18();
    virtual void vf1C(f32 arg) { SewerItem::vf1C(arg); }
    virtual SewerItem* vf14() { return SewerItem::vf14(); }
};

class SewerBubble : public SewerItem {
public:
    SewerBubble();
    virtual ~SewerBubble() {}
    void SetM38(int value);
    virtual int vf0C(UnkVec3 pos) { return 0; }
    virtual void vf10() { SewerItem::vf10(); }
    virtual void vf18();
    virtual void vf1C(f32 arg);
    virtual SewerItem* vf14() { return SewerItem::vf14(); }

    /* 0x38 */ int m38;
};

class SewerFish : public SewerItem {
public:
    SewerFish();
    virtual ~SewerFish() {}
    virtual int vf0C(UnkVec3 pos);
    virtual void vf10();
    virtual SewerItem* vf14();
    virtual void vf18();
    virtual void vf1C(f32 arg);

    /* 0x38 */ int mState;
    /* 0x3C */ int m3C;
    /* 0x40 */ int mSound;
};

#endif // GAME_SEWERITEM_H
