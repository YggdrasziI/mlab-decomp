// Game unit at 0x8006F5A0-0x8006F61C (real file name unknown): the "count_deaths"
// condition. Bounds come from the static initializers of this unit and the previous
// one.

#include "game/unk_vec3.mch"
#include "game/unk_vec3.h"

// Registers a named condition callback (real name and signature unknown).
typedef int (*UnkConditionFunc)(void* obj);
extern "C" void fn_801DC91C(const char* name, int type, UnkConditionFunc func);

// Real name and signature unknown.
extern "C" void fn_8006D3C8(void* obj, int arg);

// Layouts seen from fn_8006F5B8 only; real types unknown.
struct UnkCountDeathsInfo {
    /* 0x00 */ u8 unk0[0x1C];
    /* 0x1C */ u32 mFlags;
};

struct UnkCountDeathsObj {
    /* 0x000 */ u8 unk0[0x4];
    /* 0x004 */ UnkCountDeathsInfo* mInfo;
    /* 0x008 */ u8 unk8[0x158 - 0x8];
    /* 0x158 */ f32 m158;
    /* 0x15C */ u8 unk15C[0x164 - 0x15C];
    /* 0x164 */ f32 m164;
};

int fn_8006F5B8(void* obj);

void fn_8006F5A0() {
    fn_801DC91C("count_deaths", 22, fn_8006F5B8);
}

int fn_8006F5B8(void* obj) {
    UnkCountDeathsObj* o = (UnkCountDeathsObj*)obj;
    if (o == 0 || !(o->mInfo->mFlags & 0x100)) {
        return 0;
    }
    return o->m164 >= o->m158;
}

void fn_8006F5F4(void* obj) {
    fn_8006D3C8(obj, 0);
}
