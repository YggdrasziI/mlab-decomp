// Game unit at 0x8012F6B0-0x8013022C (real file name unknown). Bounds: the previous
// unit's static initializer ends at 0x8012F6B0 and this unit's static initializer
// ends at 0x8013022C. The unit rebuilds the virtual tables of objects read from
// level data, with placement new: entities by type, their collision primitives and
// the camera influences of a zone.

#include "game/unk_vec3.mch"
#include "game/CameraInfluence.h"
#include "game/CollisionPrimitive.h"
#include "game/Entity.h"
#include "game/externs.h"
#include "game/unk_803B53A0.h"
#include "game/unk_803C4298.h"
#include "game/unk_803E6D00.h"
#include "game/unk_vec3.h"


#include <stddef.h>

inline void* operator new(size_t, void* p) { return p; }

struct UnkStateInfo;
extern "C" void fn_801BC574(UnkStateInfo* info);

// Group of collision primitives (real type unknown).
struct UnkCollisionGroup {
    CollisionPrimitive* Get(u32 i) { return i >= mCount ? 0 : mPrimitives[i]; }

    /* 0x00 */ u8 unk0[0x24];
    /* 0x24 */ CollisionPrimitive** mPrimitives;
    /* 0x28 */ u32 mCount;
};

// Object at Entity::m90 (real type unknown); m4 selects its constructor.
struct UnkEntity90 {
    /* 0x0 */ u8 unk0[0x4];
    /* 0x4 */ int m4;
};

// Objects at the start of Entity::mA4 (real types unknown).
struct UnkEntityA4State {
    /* 0x0 */ UnkStateInfo* mInfo;
};
struct UnkEntityA4Camera {
    /* 0x00 */ u8 unk0[0x18];
    /* 0x18 */ void* m18;
};

static inline void ReleaseStateInfo(UnkStateInfo* info) {
    if (info != 0) {
        fn_801BC574(info);
    }
}

static inline void SetFlag80(Entity* entity) {
    entity->mInfo->mFlags |= 0x80;
}

// Flags of an entity of type 8 (Entity_Sensor; real layout unknown).
struct UnkSensorFlags {
    /* 0x000 */ u8 unk0[0x1B0];
    /* 0x1B0 */ u32 m1B0;
};

// Zone record of lbl_803E6D00.m0->mWireGroups (lbl_805FB94C bytes each; real type
// unknown).
struct UnkZoneRecord {
    /* 0x000 */ u8 unk0[0x50];
    /* 0x050 */ Entity* m50;
    /* 0x054 */ u8 unk54[0x94 - 0x54];
    /* 0x094 */ int m94;
    /* 0x098 */ int m98;
    /* 0x09C */ u8 unk9C[0xA8 - 0x9C];
    /* 0x0A8 */ CameraInfluence** mInfluences;
    /* 0x0AC */ u8 mInfluenceCount;
    /* 0x0AD */ u8 unkAD[0xE4 - 0xAD];
    /* 0x0E4 */ u8 mE4[0x120 - 0xE4];
    /* 0x120 */ Entity** m120;
    /* 0x124 */ u8 unk124[0x12C - 0x124];
    /* 0x12C */ Entity** m12C;
};

// Fields of lbl_803E6D00.m0 used here (real type unknown).
struct UnkLevelData {
    /* 0x000 */ u8 unk0[0x4];
    /* 0x004 */ int m4;
    /* 0x008 */ u8 unk8[0x2C - 0x8];
    /* 0x02C */ void** m2C;
    /* 0x030 */ int m30;
    /* 0x034 */ u8 unk34[0x38 - 0x34];
    /* 0x038 */ void** m38;
    /* 0x03C */ int m3C;
    /* 0x040 */ u8 unk40[0x7C - 0x40];
    /* 0x07C */ u32 m7C;
    /* 0x080 */ u32 m80;
    /* 0x084 */ u8 m84;
    /* 0x088 */ int m88;
    /* 0x08C */ struct UnkPoolElement* m8C;
    /* 0x090 */ u8 m90;
    /* 0x091 */ u8 unk91[0x1A4 - 0x91];
    /* 0x1A4 */ u8* mZones;
    /* 0x1A8 */ u8 unk1A8[0x1B0 - 0x1A8];
    /* 0x1B0 */ void** m1B0;
    /* 0x1B4 */ u8 unk1B4[0x1BC - 0x1B4];
    /* 0x1BC */ void** m1BC;
    /* 0x1C0 */ u8 unk1C0[0x1C8 - 0x1C0];
    /* 0x1C8 */ void* m1C8;
};

