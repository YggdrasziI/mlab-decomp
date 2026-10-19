// Game unit at 0x800232A4-0x80023884 (real file name unknown): queries and actions on
// the object returned by fn_80021750 (state checks, counters, and events chosen by
// its mode at 0xE8). Bounds come from the static initializers of this unit and the
// previous one.

#include "game/unk_vec3.mch"
#include "game/unk_vec3.h"

// Object returned by fn_80021750 (real type unknown).
struct UnkGameMode {
    /* 0x00 */ u8 unk0[0xBC];
    /* 0xBC */ u8 mBC;
    /* 0xC0 */ int mC0;
    /* 0xC4 */ u8 unkC4[0xE8 - 0xC4];
    /* 0xE8 */ int mMode;
    /* 0xEC */ int mEC;
    /* 0xF0 */ u8 unkF0[0xF4 - 0xF0];
    /* 0xF4 */ int mF4;
    /* 0xF8 */ u8 unkF8[0xFC - 0xF8];
    /* 0xFC */ u8 mFC;
};

// Object whose vtable pointer is at 0x138 (real type unknown).
struct UnkWidgetBase {
    u8 unk0[0x138];
};

class UnkWidget : public UnkWidgetBase {
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
    virtual void vf2C(int arg);
    virtual void vf30(int arg);
};

struct UnkWidgetSet {
    /* 0x0 */ u32 mId;
};

extern void* lbl_805FB654;

extern "C" {
UnkGameMode* fn_80021750(void);
UnkWidgetSet* fn_8018D1E4(void* obj);
UnkWidget* fn_80198F18(UnkWidgetSet* set, u32 hash);
void fn_801903CC(u32 group, u32 hash);
u32 fn_801EB704(const char* name);
void fn_80024D98(UnkGameMode* mode);
void fn_80024E90(UnkGameMode* mode);
void fn_80024EB0(UnkGameMode* mode);
void fn_80024EDC(UnkGameMode* mode);
}

extern "C" {

int fn_800232A4(void) {
    return fn_80021750()->mBC == 0;
}

int fn_800232D0(void) {
    return fn_80021750()->mBC == 1;
}

int fn_80023300(void) {
    UnkGameMode* mode = fn_80021750();
    if (mode->mBC == 1 && mode->mC0 > 0) {
        return 1;
    }
    return 0;
}

int fn_80023344(void) {
    return fn_80021750()->mBC == 0;
}

int fn_80023370(void) {
    return fn_80021750()->mFC == 1;
}

void fn_800233A0(void) {
    UnkGameMode* mode = fn_80021750();
    mode->mEC--;
    mode->mF4--;
}

void fn_800233D8(void) {
    UnkGameMode* mode = fn_80021750();
    mode->mC0 = mode->mC0 <= 0 ? 0 : mode->mC0 - 1;
}

void fn_80023414(void) {
    UnkGameMode* mode = fn_80021750();
    UnkWidgetSet* set = fn_8018D1E4(lbl_805FB654);
    if (set->mId == 0x9A5E553B) {
        UnkWidget* first = fn_80198F18(set, 0xDE8244C0);
        UnkWidget* second = fn_80198F18(set, 0x478B157A);
        switch (mode->mMode) {
        case 4:
            break;
        case 3:
            first->vf30(2);
            break;
        case 2:
            second->vf30(2);
            break;
        case 1:
            first->vf30(2);
            break;
        }
    }
}

void fn_800234FC(void) {
    switch (fn_80021750()->mMode) {
    case 4:
        fn_801903CC(0x0A7964E0, 0xA8E25FA8);
        break;
    case 3:
        fn_801903CC(0x0A7964E0, 0xA8E25FA8);
        break;
    case 2:
        fn_801903CC(0x0A7964E0, 0xA8E25FA8);
        break;
    case 1:
        fn_801903CC(0x0A7964E0, 0x2B13D2BD);
        break;
    }
}

void fn_800235A0(void) {
    switch (fn_80021750()->mMode) {
    case 4:
        fn_801903CC(0x0A7964E0, 0x4DE8B1EA);
        break;
    case 3:
        fn_801903CC(0x0A7964E0, 0x4DE8B1EA);
        break;
    case 2:
        fn_801903CC(0x0A7964E0, 0x4DE8B1EA);
        break;
    case 1:
        fn_801903CC(0x0A7964E0, 0xB324F8EF);
        break;
    }
}

void fn_80023644(void) {
    switch (fn_80021750()->mMode) {
    case 4:
        break;
    case 3:
        fn_801903CC(0x0A7964E0, 0x06977454);
        break;
    case 2:
        fn_801903CC(0x0A7964E0, 0x06977454);
        break;
    case 1:
        fn_801903CC(0x0A7964E0, 0x9FB619A1);
        break;
    }
}

void fn_800236CC(void) {
    switch (fn_80021750()->mMode) {
    case 4:
        break;
    case 3:
        fn_801903CC(0x0A7964E0, 0x7AC453D4);
        break;
    case 2:
        fn_801903CC(0x0A7964E0, 0x7AC453D4);
        break;
    case 1:
        fn_801903CC(0x0A7964E0, 0x7272C9A6);
        break;
    }
}

void fn_80023754(void) {
    UnkGameMode* mode = fn_80021750();
    UnkWidgetSet* set = fn_8018D1E4(lbl_805FB654);
    fn_80198F18(set, fn_801EB704("dirt_fling"))->vf2C(2);
    fn_80024EDC(mode);
    fn_801903CC(fn_801EB704("dirt_fling"), fn_801EB704("dirt_fling_anim"));
}

UnkGameMode* fn_800237F0(void) {
    return fn_80021750();
}

void fn_800237F4(void) {}

void fn_800237F8(void) {
    fn_80024D98(fn_80021750());
}

void fn_8002381C(void) {
    fn_80024E90(fn_80021750());
}

void fn_80023840(void) {
    fn_80024EB0(fn_80021750());
}

}
