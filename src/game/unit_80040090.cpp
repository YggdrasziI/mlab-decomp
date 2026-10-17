// Game unit at 0x80040090-0x80040638 (real file name unknown): the
// PlayerInputActionSelectionCallback class (name from RTTI). Bounds come from the
// static initializers of this unit and the previous one.

#include "game/ActionSelectionCallback.h"
#include "game/unk_803C4298.h"
#include "game/unk_vec3.h"
#include <math.h>

// Global manager created on first use (see unit_8010FA48.cpp).
class UnkManager {
public:
    UnkManager();
    static int fn_80209E70(u32 hash, int arg1, int arg2, int arg3, int arg4, int arg5);

    static UnkManager* Instance() {
        if (sInstance == 0) {
            sInstance = new UnkManager;
        }
        return sInstance;
    }

    static UnkManager* sInstance;
};

// Object at lbl_805FB964 (real type unknown).
class UnkInputSource {
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
    virtual int vf34();
};

// Object at lbl_805FBBB8 (real type unknown).
struct UnkGameState {
    /* 0x000 */ u8 unk0[0x9FE];
    /* 0x9FE */ u8 m9FE;
};

extern "C" {
extern u8 lbl_805FB0E0;
extern void* lbl_805FB654;
extern void* lbl_805FB970;
extern void* lbl_805FB974;
extern UnkInputSource* lbl_805FB964;
extern UnkGameState* lbl_805FBBB8;

int fn_8004935C(void);
int fn_80049364(void);
int fn_8004533C(int mode);
void fn_80195104(u32 hash);
void fn_80046BA4(void);
int fn_8018DEE0(void* obj);
void fn_8018D70C(void* obj, u32 hash);
int fn_80046F54(void);
void fn_80046E00(void);
UnkActionSelection fn_80046F30(void);
void fn_80147704(int channel, f32* x, f32* y);
void fn_8004012C(u32 mask);
}

PlayerInputActionSelectionCallback::PlayerInputActionSelectionCallback(void* arg)
    : XBaseActionSelectionCallback(arg) {
    m4C = 0.0f;
}

PlayerInputActionSelectionCallback::~PlayerInputActionSelectionCallback() {}

extern "C" void fn_8004012C(u32 mask) {
    if ((mask & 1) && fn_8004935C() != 0) {
        if (fn_8004533C(0)) {
            UnkManager::Instance()->fn_80209E70(0x46712111, 0, 0, 0, 0, 0);
            fn_80195104(0xD521D574);
            fn_80195104(0x91661B28);
        }
    } else if ((mask & 8) && fn_8004935C() != 3) {
        if (fn_8004533C(3)) {
            UnkManager::Instance()->fn_80209E70(0x46712111, 0, 0, 0, 0, 0);
            fn_80195104(0xD521D574);
            fn_80195104(0x6CBB4FDD);
        }
    } else if ((mask & 4) && fn_8004935C() != 2) {
        if (fn_8004533C(2)) {
            UnkManager::Instance()->fn_80209E70(0x46712111, 0, 0, 0, 0, 0);
            fn_80195104(0xD521D574);
            fn_80195104(0x23368BBC);
        }
    } else if ((mask & 2) && fn_8004935C() != 1) {
        if (fn_8004533C(1)) {
            UnkManager::Instance()->fn_80209E70(0x46712111, 0, 0, 0, 0, 0);
            fn_80195104(0xD521D574);
            fn_80195104(0xCB8DAE3E);
        }
    } else if (mask == 0 && fn_8004935C() != 4) {
        if (fn_8004533C(4)) {
            UnkManager::Instance()->fn_80209E70(0x46712111, 0, 0, 0, 0, 0);
            fn_80195104(0xD521D574);
            fn_80195104(0xD00B7E35);
        }
    }
}

void PlayerInputActionSelectionCallback::vf0C() {
    m4C = 0.0f;
    if (lbl_805FB0E0) {
        return;
    }
    if (fn_80049364() == 0) {
        fn_80046BA4();
        return;
    }
    fn_8004012C(fn_80040550());
    if (fn_8018DEE0(lbl_805FB654) == 0) {
        fn_8018D70C(lbl_805FB654, 0x7A1060A7);
    }
    if (fn_80046F54()) {
        m4C = 0.0f;
        fn_80046E00();
        Select(fn_80046F30());
        return;
    }
    bool ready = false;
    if (lbl_805FB970 && lbl_805FB974 && lbl_805FB964 && lbl_805FB964->vf34()) {
        ready = true;
    }
    if (ready && m4C > 10.0f) {
        lbl_805FBBB8->m9FE = 1;
        UnkActionSelection selection;
        selection.Set(0x4E, 0, 4);
        m4C = 0.0f;
        fn_80046E00();
        fn_8020F46C(&selection);
    }
}

// NONMATCHING: 99.2 %; the DOL puts fabs(y) in f3 and fabs(x) in f0, this build the
// other way round (operand order, locals, fabs/__fabs and inline helpers tried).
u32 PlayerInputActionSelectionCallback::fn_80040550() {
    f32 x = 0.0f;
    f32 y = 0.0f;
    u32 mask = 0;
    fn_80147704((int)lbl_803C4298.m38 * 16 + 14, &x, &y);
    if (fabsf(x) >= fabsf(y)) {
        if (x < -0.1f) {
            mask |= 4;
        } else if (x > 0.1f) {
            mask |= 8;
        }
    } else {
        if (y < -0.1f) {
            mask |= 2;
        } else if (y > 0.1f) {
            mask |= 1;
        }
    }
    return mask;
}