// Pooled object array (real type unknown); elements are 0x1F4 bytes and point back
// to it.
struct UnkPoolElement {
    /* 0x000 */ u8 unk0[0x1DC];
    /* 0x1DC */ struct UnkPoolSet* mOwner;
    /* 0x1E0 */ u8 unk1E0[0x1F4 - 0x1E0];
};
struct UnkPoolSet {
    /* 0x00 */ int mCount;
    /* 0x04 */ UnkPoolElement* mElements;
    /* 0x08 */ u8 m8;
    /* 0x0C */ u32 mC;
    /* 0x10 */ u32 m10;
    /* 0x14 */ u8 m14;
};
extern UnkPoolSet lbl_8047C730;

extern u8 lbl_805FB948;
extern int lbl_805FB94C;

extern "C" {
void* fn_801088C0(void* a, int b, void* c);
void fn_801BC574(UnkStateInfo* info);
void fn_801D94CC(void* obj);
void fn_801D9A24(void* obj);
void fn_801DA1CC(void* obj, int arg);
void fn_801DA7C8(void* obj);
void fn_801DA81C(void* obj, int arg);
void fn_801DB160(void* obj);
void fn_801DB1AC(void* obj);
void fn_80052530(void* obj, int type, EntityInfo* info, int init);
void fn_8005CC30(void* obj, int type, EntityInfo* info, int init);
void fn_8004B604(void* obj, int type, EntityInfo* info, int init);
void fn_8004B498(void* obj, int type, EntityInfo* info, int init);
void fn_8004E684(void* obj, int type, EntityInfo* info, int init);
void fn_800636BC(void* obj, int type, EntityInfo* info, int init);
void fn_800E42E8(void* obj);
void fn_800E5550(void* obj);
void fn_8009E818(void* obj);
void fn_80006B60(void* obj);
void fn_800D8100(void* obj);
void fn_801E7E28(void* obj);
void fn_801070B0(int arg);
void fn_800D3D50(void);
void fn_800D424C(void* obj, void* arg);
void fn_800D41F4(void* obj, void* arg);
void fn_800D3D6C(void* obj);
void fn_8003DA7C(void);

void fn_8012F6B0(UnkList* list);
void fn_8012F818(UnkEntity90* obj);
void fn_8012F8B8(Entity* entity, int fromInfo, int type);
void fn_8012FD0C(UnkGlobals803E6D00* self, UnkZoneRecord* zone);
void fn_8012FF5C(UnkGlobals803E6D00* self, void* obj);
void fn_8012FFA4(void);
void fn_8013006C(UnkGlobals803E6D00* self);
}

// Rebuilds the collision primitives of each group of the list.
// 99.2 %: the group, the list offset and the primitive offset take r29-r31 in another
// order (declaration orders tried).
void fn_8012F6B0(UnkList* list) {
    UnkCollisionGroup* group;
    u32 i;
    u32 j;
    u32 count;
    u32 primitiveCount;
    if (list != 0) {
        count = list->mCount;
        for (i = 0; i < count; i++) {
            group = (UnkCollisionGroup*)list->Get(i);
            if (group != 0) {
                primitiveCount = group->mCount;
                for (j = 0; j < primitiveCount; j++) {
                    CollisionPrimitive* primitive = group->Get(j);
                    if (primitive != 0) {
                        switch (primitive->mType) {
                        case 0:
                            new (primitive) CollisionPrimitive_Sphere;
                            break;
                        case 3:
                            new (primitive) CollisionPrimitive_Hemisphere;
                            break;
                        case 4:
                            new (primitive) CollisionPrimitive_AABBox;
                            break;
                        case 5:
                            new (primitive) CollisionPrimitive_LineSeg;
                            break;
                        case 6:
                            new (primitive) CollisionPrimitive_Ray;
                            break;
                        case 7:
                            new (primitive) CollisionPrimitive_OBBox;
                            break;
                        }
                    }
                }
            }
        }
    }
}

static inline void fn_8012F818_case2(void* obj) {
    if (obj != 0) {
        fn_801DA81C(obj, 0);
    }
    fn_801DB160(obj);
}

