// Game unit at 0x800747DC-0x80074CC8 (real file name unknown): global helpers called
// by script code (a hash to slot table, sending the hashes of the set flags, and
// forwarders to the game state object returned by fn_800701A4). Bounds come from the
// static initializers of this unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/unk_vec3.h"
#include "types.h"

// Object returned by fn_800701A4 (real type unknown).
struct UnkGameFlags {
    /* 0x0 */ u32 m0;
    /* 0x4 */ void* m4;
    /* 0x8 */ int m8;
    /* 0xC */ void* mC;
};

// Object returned by fn_8006FBDC; its vtable pointer is at 0x4 (real type unknown).
struct UnkGameModeBase {
    /* 0x0 */ u32 m0;
};

class UnkGameMode : public UnkGameModeBase {
public:
    virtual void vf08();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C();
    virtual void vf30();
    virtual void vf34();
    virtual void vf38();
    virtual void vf3C();
    virtual void vf40();
    virtual void vf44();
    virtual void vf48();
    virtual void vf4C();
    virtual void vf50();
    virtual void vf54();
    virtual void vf58();
    virtual void vf5C();
};

extern "C" {
extern void* lbl_805FBBC8;

int fn_802188A8(void* obj, int slot);
void fn_8000CC80(int arg, int id);
UnkGameFlags* fn_800701A4(void);
int fn_8006FA5C(UnkGameFlags* flags, int flag);
u32 fn_80070270(int flag);
void fn_8018EEB4(u32 hash);
void fn_80107244(void* obj);
void fn_8000FBA0(void* obj, f32 value);
void fn_8007043C(void);
UnkGameMode* fn_8006FBDC(void);
void fn_80070448(void);
void fn_80070450(void);

int fn_800747DC(void) {
    return 1;
}

int fn_800747E4(u32 value) {
    return value != 0;
}

int fn_800747F4(int hash) {
    int slot = -1;
    switch (hash) {
    case 0x5AE4B3A6:
        slot = 4;
        break;
    case 0x00F7B53F:
        slot = 5;
        break;
    case 0x5AADD230:
        slot = 1;
        break;
    case 0xA3D4805A:
        slot = 0;
        break;
    case 0x7A150909:
        slot = 3;
        break;
    case 0x0DA38580:
        slot = 2;
        break;
    case 0x011581D3:
        slot = 6;
        break;
    case 0xC9ABAC90:
        slot = 6;
        break;
    case 0x053BCBA4:
        slot = 11;
        break;
    case 0xB7AEEBD1:
        slot = 7;
        break;
    case 0x5B291F8C:
        slot = 9;
        break;
    case 0x8BD3CCCE:
        slot = 10;
        break;
    case 0xB4F82008:
        slot = 8;
        break;
    case 0x9A1B7746:
        slot = -1;
        break;
    }
    return fn_802188A8(lbl_805FBBC8, slot);
}

void fn_80074984(int arg) {
    fn_8000CC80(arg, 0xF611F736);
}

static inline void SendFlag(int flag) {
    if (fn_8006FA5C(fn_800701A4(), flag)) {
        fn_8018EEB4(fn_80070270(flag));
    }
}

void fn_80074990(void) {
    u32 hash = fn_800701A4()->m0;
    if (hash != 0) {
        fn_8018EEB4(hash);
    }
    SendFlag(0x1);
    SendFlag(0x2);
    SendFlag(0x4000);
    SendFlag(0x4);
    SendFlag(0x8);
    SendFlag(0x10);
    SendFlag(0x20);
    SendFlag(0x40);
    SendFlag(0x80);
    SendFlag(0x100);
    SendFlag(0x200);
    SendFlag(0x400);
    SendFlag(0x800);
    SendFlag(0x1000);
    SendFlag(0x2000);
    SendFlag(0x8000);
    fn_8018EEB4(0xC62FBC4E);
}

void fn_80074BD8(void) {
    if (fn_800701A4()->m8 != 0) {
        fn_8018EEB4(0x6825C6AE);
    }
}

void fn_80074C10(void) {
    fn_80107244(fn_800701A4()->m4);
}

void fn_80074C38(void) {
    void* obj = fn_800701A4()->mC;
    if (obj != 0) {
        fn_8000FBA0(obj, 1.0f);
    }
}

void fn_80074C6C(void) {
    fn_8007043C();
    fn_8006FBDC()->vf5C();
}

void fn_80074CA0(void) {
    fn_80070448();
}

void fn_80074CA4(void) {
    fn_80070450();
}
}
