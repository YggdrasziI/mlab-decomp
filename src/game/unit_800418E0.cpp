// Game unit at 0x800418E0-0x80041AB4 (real file name unknown): the
// CombatOMaticDamageCallback class. Bounds come from the static initializers of this
// unit and the previous one.

#include "game/CombatCallback.h"
#include "game/unk_803C4298.h"
#include "game/unk_vec3.h"

// Global block at 0x803C2A20 (real type unknown).
struct UnkGlobals803C2A20 {
    /* 0x000 */ u8 unk0[0x820];
    /* 0x820 */ u8 m820;
};
extern UnkGlobals803C2A20 lbl_803C2A20;

// Defined in the unsplit unit at 0x80040824 (real names unknown).
extern int lbl_805F8538;
extern "C" void fn_80040EFC(int index, char* buf);
extern "C" void fn_80040F20(int flag, char* buf);

extern "C" int fn_801B9B40(void* obj);
extern "C" int fn_80214F48(void* obj);

extern "C" int fn_80041A8C();

CombatOMaticDamageCallback::CombatOMaticDamageCallback(u8 arg) : XBaseCombatCallback(2) {
    mC = arg;
    mD = 0;
}

CombatOMaticDamageCallback::~CombatOMaticDamageCallback() {}

void CombatOMaticDamageCallback::vf10() {}

// NonMatching (86 %): the original calls fn_80041A8C through ctr (lis r12 / addi /
// mtctr / bctrl) and keeps `match` in r31 for a dead `cmpwi r31, 0` after the two
// calls; no spelling tried reproduces either, so the registers differ too.
void CombatOMaticDamageCallback::vf0C() {
    char indexText[0x100];
    char flagText[0x100];
    if (fn_801B9B40(lbl_803C4298.m38) != 0 || !(fn_80041A8C() & 1) || mC == 0) {
        vf14();
    } else {
        void* obj = m4;
        u8 saved = lbl_803C2A20.m820;
        lbl_803C2A20.m820 = 1;
        for (int i = 0; i < 2; i++) {
            int match = fn_80214F48(obj) == i;
            fn_80040F20(match, flagText);
            fn_80040EFC(i, indexText);
            if (match) {
            }
        }
        lbl_803C2A20.m820 = saved;
    }
}

extern "C" int fn_80041A8C() {
    return lbl_805F8538;
}
