// Game unit at 0x801F59F0-0x801F5E3C (real file name unknown). Bounds: the previous
// unit's static initializer (0x801F5830) is followed by the two inline functions it
// uses (0x801F59A4, 0x801F59B0); this unit's static initializer (0x801F5CAC) is
// followed by the destructors it registers, which end at 0x801F5E3C.
//
// Built with "-ipa file" (configure.py): it is the only setting found that emits the
// three generated destructors in the order the static initializer registers them, as
// in the DOL; with the usual flags they come out in reverse. Hypothesis: the real
// cause may be a source construct that was not found.

#include "types.h"

// Size of a message of the given type (real name unknown).
extern "C" u32 fn_801F5294(u32 type);

// Header word of a message (real name unknown). Bits 5-11 hold the type; the low
// half depends on the mode in bits 14-15: below 3 it holds one 14-bit value,
// otherwise a 9-bit and a 5-bit value. The DOL reads the type with a shift and
// writes every field with a mask and a shift, not with bit fields.
struct UnkMsgHeader {
    u32 GetType() const { return (m0 >> 20) & 0x7F; }
    void SetType(u32 type) { m0 = (m0 & ~0x07F00000) | (type << 20); }
    void SetFlagA(u32 flag) { m0 = (m0 & ~0x00080000) | (flag << 19); }
    void SetFlagB(u32 flag) { m0 = (m0 & ~0x00040000) | (flag << 18); }
    void SetMode(u32 mode) { m0 = (m0 & ~0x00030000) | (mode << 16); }
    void SetValue(u32 value) { m0 = (m0 & ~0x0000FFFC) | (value << 2); }
    void SetSplitValue(u32 value) { m0 = (m0 & ~0x0000FF80) | (value << 7); }
    void SetSplitExtra(u32 value) { m0 = (m0 & ~0x0000007C) | (value << 2); }

    /* 0x0 */ u32 m0;
};

// Size of the message at msg (real name unknown).
extern "C" u32 fn_801F5308(UnkMsgHeader* msg);

// Fixed 0x400-byte block of messages (real name unknown). The 0x18-byte header holds
// the used size at 0x10; messages follow it until type 0x6F or the used size.
struct UnkMsgBuffer {
    UnkMsgHeader* fn_801F59F0();
    UnkMsgHeader* fn_801F5A14(UnkMsgHeader* msg);
    int fn_801F5A90();
    void fn_801F5B30();
    int fn_801F5B44(u32 type);
    u32 fn_801F5B94();
    void* fn_801F5BA0();
    UnkMsgHeader* fn_801F5BB8(u32 type, u32 flagA, u32 flagB, u32 mode, u32 value);

    /* 0x00 */ u32 m0;
    /* 0x04 */ u8 unk4[0xC];
    /* 0x10 */ u32 mUsed;
    /* 0x14 */ u8 unk14[0x4];
};

UnkMsgHeader* UnkMsgBuffer::fn_801F59F0() {
    UnkMsgHeader* msg = (UnkMsgHeader*)((u8*)this + 0x18);
    if ((u8*)msg - (u8*)this >= mUsed) {
        return 0;
    }
    return msg;
}

UnkMsgHeader* UnkMsgBuffer::fn_801F5A14(UnkMsgHeader* msg) {
    if (msg == 0) {
        return 0;
    }
    UnkMsgHeader* next = (UnkMsgHeader*)((u8*)msg + fn_801F5308(msg));
    if (msg->GetType() == 0x6F) {
        return 0;
    }
    if ((u8*)next - (u8*)this >= mUsed) {
        return 0;
    }
    return next;
}

int UnkMsgBuffer::fn_801F5A90() {
    for (UnkMsgHeader* msg = fn_801F59F0(); msg != 0; msg = fn_801F5A14(msg)) {
    }
    return 1;
}

void UnkMsgBuffer::fn_801F5B30() {
    mUsed = 0x18;
    m0 = 0;
}

int UnkMsgBuffer::fn_801F5B44(u32 type) {
    return mUsed + fn_801F5294(type) <= 0x400;
}

u32 UnkMsgBuffer::fn_801F5B94() {
    return 0x400 - mUsed;
}

void* UnkMsgBuffer::fn_801F5BA0() {
    void* rest = (u8*)this + mUsed;
    mUsed += fn_801F5B94();
    return rest;
}

UnkMsgHeader* UnkMsgBuffer::fn_801F5BB8(u32 type, u32 flagA, u32 flagB, u32 mode,
                                        u32 value) {
    u32 size = fn_801F5294(type);
    if (mUsed + size <= 0x400) {
        UnkMsgHeader* msg = (UnkMsgHeader*)((u8*)this + mUsed);
        msg->SetFlagA(flagA);
        msg->SetType(type);
        msg->SetFlagB(flagB);
        if (mode >= 3) {
            msg->SetMode(3);
            msg->SetSplitValue(mode);
            msg->SetSplitExtra(value);
        } else {
            msg->SetMode(mode);
            msg->SetValue(value);
        }
        mUsed += size;
        return msg;
    }
    return 0;
}

// Four-byte id that starts at -1 (real name unknown); the previous unit builds an
// array of them with its out-of-line copy of this constructor.
struct UnkId {
    UnkId() { m0 = -1; }

    /* 0x0 */ int m0;
};

// Classes built elsewhere (constructors and destructors at 0x80146EEC-0x801472A8);
// real names unknown.
class Unk80146EEC {
public:
    Unk80146EEC();
    ~Unk80146EEC();

    /* 0x00 */ u8 unk0[0xB8];
};

class Unk80146FB8 {
public:
    Unk80146FB8();
    ~Unk80146FB8();

    /* 0x00 */ u8 unk0[0x2C];
};

class Unk80147130 {
public:
    Unk80147130();
    ~Unk80147130();

    /* 0x00 */ u8 unk0[0x28];
};

// The global objects are registered with destructors emitted in this unit, so their
// types wrap the classes above (real names unknown).
struct UnkGlobal80146EEC : Unk80146EEC {};
struct UnkGlobal80146FB8 : Unk80146FB8 {};
struct UnkGlobal80147130 : Unk80147130 {};

UnkId lbl_805FB9F0;
UnkGlobal80146EEC lbl_804FB728;
UnkGlobal80146FB8 lbl_804FB7EC;
UnkGlobal80147130 lbl_804FB828;
