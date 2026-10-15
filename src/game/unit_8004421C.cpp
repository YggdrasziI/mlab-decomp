// Game unit at 0x8004421C-0x800444FC (real file name unknown): the
// PlayerInputGestureCallback class. Bounds come from the static initializers of this
// unit and the previous one.

#include "game/GestureCallback.h"
#include "game/externs.h"
#include "game/unk_vec3.h"

static inline void SendMessage(u32 flag) {
    UnkMessage* message = fn_802209D8(3);
    if (message != 0) {
        message->m4 = fn_80220A2C();
        message->mC = 40;
        message->m24 = flag;
        message->m14 = 0;
        message->m14 = fn_80220890(message, 40);
    }
}

PlayerInputGestureCallback::PlayerInputGestureCallback() : mAnim(lbl_803C4298.m38) {
    mTimer = -1.0f;
    mTimerStart = 0.0f;
    UnkAnimParam param(-1, 0, 0);
    mAnim.fn_801F8F34(0.7f);
    mAnim.fn_801F8FA0(1.5f);
    mAnim.m98 = 0;
    mAnim.fn_801F8628(&param, 1);
    mAnim.m94 = 1.0f / 12.0f;
}

PlayerInputGestureCallback::~PlayerInputGestureCallback() {
    fn_801B3308()->fn_801B3378(&mAnim);
}

int PlayerInputGestureCallback::vf0C(f32 dt) {
    if (m45) {
        m44 = 1;
        return m46;
    }
    if (mAnim.m90 && mAnim.m94 > 0.0f) {
        mAnim.m8 = mAnim.m94;
    }
    if (mAnim.m90) {
        m44 = 1;
        fn_801B3308()->fn_801B3378(&mAnim);
        SendMessage(1);
        return 1;
    }
    if (mTimer < 0.0f) {
        m44 = 1;
        fn_801B3308()->fn_801B3378(&mAnim);
        SendMessage(0);
        return 0;
    }
    mTimer -= dt;
    return 0;
}

void PlayerInputGestureCallback::vf10() {
    m44 = 0;
    mTimer = mTimerStart;
    fn_801B3308()->fn_801B3310(&mAnim);
}
