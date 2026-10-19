// Game unit at 0x80106AA8-0x8010711C (real file name unknown): accessors for two
// global register sets (bits, ints and floats; the strings "bit %s[%d] = %d",
// "int %s[%d] = %d" and "f32 %s[%d] = %5.2f" name the kinds) and for the register
// set of an object. Bounds come from the static initializers of this unit and the
// previous one.

#include "game/unk_vec3.mch"
#include "types.h"

// Register set (0x100 bytes; the accessors are at 0x801DFE94-0x801E0040, unit not
// split yet). Bits and ints share the words after the header. Real names unknown.
struct UnkRegisters {
    /* 0x00 */ u32 m0;
    /* 0x04 */ u32 mWords[0x3F];
};

// Default register set used when an object has none.
static UnkRegisters lbl_803C7270;

#include "game/unk_803E6D00.h"
#include "game/unk_vec3.h"

// Global objects (real types unknown).
struct UnkGameRegisters {
    /* 0x000 */ u8 unk0[0x5C8];
    /* 0x5C8 */ UnkRegisters m5C8;
    /* 0x6C8 */ UnkRegisters m6C8;
};
extern UnkGameRegisters lbl_803B53A0;

struct UnkGlobals803C2A20 {
    /* 0x000 */ u8 unk0[0x860];
    /* 0x860 */ UnkRegisters m860;
};
extern UnkGlobals803C2A20 lbl_803C2A20;

// Object with a register set at 0x58 (real type unknown).
struct UnkRegisterOwner {
    /* 0x00 */ u8 unk0[0x58];
    /* 0x58 */ UnkRegisters* mRegisters;
};

extern "C" {
void fn_801DFE94(UnkRegisters* dst, const void* src);
void fn_801DFEA8(UnkRegisters* regs, int index, int value);
int fn_801DFF00(UnkRegisters* regs, int index);
void fn_801DFF3C(UnkRegisters* regs, int index, int value);
int fn_801DFF54(UnkRegisters* regs, int index);
void fn_801DFF74(UnkRegisters* regs, int index, f32 value);
f32 fn_801DFF9C(UnkRegisters* regs, int index);
void fn_801DFFCC(UnkRegisters* regs, int index, int value);
void fn_801E0004(UnkRegisters* regs, int index, f32 value);

// Debug print, compiled out (level, channel, format, ...).
void fn_8000B2F0(int level, int channel, const char* format, ...);
}

static inline void PrintFloat(UnkRegisters* regs, int index) {
    fn_8000B2F0(1, 3, "f32 %s[%d] = %5.2f\n", "", index, fn_801DFF9C(regs, index));
}

static inline void PrintInt(UnkRegisters* regs, int index) {
    fn_8000B2F0(1, 3, "int %s[%d] = %d\n", "", index, fn_801DFF54(regs, index));
}

static inline void PrintBit(UnkRegisters* regs, int index) {
    fn_8000B2F0(1, 3, "bit %s[%d] = %d\n", "", index, fn_801DFF00(regs, index));
}

// Never called, so not emitted; it only gives the string pool its order in the DOL
// (f32, int, bit: the reverse of the print functions below). Hypothesis: the
// original file had an unused function like this one.
static void UnusedDump(UnkRegisters* regs, int index) {
    PrintFloat(regs, index);
    PrintInt(regs, index);
    PrintBit(regs, index);
}

static inline UnkRegisters* GetRegisters(UnkRegisterOwner* owner) {
    if (owner != 0 && owner->mRegisters != 0) {
        return owner->mRegisters;
    }
    return &lbl_803C7270;
}

