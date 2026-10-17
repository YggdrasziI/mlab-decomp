// Game unit at 0x800445E4-0x80044C58 (real file name unknown): the HudPowerMeter
// class (RTTI), a frame allocator and texture/render state set-up written straight
// to the GX FIFO. Bounds come from the static initializers of this unit and the
// previous one.

#include <revolution/gx/GXHardware.h>
#include "game/unk_vec3.h"

// Load a BP register (real name unknown). The DOL computes the value before writing
// the command byte, as for an inline function's argument.
inline void UnkLoadBP(u32 value) {
    GX_WRITE_U8(0x61);
    GX_WRITE_U32(value);
}

// Load one XF register (real name unknown).
inline void UnkLoadXF(u16 addr, u32 value) {
    GX_WRITE_U8(0x10);
    GX_WRITE_U16(0);
    GX_WRITE_U16(addr);
    GX_WRITE_U32(value);
}

class HudPowerMeter {
public:
    HudPowerMeter();
    virtual ~HudPowerMeter();
};

HudPowerMeter::HudPowerMeter() {}

HudPowerMeter::~HudPowerMeter() {}

// Frame allocator state (real names unknown).
extern int lbl_805FB4C4; // used size
extern int lbl_805FB4CC; // free size
extern u8* lbl_805FB4D0; // base

// Grows the frame heap (real name unknown).
extern "C" void fn_8014E3EC(int size);

extern "C" void* fn_80044634(int size) {
    size = (size + 0x1F) & ~0x1F;
    if (lbl_805FB4CC < size) {
        fn_8014E3EC(size);
    }
    u8* p = lbl_805FB4D0 + lbl_805FB4C4;
    lbl_805FB4CC -= size;
    lbl_805FB4C4 += size;
    return p;
}

extern "C" void fn_80044694() {
    UnkLoadBP(0xFE000001);
    UnkLoadBP(0x41000000);
}

// Texture description (real type unknown).
struct UnkTexture {
    /* 0x00 */ u8 unk0[0x8];
    /* 0x08 */ u16 mWidth;
    /* 0x0A */ u16 mHeight;
    /* 0x0C */ u8 unkC[0x11 - 0xC];
    /* 0x11 */ u8 mTlutCount;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 m13;
    /* 0x14 */ u8 mFormat;
    /* 0x15 */ u8 mTlutFormat;
    /* 0x16 */ u8 m16;
    /* 0x17 */ u8 mRegion;
    /* 0x18 */ u8 unk18[0x1C - 0x18];
    /* 0x1C */ u32 mImage;
    /* 0x20 */ u32 mTlut;
};

// 0x94-byte entries (real type unknown).
struct UnkFrameInfo {
    /* 0x00 */ u8 unk0[0x94];
};

struct UnkContext;

extern u8 lbl_805FB50C;
extern int lbl_805FB4B0;
extern int lbl_805FB4BC;
extern int lbl_805FB4C0;
extern int lbl_805FB4C8;
extern UnkFrameInfo lbl_80401108[];
// Per texture map slot: image loaded last and a reload flag (real names unknown).
extern u32 lbl_8040ADF0[8];
extern int lbl_8040AE10[8];
extern const u32 lbl_80348950[][8];
extern const u32 lbl_80348A10[][8];
extern const u32 lbl_80348AD0[8];
extern "C" int fn_8015BA30(UnkFrameInfo* info);
extern "C" void fn_8014DE68();

