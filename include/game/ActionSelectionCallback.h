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

    /* 0x0 */ s32 m0;
    /* 0x4 */ u8 m4;
    /* 0x8 */ s32 m8;
};

class XBaseActionSelectionCallback {
public:
    // Also builds the member at 0xC with the name "NAME THIS CALLBACK!!!".
    XBaseActionSelectionCallback(void* arg);
    virtual ~XBaseActionSelectionCallback();
    virtual void vf0C() = 0;
    virtual void vf10();

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

#endif // GAME_ACTIONSELECTIONCALLBACK_H
