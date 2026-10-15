#ifndef GAME_GESTURECALLBACK_H
#define GAME_GESTURECALLBACK_H

// Classes "XBaseGestureCallback" and "NetworkGestureCallback" (names from RTTI).
// The base class's functions are at 0x8020F498-0x8020F570 (unit not split yet);
// NetworkGestureCallback is in the game unit at 0x800444FC-0x800445E4. Real file
// names unknown; method names unknown (vfXX = vtable offset).

#include "game/unk_anim.h"
#include "types.h"

class XBaseGestureCallback {
public:
    // Also builds the member at 0x4 with the name "NAME THIS CALLBACK!!!".
    XBaseGestureCallback();
    virtual ~XBaseGestureCallback();
    virtual int vf0C(f32 dt);
    virtual void vf10();
    virtual void vf14();

    /* 0x00 */ // vtable
    /* 0x04 */ u8 unk4[0x40];
    /* 0x44 */ u8 m44;
    /* 0x45 */ u8 m45;
    /* 0x46 */ u8 m46;
    /* 0x47 */ u8 m47;
};

class NetworkGestureCallback : public XBaseGestureCallback {
public:
    NetworkGestureCallback();
    virtual ~NetworkGestureCallback();
    virtual int vf0C(f32 dt);
    virtual void vf10();
    virtual void vf14();
};

class PlayerInputGestureCallback : public XBaseGestureCallback {
public:
    PlayerInputGestureCallback();
    virtual ~PlayerInputGestureCallback();
    virtual int vf0C(f32 dt);
    virtual void vf10();

    /* 0x048 */ UnkAnimPlayer mAnim;
    /* 0x4CC */ f32 mTimer;
    /* 0x4D0 */ f32 mTimerStart;
};

#endif // GAME_GESTURECALLBACK_H
