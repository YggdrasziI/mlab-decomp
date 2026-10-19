// Game unit at 0x8010FD14-0x8010FDA4 (real file name unknown): the "book" base
// class. Bounds come from the static initializers of this unit and the previous
// one.

#include "game/unk_vec3.mch"
#include "game/book.h"
#include "game/unk_vec3.h"

book::book() {
    m4 = 0;
}

book::~book() {
    if (m4 != 0) {
        m4 = 0;
    }
}
