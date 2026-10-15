// Game unit at 0x8006F6BC-0x8006F794 (real file name unknown): the
// Entity_Trigger_Script class. Bounds come from the static initializers of this unit
// and the previous one.

#include "game/Entity.h"
#include "game/unk_vec3.h"

Entity_Trigger_Script::Entity_Trigger_Script(int type, EntityInfo* info, int init)
    : Entity_Sensor(type, info, init) {
    SetType(9);
}

Entity_Trigger_Script::~Entity_Trigger_Script() {}
