// Game unit at 0x800214D4-0x80021594 (real file name unknown): the set of input
// sequences. Bounds come from the unit's static initializer and its own .sdata2
// constants.

#include "game/unit_800214D4.h"

void UnkInputSequenceSet::fn_800214D4() {
    int i;
    for (i = 0; i < mCount; i++) {
        mSequences[i].fn_80020334();
    }
    for (i = 0; i < 8; i++) {
        mFixed[i].fn_80020334();
    }
}

UnkInputSequence* UnkInputSequenceSet::fn_80021550(int id) {
    if (id == 34) {
        return mFixed;
    }
    return &mSequences[mIndices[id]];
}
