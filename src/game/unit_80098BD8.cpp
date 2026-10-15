// Game unit at 0x80098BD8-0x80098DA8 (real file name unknown): the
// Entity_ReactorLeakHammer class. Bounds come from the static initializers of this
// unit and the previous one; the two small functions at 0x80098BC0-0x80098BD8 after
// the previous static initializer are left to the previous unit (probably its
// inline functions).

#include "game/Entity.h"
#include "game/unk_vec3.h"

Entity_ReactorLeakHammer::Entity_ReactorLeakHammer(int type, EntityInfo* info, int init)
    : Entity_ReactorLeakManipulator(type, info, init), m264(0), mRecognizer(lbl_803C4298.m38) {}

Entity_ReactorLeakHammer::~Entity_ReactorLeakHammer() {}

void Entity_ReactorLeakHammer::vfBC() {
    mRecognizer.fn_801F8F34(0.3f);
    mRecognizer.fn_801F8FA0(0.4f);
    UnkMotionParam param(0, -1, 0);
    mRecognizer.fn_801F8628(&param, 1);
    mRecognizer.m98 = 0;
    mRecognizer.m94 = 0.25f;
    fn_801B3308()->fn_801B3310(&mRecognizer);
}

void Entity_ReactorLeakHammer::vfC0() {
    fn_801B3308()->fn_801B3378(&mRecognizer);
}

u8 Entity_ReactorLeakHammer::vfC4() {
    return mRecognizer.Update();
}
