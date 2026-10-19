// Game unit at 0x800E2EBC-0x800E2FE8 (real file name unknown): registration of the
// CIWaypointPath static setters, and a task dispatch function. Bounds come from the
// static initializers of this unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/CameraInfluence.h"
#include "game/unk_vec3.h"

// Registers a function under a name and a type id; returns a handle (real name and
// signature unknown).
extern "C" void* fn_801DC91C(const char* name, int type, void* func);

// Object whose byte at 0x7F selects the handler (real type unknown).
struct UnkTaskNode {
    /* 0x00 */ u8 unk0[0x7F];
    /* 0x7F */ u8 mKind;
};

extern "C" void fn_801E7E28(UnkTaskNode* node);
extern "C" void fn_801E839C(UnkTaskNode* node);
extern "C" void fn_801E97FC(UnkTaskNode* node);

void* lbl_805FB1B0;
void* lbl_805FB1B4;

void fn_800E2EBC() {
    lbl_805FB1B0 = fn_801DC91C("CIWaypointPath::StaticSetPosition", 0x90, (void*)&CIWaypointPath::StaticSetPosition);
    lbl_805FB1B4 =
        fn_801DC91C("CIWaypointPath::StaticSetOrientation", 0x8A, (void*)&CIWaypointPath::StaticSetOrientation);
}

int fn_800E2F10(UnkTaskNode* node) {
    switch (node->mKind) {
    case 0x20:
        fn_801E97FC(node);
        return 2;
    case 0x40:
        fn_801E839C(node);
        return 3;
    case 0x08:
        fn_801E7E28(node);
    case 0x10:
        return 2;
    }
    return 0;
}

void CIWaypointPath::StaticSetPosition(const UnkVec3* value, CIWaypointPath* path) {
    path->mPosition = *value;
}

void CIWaypointPath::StaticSetOrientation(const UnkQuat* value, CIWaypointPath* path) {
    path->mOrientation = *value;
}