extern "C" {

void fn_80106AA8(int index, int value) {
    fn_801DFEA8(&lbl_803B53A0.m5C8, index, value);
}

int fn_80106AC0(int index) {
    return fn_801DFF00(&lbl_803B53A0.m5C8, index);
}

void fn_80106AD4(int index, int value) {
    fn_801DFF3C(&lbl_803B53A0.m5C8, index, value);
}

int fn_80106AEC(int index) {
    return fn_801DFF54(&lbl_803B53A0.m5C8, index);
}

void fn_80106B00(int index, f32 value) {
    fn_801DFF74(&lbl_803B53A0.m5C8, index, value);
}

f32 fn_80106B14(int index) {
    return fn_801DFF9C(&lbl_803B53A0.m5C8, index);
}

void fn_80106B28(int index, int value) {
    fn_801DFFCC(&lbl_803B53A0.m5C8, index, value);
}

void fn_80106B40(int index, f32 value) {
    fn_801E0004(&lbl_803B53A0.m5C8, index, value);
}

void fn_80106B54(int index) {
    PrintBit(&lbl_803B53A0.m5C8, index);
}

void fn_80106BB4(int index) {
    PrintInt(&lbl_803B53A0.m5C8, index);
}

void fn_80106C14(int index) {
    PrintFloat(&lbl_803B53A0.m5C8, index);
}

void fn_80106C70(int index, int value) {
    fn_801DFEA8(&lbl_803B53A0.m6C8, index, value);
}

int fn_80106C88(int index) {
    return fn_801DFF00(&lbl_803B53A0.m6C8, index);
}

void fn_80106C9C(int index, int value) {
    fn_801DFF3C(&lbl_803B53A0.m6C8, index, value);
}

int fn_80106CB4(int index) {
    return fn_801DFF54(&lbl_803B53A0.m6C8, index);
}

void fn_80106CC8(int index, f32 value) {
    fn_801DFF74(&lbl_803B53A0.m6C8, index, value);
}

f32 fn_80106CDC(int index) {
    return fn_801DFF9C(&lbl_803B53A0.m6C8, index);
}

void fn_80106CF0(int index, int value) {
    fn_801DFFCC(&lbl_803B53A0.m6C8, index, value);
}

void fn_80106D08(int index, f32 value) {
    fn_801E0004(&lbl_803B53A0.m6C8, index, value);
}

void fn_80106D1C(int index) {
    PrintBit(&lbl_803B53A0.m6C8, index);
}

void fn_80106D7C(int index) {
    PrintInt(&lbl_803B53A0.m6C8, index);
}

void fn_80106DDC(int index) {
    PrintFloat(&lbl_803B53A0.m6C8, index);
}

void fn_80106E38(UnkRegisterOwner* owner, int index, int value) {
    fn_801DFEA8(GetRegisters(owner), index, value);
}

int fn_80106E5C(UnkRegisterOwner* owner, int index) {
    return fn_801DFF00(GetRegisters(owner), index);
}

void fn_80106E80(UnkRegisterOwner* owner, int index, int value) {
    fn_801DFF3C(GetRegisters(owner), index, value);
}

int fn_80106EA4(UnkRegisterOwner* owner, int index) {
    return fn_801DFF54(GetRegisters(owner), index);
}

void fn_80106EC8(UnkRegisterOwner* owner, int index, f32 value) {
    fn_801DFF74(GetRegisters(owner), index, value);
}

f32 fn_80106EEC(UnkRegisterOwner* owner, int index) {
    return fn_801DFF9C(GetRegisters(owner), index);
}

void fn_80106F10(UnkRegisterOwner* owner, int index, int value) {
    fn_801DFFCC(GetRegisters(owner), index, value);
}

void fn_80106F34(UnkRegisterOwner* owner, int index, f32 value) {
    fn_801E0004(GetRegisters(owner), index, value);
}

void fn_80106F58(UnkRegisterOwner* owner, int index) {
    PrintBit(GetRegisters(owner), index);
}

void fn_80106FCC(UnkRegisterOwner* owner, int index) {
    PrintInt(GetRegisters(owner), index);
}

void fn_80107040(UnkRegisterOwner* owner, int index) {
    PrintFloat(GetRegisters(owner), index);
}

void fn_801070B0(int which) {
    if (which == 0x1FF) {
        fn_801DFE94(&lbl_803B53A0.m5C8, &lbl_803C2A20.m860);
    } else if (which == 0) {
        fn_801DFE94(&lbl_803B53A0.m6C8, (u8*)lbl_803E6D00.m0 + 0xA4);
    }
}

}
