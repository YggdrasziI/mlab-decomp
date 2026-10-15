// Game unit at 0x800444FC-0x800445E4 (real file name unknown): the
// NetworkGestureCallback class. Bounds come from the static initializers of this
// unit and the previous one.

#include "game/GestureCallback.h"
#include "game/unk_vec3.h"

NetworkGestureCallback::NetworkGestureCallback() {
    m47 = 0;
}

NetworkGestureCallback::~NetworkGestureCallback() {}

int NetworkGestureCallback::vf0C(f32 dt) {
    m44 = 1;
    m47 = 0;
    return 0;
}

void NetworkGestureCallback::vf10() {
    m44 = 0;
    m47 = 0;
}

void NetworkGestureCallback::vf14() {}
