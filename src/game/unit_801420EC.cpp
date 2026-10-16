// Game unit at 0x801420EC-0x801424B8 (real file name unknown): online error
// handling. Polls the network object for an error type and code, shows a message
// chosen from the code ranges (20100-20999, 23000-23999, 29000, 29001, 31000-31999,
// 50000-59999, 80430) and reports the state to the callers. Bounds come from the
// static initializers of this unit and the previous one.

#include "game/unk_vec3.h"
#include "types.h"

// math.h maps abs to the __abs intrinsic; this unit calls the MSL function.
#undef abs

// Network object at lbl_805FB964 (real type unknown).
class UnkNetwork {
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
    // Gets the last error type and code; returns whether there is an error.
    virtual u8 vf58(int* type, int* code);
};

extern "C" {
int abs(int x);
extern UnkNetwork* lbl_805FB964;
extern f32 lbl_805FB918;
extern int lbl_805FB038;
extern void* lbl_805FB654;

int fn_801F38F0(void);
int fn_801F56FC(int arg);
int fn_801F57E0(f32 arg);
void fn_801F57B8(void);
void fn_8013BFA8(void);
void fn_8013BD7C(void);
void fn_80140EF8(u32 hash);
void fn_8018F000(u32 hash);
void* fn_8018D1E4(void* obj);
void* fn_80198E34(void* set, u32 hash);
void* fn_80198C34(void* set, void* item);
int fn_8013E464(void);
void fn_801F5724(int arg);
u32 fn_80140CE0(void);

int lbl_805FB440;
u8 lbl_805FB444;
int lbl_805FB448;
int lbl_805FB44C;
int lbl_805FB450;
u8 lbl_805FB454;

int fn_80142358(void);

void fn_801420EC(void) {
    lbl_805FB440 = 0;
    lbl_805FB444 = 0;
    lbl_805FB448 = 0;
    lbl_805FB44C = 0;
    lbl_805FB450 = 0;
    lbl_805FB454 = 0;
}

void fn_8014210C(void) {
    if (lbl_805FB964 == 0) {
        return;
    }
    if (!fn_80142358()) {
        return;
    }
    if (fn_801F38F0() != 0) {
        return;
    }
    switch (fn_801F56FC(2)) {
    case 0:
        break;
    default:
        return;
    }
    if (fn_801F57E0(lbl_805FB918) == 0) {
        fn_801F57B8();
        fn_8013BFA8();
    }
    lbl_805FB444 = lbl_805FB964->vf58(&lbl_805FB440, &lbl_805FB448);
    if (lbl_805FB444) {
        switch (lbl_805FB440) {
        case 3:
        case 4:
        case 5:
        case 6:
            fn_8013BD7C();
            break;
        }
    }
    if (lbl_805FB444 == 1 && lbl_805FB44C == 0) {
        lbl_805FB44C = 1;
        int code = abs(lbl_805FB448);
        if ((code >= 20102 && code <= 20109) || (code >= 20111 && code <= 20999)) {
            fn_80140EF8(0x8554F56C);
        } else if (code == 20101 || (code >= 23000 && code <= 23999)) {
            fn_80140EF8(0x0FDD8F3A);
        } else if (code == 20110) {
            fn_80140EF8(0x8F2E6505);
        } else if (code == 29000) {
            fn_80140EF8(0x0D84AA48);
        } else if (code == 29001) {
            fn_80140EF8(0x340550DD);
        } else if (code == 20100 || (code >= 50000 && code <= 59999)) {
            fn_80140EF8(0xEB4BCABF);
        } else if (code == 80430) {
            fn_80140EF8(0xD48D7CA1);
        } else if (lbl_805FB440 == 7) {
            fn_80140EF8(0xBD65120A);
        } else if ((u32)(lbl_805FB440 - 3) <= 3) {
            fn_80140EF8(0xC0189F1E);
        } else if (code < 31000 || code > 31999) {
            fn_80140EF8(0x5A9872D0);
        } else {
            fn_80140EF8(0x5639D127);
        }
    }
    if (lbl_805FB454 == 1) {
        lbl_805FB454 = 0;
        fn_8018F000(0x28DCC9E9);
    }
}

int fn_80142358(void) {
    int result = 0;
    void* item = 0;
    int found = 0;
    void* set = fn_8018D1E4(lbl_805FB654);
    if (set != 0) {
        void* a = fn_80198E34(set, 0x1B6732BB);
        void* b = fn_80198E34(set, 0x28DCC9E9);
        if (a != 0) {
            item = fn_80198C34(set, a);
        }
        if (b != 0) {
            found = 1;
        }
    }
    if (lbl_805FB038 <= 0 && item == 0 && found && fn_8013E464() == 0) {
        result = 1;
    }
    return result;
}

void fn_8014241C(void) {
    lbl_805FB454 = 1;
    fn_801F5724(0x140);
}

int fn_8014242C(void) {
    return fn_80140CE0() == 0x94B66F2D;
}

int fn_80142460(void) {
    return lbl_805FB448;
}

u8 fn_80142468(void) {
    return lbl_805FB444;
}

void fn_80142470(void) {
    lbl_805FB440 = 0;
    lbl_805FB444 = 0;
    lbl_805FB448 = 0;
    lbl_805FB44C = 0;
    lbl_805FB450 = 0;
    lbl_805FB454 = 0;
}

int fn_80142490(void) {
    return lbl_805FB44C;
}
}