// Loads tex into texture map 0 and switches the TEV set-up between textured and
// untextured drawing (tex == 0). NonMatching (93.8 %): same instructions, but the DOL
// schedules the register values of the image and TLUT loads differently (it computes
// them before the first write, apparently through a register-building helper).
extern "C" void fn_800446BC(UnkContext* context, UnkTexture* tex) {
    if (tex != 0) {
        if (lbl_805FB4C0 != 0) {
            if (fn_8015BA30(&lbl_80401108[lbl_805FB4BC]) + 0x10C0 > lbl_805FB4C8) {
                fn_8014DE68();
            }
        }
        if (!(lbl_805FB50C & 2)) {
            lbl_805FB50C |= 2;
            if (lbl_805FB50C & 1) {
                UnkLoadBP(0xFE00000F);
                UnkLoadBP(0x00000001);
                UnkLoadXF(0x103F, 1);
                UnkLoadBP(0xFE003C00);
                UnkLoadBP(0x00000400);
                UnkLoadXF(0x1010, 0x441);
                UnkLoadBP(0x283C0000);
                UnkLoadBP(0xC008F2A4);
                UnkLoadBP(0xC108E6F0);
                UnkLoadBP(0xC208F80F);
                UnkLoadBP(0xC308F070);
            } else {
                UnkLoadBP(0xFE00000F);
                UnkLoadBP(0x00000001);
                UnkLoadXF(0x103F, 1);
                UnkLoadBP(0xFE003C00);
                UnkLoadBP(0x00000000);
                UnkLoadXF(0x100E, 0x442);
                UnkLoadXF(0x1010, 0x442);
                UnkLoadBP(0x28380040);
                UnkLoadBP(0xC008F8AF);
                UnkLoadBP(0xC108F2F0);
            }
        }
        UnkLoadBP(0xFE000060);
        UnkLoadBP(0x80000000 | (tex->m16 << 5));
        if (lbl_8040ADF0[0] != tex->mImage || lbl_8040AE10[0] != 0) {
            lbl_805FB4B0++;
            lbl_8040ADF0[0] = tex->mImage;
            u32 tlutTmem = lbl_80348AD0[0];
            lbl_8040AE10[0] = 0;
            u32 region = tex->mRegion;
            UnkLoadBP(0x84000000 | (((tex->m13 - 1) & 0xF) << 12));
            u32 even = lbl_80348950[region][0];
            u32 odd = lbl_80348A10[region][0];
            UnkLoadBP(0x88000000 | (tex->mFormat << 20) | ((tex->mHeight - 1) << 10) |
                   (tex->mWidth - 1));
            UnkLoadBP(0x8C0D8000 | (even >> 5));
            if ((s32)odd >= 0) {
                UnkLoadBP(0x900D8000 | (odd >> 5));
            } else {
                UnkLoadBP(0x90000000);
            }
            UnkLoadBP(0x94000000 | ((tex->mImage >> 5) & 0xFFFFFF));
            if (tex->mTlutCount != 0) {
                u32 tmem = (tlutTmem >> 9) & 0x3FF;
                u32 load = 0x98000000 | tmem;
                load = (load & ~0x3FC00) | ((tex->mTlutFormat << 10) & 0x3FC00);
                u32 count = 0x65000000 | tmem;
                count = (count & ~0x1FC00) | (((tex->mTlutCount >> 1) << 10) & 0x1FC00);
                u32 addr = 0x64000000 | ((tex->mTlut >> 5) & 0xFFFFFF);
                UnkLoadBP(load);
                UnkLoadBP(0xFEFFFF00);
                UnkLoadBP(0x0F000000);
                UnkLoadBP(addr);
                UnkLoadBP(count);
                UnkLoadBP(0xFEFFFF00);
                UnkLoadBP(0x0F000000);
            }
        }
        UnkLoadBP(0x30000000 | (tex->mWidth - 1));
        UnkLoadBP(0x31000000 | (tex->mHeight - 1));
    } else if (lbl_805FB50C & 2) {
        lbl_805FB50C &= ~2;
        if (lbl_805FB50C & 1) {
            UnkLoadBP(0xFE00000F);
            UnkLoadBP(0x00000000);
            UnkLoadXF(0x103F, 0);
            UnkLoadBP(0xFE003C00);
            UnkLoadBP(0x00000000);
            UnkLoadXF(0x1010, 0x441);
            UnkLoadBP(0x28380000);
            UnkLoadBP(0xC008F2A4);
            UnkLoadBP(0xC108E6F0);
        } else {
            UnkLoadBP(0xFE00000F);
            UnkLoadBP(0x00000000);
            UnkLoadXF(0x103F, 0);
            UnkLoadBP(0xFE003C00);
            UnkLoadBP(0x00000000);
            UnkLoadXF(0x100E, 0x442);
            UnkLoadXF(0x1010, 0x442);
            UnkLoadBP(0x28380000);
            UnkLoadBP(0xC008FFFA);
            UnkLoadBP(0xC108FFD0);
        }
    }
}

extern "C" void fn_80044C0C() {
    UnkLoadBP(0xFE000001);
    UnkLoadBP(0x41000001);
}