void fn_8012F818(UnkEntity90* obj) {
    if (obj != 0) {
        switch (obj->m4) {
        case 1:
            if (obj != 0) {
                fn_801D94CC(obj);
            }
            break;
        case 2:
            fn_8012F818_case2(obj);
            break;
        case 3:
            if (obj != 0) {
                fn_801DA1CC(obj, 0);
            }
            fn_801DA7C8(obj);
            break;
        case 4:
            if (obj != 0) {
                fn_801D9A24(obj);
            }
            break;
        }
    }
}

// Rebuilds an entity for its type (from its info when fromInfo is set), then its
// attached objects and its children.
// 99.2 %: before setting flag 0x80 the DOL loads mInfo again; this build reuses the
// pointer loaded for the type mask test (inline helpers tried).
void fn_8012F8B8(Entity* entity, int fromInfo, int type) {
    if (entity == 0) {
        return;
    }
    if (fromInfo != 0 && entity->mInfo == 0) {
        return;
    }
    int kind = fromInfo != 0 ? entity->mInfo->mType : type;
    switch (kind) {
    case 2:
        new (entity) Entity_Static(kind, 0, 0);
        break;
    case 3:
        new (entity) Entity_Dynamic(kind, 0, 0);
        break;
    case 4:
        if (entity != 0) {
            fn_80052530(entity, kind, 0, 0);
        }
        break;
    case 5:
        if (entity != 0) {
            fn_8005CC30(entity, kind, 0, 0);
        }
        break;
    case 15:
        new (entity) Entity_Hazard_Prop(kind, 0, 0);
        break;
    case 16:
        new (entity) Entity_Challenge_Projector(kind, 0, 0);
        break;
    case 6:
        if (fn_801088C0(entity, entity->m9C != 0 ? entity->m9C->m8 : 0, 0) == 0) {
            new (entity) Entity_Prop(kind, 0, 0);
        }
        break;
    case 13:
        if (entity != 0) {
            fn_8004B604(entity, kind, 0, 0);
        }
        break;
    case 14:
        if (entity != 0) {
            fn_8004B498(entity, kind, 0, 0);
        }
        break;
    case 12:
        if (entity != 0) {
            fn_8004E684(entity, kind, 0, 0);
        }
        break;
    case 7:
        if (entity != 0) {
            fn_800636BC(entity, kind, 0, 0);
        }
        break;
    case 8:
        new (entity) Entity_Sensor(kind, 0, 0);
        break;
    case 9:
        new (entity) Entity_Trigger_Script(kind, 0, 0);
        break;
    case 10:
        new (entity) Entity_Light(kind, 0, 0);
        break;
    default:
        new (entity) Entity(kind, 0, 0);
        break;
    }

    if (fromInfo != 0) {
        fn_8012F818((UnkEntity90*)entity->m90);
    }

    void** a4 = entity->mA4;
    if (a4 != 0) {
        UnkEntityA4Camera* camera = (UnkEntityA4Camera*)a4[1];
        if (camera != 0) {
            camera->m18 = lbl_803C4298.m38;
        }
        UnkEntityA4State* state = (UnkEntityA4State*)a4[0];
        if (state != 0 && state->mInfo != 0) {
            ReleaseStateInfo(state->mInfo);
        }
    }

    fn_8012F6B0(entity->m8C);

    if (fromInfo != 0) {
        if (entity->mInfo->mTypeMask & 0x40) {
            Entity_Prop* prop = (Entity_Prop*)entity;
            if (prop->m194 != 0) {
                for (int i = 0; i < (int)prop->m1A4; i++) {
                    fn_8012F6B0(prop->m194[i]);
                }
            }
        }
        if (entity->mInfo->mTypeMask & 0x100) {
            if (lbl_805FB948 != 0) {
                if (!(((UnkSensorFlags*)entity)->m1B0 & 0x80)) {
                    SetFlag80(entity);
                }
            } else if (!(((UnkSensorFlags*)entity)->m1B0 & 0x40)) {
                SetFlag80(entity);
            }
        }
    }

    for (UnkEntityNode* node = entity->mB0; node != 0; node = node->mNext) {
        if (node->mEntity != 0) {
            fn_8012F8B8(node->mEntity, 1, 0);
        }
    }
}

