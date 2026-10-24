// Game unit at 0x80014230-0x8001425C (real file name unknown): the build date and
// build numbers. Bounds come from the static initializers of this unit and the
// previous one.

#include "game/unk_vec3.mch"
#include "game/unit_80014230.h"
#include "game/unk_vec3.h"
#include "version.h"

// RM7E4F is an earlier build of the same code (SPEC.md, "Other versions").
#if REGION_USA
const char lbl_803476A8[] = "Jul 31 2008 (16:44)";

const int lbl_805FC720 = 517;
const int lbl_805FC724 = 142067;
#else
const char lbl_803476A8[] = "Aug 28 2008 (14:30)";

const int lbl_805FC720 = 519;
const int lbl_805FC724 = 142835;
#endif

const char* fn_80014230() {
    return lbl_803476A8;
}
