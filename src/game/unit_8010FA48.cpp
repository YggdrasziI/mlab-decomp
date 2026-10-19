// Game unit at 0x8010FA48-0x8010FD14 (real file name unknown): wrappers that start or
// stop objects through a lazily created global manager (lbl_805FBBAC). Bounds come
// from the static initializers of this unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/unk_vec3.h"

// Object handled by the manager (real type unknown).
struct UnkManagedObject {
    /* 0x00 */ void* m0;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 mFlags;
    /* 0x06 */ u8 unk6[0x2E - 0x6];
    /* 0x2E */ u8 m2E;
};

// Global manager created on first use (real name unknown; its functions are at
// 0x80209814-0x8020A59C, unit not split yet).
class UnkManager {
public:
    UnkManager();
    static int fn_80209ABC(UnkManagedObject* object, int arg1, int arg2, int arg3);

    static UnkManager* Instance() {
        if (sInstance == 0) {
            sInstance = new UnkManager;
        }
        return sInstance;
    }

    static UnkManager* sInstance;
};

extern "C" void fn_8020A474(void* obj);

extern "C" {

void fn_8010FA48(void) {}

int fn_8010FA4C(UnkManagedObject* object) {
    if (object != 0 && (object->mFlags & 1)) {
        return 0;
    }
    return UnkManager::Instance()->fn_80209ABC(object, 0, 0, 0);
}

int fn_8010FAC8(UnkManagedObject* object, u8 value) {
    if (object != 0) {
        if (object->mFlags & 1) {
            return 0;
        }
        object->m2E = value;
    }
    return UnkManager::Instance()->fn_80209ABC(object, 0, 0, 0);
}

void fn_8010FB48(UnkManagedObject* first, UnkManagedObject* second) {
    UnkManager::Instance()->fn_80209ABC(first, 0, 0, 6);
    UnkManager::Instance()->fn_80209ABC(second, 0, 0, 7);
}

void fn_8010FBEC(UnkManagedObject* first, UnkManagedObject* second, u8 value) {
    first->m2E = value;
    second->m2E = value;
    UnkManager::Instance()->fn_80209ABC(first, 0, 0, 6);
    UnkManager::Instance()->fn_80209ABC(second, 0, 0, 7);
}

void fn_8010FC98(UnkManagedObject* first, UnkManagedObject* second) {
    if (first != 0) {
        fn_8020A474(first->m0);
    }
    if (second != 0) {
        fn_8020A474(second->m0);
    }
}

void fn_8010FCE0(UnkManagedObject* object) {
    if (object != 0) {
        fn_8020A474(object->m0);
    }
}

}