// Rebuilds the entities and camera influences of a zone.
void fn_8012FD0C(UnkGlobals803E6D00* self, UnkZoneRecord* zone) {
    CameraInfluence* influence;
    u8 count;
    u32 i;
    for (int i = 0; i < zone->m94; i++) {
        fn_8012F8B8(zone->m120[i], 1, 0);
    }
    for (int i = 0; i < zone->m98; i++) {
        fn_8012F8B8(zone->m12C[i], 1, 0);
    }
    fn_8012F8B8(zone->m50, 1, 0);
    fn_801DB1AC(zone->mE4);

    count = zone->mInfluenceCount;
    for (i = 0; i < count; i++) {
        influence = zone->mInfluences[i];
        if (influence != 0) {
            switch (influence->mType) {
            case 0:
                new (influence) CIFixedEyeLook;
                break;
            case 1:
                new (influence) CIFixedTrackPlayer(1);
                break;
            case 2:
                new (influence) CIPlayerOffset;
                break;
            case 3:
                new (influence) CIDirectionGuide;
                break;
            case 8:
                new (influence) CITrackTarget;
                break;
            case 9:
                new (influence) CITrackTargetDan;
                break;
            case 4:
                new (influence) CIPitch;
                break;
            case 5:
                new (influence) CIDist;
                break;
            case 15:
                if (influence != 0) {
                    fn_800E42E8(influence);
                }
                break;
            case 16:
                if (influence != 0) {
                    fn_800E5550(influence);
                }
                break;
            case 17:
                if (influence != 0) {
                    fn_8009E818(influence);
                }
                break;
            case 18:
                new (influence) CIStitching;
                break;
            }
        }
    }
}

void fn_8012FF5C(UnkGlobals803E6D00* self, void* obj) {
    if (obj != 0) {
        fn_8012F818_case2(obj);
    }
}

// 99.6 %: the two increments of the loop come out in the other order.
void fn_8012FFA4(void) {
    fn_80006B60(&lbl_803E6D00.m0->m1E0);
    fn_800D8100(lbl_803B53A0.mD5F0);
    UnkLevelData* pools = (UnkLevelData*)lbl_803E6D00.m0;
    lbl_8047C730.m10 = pools->m80;
    lbl_8047C730.mC = pools->m7C;
    lbl_8047C730.m14 = pools->m84;
    lbl_8047C730.mElements = pools->m8C;
    lbl_8047C730.mCount = pools->m88;
    lbl_8047C730.m8 = pools->m90;
    if (pools->m80 != 0 && lbl_8047C730.mElements != 0) {
        for (int i = 0; i < lbl_8047C730.mCount; i++) {
            UnkPoolElement* element = &lbl_8047C730.mElements[i];
            if (element != 0) {
                element->mOwner = &lbl_8047C730;
            }
        }
    }
}

// Rebuilds the objects of the loaded level.
// 95.1 %: in its loops the DOL loads self->m0 again in the body, this build reuses the
// value loaded for the loop condition (while, inline accessor, local entity tried).
void fn_8013006C(UnkGlobals803E6D00* self) {
    fn_8012FFA4();
    UnkLevelData* level = (UnkLevelData*)self->m0;
    fn_8012FD0C(self, (UnkZoneRecord*)level->mZones);
    fn_801E7E28(((UnkLevelData*)self->m0)->m1C8);
    fn_8012FF5C(self, ((UnkLevelData*)self->m0)->m1B0[0]);
    fn_8012FF5C(self, ((UnkLevelData*)self->m0)->m1BC[0]);
    for (int i = 0; i <= ((UnkLevelData*)self->m0)->m4; i++) {
        UnkZoneRecord* zone =
            (UnkZoneRecord*)(((UnkLevelData*)self->m0)->mZones + i * lbl_805FB94C);
        if (zone->m50 != 0) {
            fn_8012F8B8(zone->m50, 1, 0);
        }
    }
    fn_801070B0(0);
    if (lbl_805FB180 != 0 && lbl_803B53A0.mFE0 == 0) {
        fn_800D3D50();
        for (int i = 0; i < ((UnkLevelData*)self->m0)->m30; i++) {
            fn_800D424C(lbl_805FB180, ((UnkLevelData*)self->m0)->m2C[i]);
        }
        for (int i = 0; i < ((UnkLevelData*)self->m0)->m3C; i++) {
            fn_800D41F4(lbl_805FB180, ((UnkLevelData*)self->m0)->m38[i]);
        }
        fn_800D3D6C(lbl_805FB180);
    }
    fn_8003DA7C();
}
