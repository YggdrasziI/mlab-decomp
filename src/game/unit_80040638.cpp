// Game unit at 0x80040638-0x80040824 (real file name unknown): the
// NetworkInputActionSelectionCallback class. Bounds come from the static
// initializers of this unit and the previous one.

#include "game/ActionSelectionCallback.h"
#include "game/unk_vec3.h"

NetworkInputActionSelectionCallback::NetworkInputActionSelectionCallback(void* arg)
    : XBaseActionSelectionCallback(arg), mHasCurrent(0), mHasNext(0) {
    mCurrent.Set(0x4E, 0, 4);
    mNext.Set(0x4E, 0, 4);
}

NetworkInputActionSelectionCallback::~NetworkInputActionSelectionCallback() {}

void NetworkInputActionSelectionCallback::vf0C() {
    if (mHasCurrent) {
        fn_8020F46C(&mCurrent);
        if (mHasNext) {
            mHasCurrent = 1;
            mCurrent = mNext;
            mHasNext = 0;
        } else {
            mHasCurrent = 0;
        }
    }
}

void NetworkInputActionSelectionCallback::vf10(UnkActionRequest* request) {
    if (request == 0) {
        return;
    }
    if (mHasCurrent) {
        mHasNext = 1;
        mNext.m4 = 0;
        mNext.m8 = request->m2C;
        mNext.m0 = request->m24;
    } else {
        mHasCurrent = 1;
        mCurrent.m4 = 0;
        mCurrent.m8 = request->m2C;
        mCurrent.m0 = request->m24;
    }
}
