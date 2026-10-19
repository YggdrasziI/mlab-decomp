// Game unit at 0x801068A8-0x80106AA8 (real file name unknown): five predicates on
// an id, registered through fn_801DC574-fn_801DC594. Bounds come from the static
// initializers of this unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/unk_vec3.h"

// Registration functions (real names unknown).
extern "C" {
typedef u32 (*UnkIdPredicate)(int id);
void fn_801DC574(UnkIdPredicate func);
void fn_801DC57C(UnkIdPredicate func);
void fn_801DC584(UnkIdPredicate func);
void fn_801DC58C(UnkIdPredicate func);
void fn_801DC594(UnkIdPredicate func);

u32 fn_80106900(int id);
u32 fn_80106920(int id);
u32 fn_80106988(int id);
u32 fn_801069B0(int id);
u32 fn_80106A2C(int id);

void fn_801068A8(void) {
    fn_801DC574(fn_80106900);
    fn_801DC58C(fn_80106920);
    fn_801DC57C(fn_80106988);
    fn_801DC584(fn_801069B0);
    fn_801DC594(fn_80106A2C);
}

u32 fn_80106900(int id) {
    return (u32)id >= 43 && (u32)id < 92;
}

u32 fn_80106920(int id) {
    u32 result = false;
    switch (id) {
    case 15:
    case 46:
    case 47:
    case 49:
    case 50:
    case 52:
    case 62:
    case 85:
    case 86:
    case 87:
    case 88:
    case 89:
    case 90:
        result = true;
        break;
    case 48:
    case 51:
        break;
    }
    return result;
}

u32 fn_80106988(int id) {
    u32 inRange = (u32)id >= 43 && (u32)id < 92;
    return inRange != 0;
}

u32 fn_801069B0(int id) {
    u32 result = 0;
    switch (id) {
    case 46:
    case 47:
    case 49:
    case 50:
    case 52:
    case 62:
    case 85:
    case 86:
    case 87:
    case 88:
    case 89:
    case 90:
        result = 0xC87B6F93;
        break;
    case 48:
        result = 0;
        break;
    case 43:
        result = 0;
        break;
    case 51:
        break;
    }
    return result;
}

u32 fn_80106A2C(int id) {
    u32 result = false;
    switch (id) {
    case 43:
    case 51:
    case 53:
    case 54:
    case 55:
    case 63:
    case 64:
    case 65:
    case 66:
    case 67:
    case 68:
    case 69:
    case 70:
    case 71:
    case 72:
    case 73:
    case 74:
    case 75:
    case 76:
    case 77:
    case 78:
    case 79:
    case 80:
    case 81:
    case 82:
    case 83:
    case 84:
    case 91:
        result = true;
        break;
    }
    return result;
}

} // extern "C"
