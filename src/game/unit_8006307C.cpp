// Game unit at 0x8006307C-0x800636A0 (real file name unknown): the Entity_Light class
// (RTTI) and helpers for the light named "attached" (color, range, register). The
// implicit destructor of Entity_Static is emitted here. Bounds come from the static
// initializers of this unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/Entity.h"
#include "game/unk_803E6D00.h"
#include "game/unk_vec3.h"

// Light (real type unknown).
struct UnkLight {
    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ void* mOwner;
    /* 0x10 */ s16 mType;
    /* 0x12 */ s16 mFlags;
    /* 0x14 */ u8 unk14[0x44 - 0x14];
    /* 0x44 */ f32 mColor[4];
    /* 0x54 */ UnkVec3 mAttenuation;
};

struct UnkColor {
    UnkColor() {}
    UnkColor(f32 r_, f32 g_, f32 b_, f32 a_) : r(r_), g(g_), b(b_), a(a_) {}

    f32 r, g, b, a;
};

// Zone of the level (real type unknown).
struct UnkZoneData {
    /* 0x00 */ u8 unk0[0x4C];
    /* 0x4C */ void* mLights;
};

// Object returned by Entity::vf0C (real type unknown).
struct UnkEntityState {
    /* 0x0 */ u8 unk0[0x8];
    /* 0x8 */ u32 mFlags;
};

extern "C" {
UnkLight* fn_80172F70(int type, const char* name);
void fn_80172E84(UnkLight* light);
void fn_80173164(UnkLight* light);
void fn_80184488(void* lights, UnkLight* light);
void* fn_801842C8(void* owner);
void fn_80184590(void* lights, UnkLight* light);
void fn_801DAA40(UnkZoneData* zone, void* obj);
void fn_801DAA74(UnkZoneData* zone, void* obj);
void fn_801D9B34(void* obj, f32 dt);
}

Entity_Light::Entity_Light(int type, EntityInfo* info, int init) : Entity_Static(type, info, 0) {
    SetType(10);
}

void Entity_Light::vf14(void* arg) {
    void* obj = m90;
    if (obj != 0 && !(mInfo->mFlags & 0x40)) {
        u16 index = mInfo->m30;
        if (index != 0xFFFF) {
            UnkZoneData* zone = lbl_803E6D00.m0->mZones[index];
            if (zone != 0) {
                fn_801DAA40(zone, obj);
            }
        } else {
            UnkZoneData* zone = lbl_803E6D00.m0->mZones[0];
            if (zone != 0) {
                fn_801DAA40(zone, obj);
            }
        }
    }
    mInfo->mFlags &= ~0x20500;
    if (vf0C() != 0) {
        ((UnkEntityState*)vf0C())->mFlags |= 2;
    }
    mInfo->mFlags |= 3;
}

void Entity_Light::vf18(void* arg) {
    if (mInfo != 0) {
        if (vf0C() != 0) {
            ((UnkEntityState*)vf0C())->mFlags &= ~2;
        }
        mInfo->mFlags &= ~0x20503;
        void* obj = m90;
        if (obj != 0 && !(mInfo->mFlags & 0x40)) {
            u16 index = mInfo->m30;
            if (index != 0xFFFF) {
                UnkZoneData* zone = lbl_803E6D00.m0->mZones[index];
                if (zone != 0) {
                    fn_801DAA74(zone, obj);
                }
            } else {
                UnkZoneData* zone = lbl_803E6D00.m0->mZones[0];
                if (zone != 0) {
                    fn_801DAA74(zone, obj);
                }
            }
        }
    }
}

void Entity_Light::vf20(f32 dt, int arg) {
    if (mInfo->mFlags & 1) {
        fn_801D9B34(m90, lbl_803E6D00.mDeltaTime);
        Entity_Static::vf20(dt, 0);
    }
}

extern "C" {

void fn_80063388(int enable) {
    UnkLight* light = fn_80172F70(1, "attached");
    if (enable == 0) {
        light->mFlags |= 8;
    }
}

// NonMatching (90 %): our build folds the type test into one range compare and
// reuses the first null test of light; the DOL tests each again.
UnkLight* fn_800633D4(UnkLight* light) {
    if (light == 0) {
        light = fn_80172F70(1, "attached");
    }
    if (light != 0) {
        UnkGlobals803E6D00Sub* sub = lbl_803E6D00.m0;
        Entity* player = sub->m8;
        if (player != 0 && player->mInfo != 0) {
            int index = player->mInfo->m30;
            if (index > 0 && index <= sub->m4) {
                UnkZoneData* zone = sub->mZones[index];
                if ((light->mType == 1 || light->mType == 2) && zone->mLights != 0 && light != 0) {
                    {
                        UnkColor color;
                        color.r = light->mColor[0];
                        color.g = light->mColor[1];
                        color.b = light->mColor[2];
                        color.a = 0.0f;
                        light->mColor[3] = color.a;
                        fn_80172E84(light);
                        fn_80184488(zone->mLights, light);
                    }
                }
            }
        }
    }
    return light;
}

void fn_800634D0(UnkLight* light) {
    if (light != 0 && fn_801842C8(light->mOwner) != 0) {
        fn_80184590(fn_801842C8(light->mOwner), light);
    }
}

void fn_80063520(UnkLight* light) {
    if (light != 0) {
        fn_80173164(light);
    }
}

void fn_80063530(UnkLight* light, const UnkColor* color) {
    if (light == 0) {
        light = fn_80172F70(1, "attached");
    }
    if (light != 0) {
        *(UnkColor*)light->mColor = *color;
        fn_80172E84(light);
    }
}

void fn_8006359C(UnkLight* light, f32 range) {
    if (light == 0) {
        light = fn_80172F70(1, "attached");
    }
    if (light != 0 && range > 0.0f) {
        f32 falloff = 5.0f / range;
        light->mAttenuation = UnkVec3(1.0f, 0.0f, falloff);
        fn_80172E84(light);
    }
}
}
