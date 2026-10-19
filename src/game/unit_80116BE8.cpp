// Game unit at 0x80116BE8-0x80116CA8 (real file name unknown): the "section" base
// class. Bounds come from the static initializers of this unit and the previous
// one.

#include "game/unk_vec3.mch"
#include "game/section.h"
#include "game/externs.h"
#include "game/unk_vec3.h"

section::section() {
    m4 = 0;
    m8 = 0;
}

section::~section() {}

const char* section::fn_80116C44() {
    fn_80142EF8(mText, "%d/%d", m4, m8);
    return mText;
}
