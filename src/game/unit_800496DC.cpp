// Game unit at 0x800496DC-0x800497F4 (real file name unknown): the
// CompositeCharacterActionSelectionCB class. Bounds come from the static
// initializers of this unit and the previous one.

#include "game/ActionSelectionCallback.h"
#include "game/unk_vec3.h"

CompositeCharacterActionSelectionCB::CompositeCharacterActionSelectionCB(void* arg)
    : XBaseActionSelectionCallback(arg) {
    mPending = 0;
}

CompositeCharacterActionSelectionCB::~CompositeCharacterActionSelectionCB() {}

void CompositeCharacterActionSelectionCB::vf0C() {
    if (mPending) {
        fn_8020F46C(&mSelection);
        mPending = 0;
    }
}
