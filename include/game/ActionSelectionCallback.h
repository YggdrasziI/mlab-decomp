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

// Name holder at XBaseActionSelectionCallback+0xC (real type unknown; 0x40 bytes).
class UnkCallbackName {
public:
    void fn_8022127C(const char* name);

    /* 0x00 */ u8 unk0[0x40];
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

    void SetName(const char* name) { mName.fn_8022127C(name); }

    /* 0x00 */ // vtable
    /* 0x04 */ u8 m4;
    /* 0x05 */ u8 m5;
    /* 0x08 */ void* m8;
    /* 0x0C */ UnkCallbackName mName;
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

class UnkDebugMenu;
class UnkMenu;

// User data of one debug menu line (real type unknown).
struct UnkDebugMenuEntry {
    /* 0x0 */ UnkDebugMenu* mOwner;
    /* 0x4 */ void* mAction;
    /* 0x8 */ s32 mIndex;
};

// Debug menu driven by DebugMenuActionSelectionCallback (0x9C bytes, game unit at
// 0x8003FB24-0x80040090; real name unknown).
class UnkDebugMenu {
public:
    UnkDebugMenu();
    ~UnkDebugMenu();
    void fn_8003FC00(void* arg);
    void fn_8003FC6C(void* arg);
    void fn_8003FF30();
    u32 fn_8003FFA4(UnkActionSelection* selection);
    s32 fn_8003FFD0() const;
    void fn_80040028();

    /* 0x00 */ UnkDebugMenuEntry mEntries[10];
    /* 0x78 */ u8 unk78[0x84 - 0x78];
    /* 0x84 */ UnkMenu* m84;
    /* 0x88 */ u8 mHasSelection;
    /* 0x8C */ UnkActionSelection mSelection;
    /* 0x98 */ u8 m98;
};

// Global object at 0x8047C7D8 (0x18 bytes, real type unknown).
class UnkFocus {
public:
    void* fn_801B41F0();
    void fn_801B4090(UnkMenu* menu);
    void fn_801B40E4();

    /* 0x00 */ u8 m0;
    /* 0x01 */ u8 unk1[0x18 - 0x1];
};

extern UnkFocus lbl_8047C7D8;

class DebugMenuActionSelectionCallback : public XBaseActionSelectionCallback {
public:
    DebugMenuActionSelectionCallback(void* arg);
    virtual ~DebugMenuActionSelectionCallback();
    virtual void vf0C();

    /* 0x4C */ UnkDebugMenu* mMenu;
    /* 0x50 */ bool mFocused;
};

#endif // GAME_ACTIONSELECTIONCALLBACK_H
