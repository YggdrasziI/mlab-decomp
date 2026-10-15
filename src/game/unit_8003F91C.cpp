// Game unit at 0x8003F91C-0x8003FB24 (real file name unknown): the
// DebugMenuActionSelectionCallback class. Bounds come from the static initializers
// of this unit and the previous one.

#include "game/ActionSelectionCallback.h"
#include "game/unk_vec3.h"

DebugMenuActionSelectionCallback::DebugMenuActionSelectionCallback(void* arg)
    : XBaseActionSelectionCallback(arg), mMenu(0), mFocused(false) {
    mMenu = new UnkDebugMenu;
    m5 = 1;
    SetName("Debug menu callback");
}

DebugMenuActionSelectionCallback::~DebugMenuActionSelectionCallback() {
    delete mMenu;
}

void DebugMenuActionSelectionCallback::vf0C() {
    if (mMenu->m84 == 0) {
        mMenu->fn_8003FC00(m8);
    }
    void* current = mMenu->m84;
    bool focused = lbl_8047C7D8.fn_801B41F0() == current;
    if (!mMenu->m98 || (focused == true && !mFocused)) {
        mMenu->fn_8003FC6C(m8);
    }
    mFocused = focused;
    mMenu->fn_8003FF30();
    UnkActionSelection selection;
    if (mMenu->fn_8003FFA4(&selection) == 1) {
        fn_8020F46C(&selection);
        mMenu->fn_80040028();
        return;
    }
    mMenu->fn_8003FFD0();
}
