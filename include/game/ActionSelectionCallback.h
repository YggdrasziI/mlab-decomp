#ifndef GAME_ACTIONSELECTIONCALLBACK_H
#define GAME_ACTIONSELECTIONCALLBACK_H

// Classes "XBaseActionSelectionCallback" and "CompositeCharacterActionSelectionCB"
// (names from RTTI). The base class and UnkActionSelection functions are at
// 0x8020F36C-0x8020F498 (unit not split yet); the derived class is in the game unit
// at 0x800496DC-0x800497F4. Real file names unknown; method names unknown (vfXX =
// vtable offset).

#include "types.h"

// 12-byte selection record (real name unknown).
class UnkActionSelection {
public:
    UnkActionSelection();
    ~UnkActionSelection();

    // Both write 0x4, 0x8 then 0x0, as in the DOL.
    void Set(s32 a, u8 b, s32 c) {
        m4 = b;
        m8 = c;
        m0 = a;
    }
    UnkActionSelection& operator=(const UnkActionSelection& other) {
        m4 = other.m4;
        m8 = other.m8;
        m0 = other.m0;
        return *this;
    }

    /* 0x0 */ s32 m0;
    /* 0x4 */ u8 m4;
    /* 0x8 */ s32 m8;
};

// Object given to vf10 (real type unknown).
struct UnkActionRequest {
    /* 0x00 */ u8 unk0[0x24];
    /* 0x24 */ s32 m24;
    /* 0x28 */ u8 unk28[0x2C - 0x28];
    /* 0x2C */ s32 m2C;
};

class XBaseActionSelectionCallback {
public:
    // Also builds the member at 0xC with the name "NAME THIS CALLBACK!!!".
    XBaseActionSelectionCallback(void* arg);
    virtual ~XBaseActionSelectionCallback();
    virtual void vf0C() = 0;
    virtual void vf10(UnkActionRequest* request);

    // Copies the selection into the object at m8 (offset 0x38) and sets m4.
    void fn_8020F46C(UnkActionSelection* selection);

    /* 0x00 */ // vtable
    /* 0x04 */ u8 m4;
    /* 0x05 */ u8 m5;
    /* 0x08 */ void* m8;
    /* 0x0C */ u8 unkC[0x40];
};

class CompositeCharacterActionSelectionCB : public XBaseActionSelectionCallback {
public:
    CompositeCharacterActionSelectionCB(void* arg);
    virtual ~CompositeCharacterActionSelectionCB();
    virtual void vf0C();

    /* 0x4C */ UnkActionSelection mSelection;
    /* 0x58 */ u8 mPending;
};

class NetworkInputActionSelectionCallback : public XBaseActionSelectionCallback {
public:
    NetworkInputActionSelectionCallback(void* arg);
    virtual ~NetworkInputActionSelectionCallback();
    virtual void vf0C();
    virtual void vf10(UnkActionRequest* request);

    /* 0x4C */ UnkActionSelection mCurrent;
    /* 0x58 */ u8 mHasCurrent;
    /* 0x5C */ UnkActionSelection mNext;
    /* 0x68 */ u8 mHasNext;
};

#endif // GAME_ACTIONSELECTIONCALLBACK_H
