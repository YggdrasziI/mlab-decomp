// Game unit at 0x8005230C-0x800523B8 (real file name unknown): releases or keeps the
// object at lbl_803C2A20 + 0xDC8. Bounds come from the static initializers of this
// unit and the previous one.

#include "game/unk_vec3.h"

// Object with a virtual destructor (real type unknown).
class UnkDC8Object {
public:
    virtual ~UnkDC8Object();
};

// Global block at 0x803C2A20 (0x15E0 bytes, real type unknown).
struct UnkGlobals803C2A20 {
    /* 0x000 */ u8 unk0[0xDC8];
    /* 0xDC8 */ UnkDC8Object* mDC8;
};

extern UnkGlobals803C2A20 lbl_803C2A20;
extern void* lbl_805FBBC0;

// Allocator free function (real name unknown).
extern "C" void fn_801D4980(void* ptr);

static u8 lbl_805FB100;

extern "C" {

void fn_8005230C(void) {
    UnkDC8Object* object = lbl_803C2A20.mDC8;
    if (object != 0) {
        if (!lbl_805FB100) {
            lbl_805FB100 = 1;
        } else {
            object->~UnkDC8Object();
            fn_801D4980(lbl_805FBBC0);
        }
    }
    lbl_805FBBC0 = 0;
    lbl_803C2A20.mDC8 = 0;
}

void fn_80052384(void) {
    lbl_805FBBC0 = lbl_803C2A20.mDC8;
}

} // extern "C"
