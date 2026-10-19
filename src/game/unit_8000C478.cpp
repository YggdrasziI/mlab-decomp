// Game unit at 0x8000C478-0x8000C7C8 (real file name unknown). Bounds: the previous
// unit's static initializer (0x8000C380) is followed by the destructor it registers
// (0x8000C41C); this unit's static initializer ends at 0x8000C7C8.

#include "game/unk_vec3.mch"
#include "game/Entity.h"
#include "game/unk_803E6D00.h"
#include "game/unk_vec3.h"

// Resource whose first word is the magic 0x37645F52 (real type unknown).
struct UnkResource {
    /* 0x0 */ u32 mMagic;
};

// Object taken from a pool by type hash (real type unknown).
struct UnkPooled {
    /* 0x000 */ u8 unk0[0x8];
    /* 0x008 */ UnkVec3 mScale;
    /* 0x014 */ u8 unk14[0x9C - 0x14];
    /* 0x09C */ UnkResource* m9C;
    /* 0x0A0 */ u8 unkA0[0xB8 - 0xA0];
    /* 0x0B8 */ u32 mFlags;
    /* 0x0BC */ u8 unkBC[0x228 - 0xBC];
    /* 0x228 */ UnkResource* m228;
    /* 0x22C */ u8 unk22C[0x248 - 0x22C];
    /* 0x248 */ void* m248;
};

// Owner passed to the pooled object (real type unknown).
struct UnkOwner {
    /* 0x00 */ u8 unk0[0xB8];
    /* 0xB8 */ f32 mB8;
};

// Entity with a velocity at 0x770 (real type unknown).
class UnkMovingEntity : public Entity {
public:
    /* 0x14C */ u8 unk14C[0x770 - 0x14C];
    /* 0x770 */ UnkVec3 mVelocity;
};

extern "C" UnkPooled* fn_801DB32C(u8* pool, u32 hash);
extern "C" int fn_80065200(UnkPooled* obj, UnkOwner* owner, void* arg, int a, int b,
                           int c, int d);

UnkPooled* fn_8000C478(UnkResource* res, void* arg) {
    if (res != 0 && res->mMagic == 0x37645F52) {
        UnkPooled* obj = fn_801DB32C(lbl_803E6D00.m0->mWireGroups + 0xE4, 0xBE8520AB);
        if (obj != 0) {
            obj->mFlags &= ~2;
            obj->m228 = res;
            obj->m9C = res;
            obj->m248 = arg;
        }
        return obj;
    }
    return 0;
}

UnkPooled* fn_8000C508(UnkOwner* owner, UnkResource* res, void* arg) {
    UnkPooled* obj = fn_8000C478(res, 0);
    if (obj != 0) {
        UnkVec3 scale;
        scale.x = scale.y = scale.z = 1.0f;
        obj->mScale = scale;
        if (fn_80065200(obj, owner, arg, 0, 0, 0, 0) != 0) {
            return obj;
        }
    }
    return 0;
}

// Larger of two values, returned as a double: the DOL rounds the result back to a
// float (real name unknown).
inline double UnkMaxD(f32 a, f32 b) {
    return a > b ? a : b;
}

// NonMatching (99.8 %): the DOL rounds the larger time step into f0 and loads 1.0f
// into f1; this version rounds it in place (frsp f1,f1) and loads 1.0f into f0.
void fn_8000C604(UnkOwner* owner, Entity* target, UnkMovingEntity* source, UnkVec3* out,
                 f32 t) {
    if (target == 0 || source == 0) {
        return;
    }
    *out = *source->vf44(&target->m18, target);
    UnkVec3 diff = *out - target->m18;
    if (diff.LengthSq() < 4.0f) {
        return;
    }
    if (!(source->mInfo->mTypeMask & 0x10)) {
        return;
    }
    f32 dt = UnkMaxD(source->m24, 0.032f);
    UnkVec3 dir = source->mVelocity * (1.0f / dt);
    if (owner->mB8) {
        UnkVec3 step = dir * (10.0f * t / owner->mB8);
        *out += step;
    }
}

// Emitted here in the DOL (0x8000C7A0), probably as an inline function.
UnkVec3* Entity::vf44(const UnkVec3* from, Entity* fromEntity) {
    return &m18;
}
