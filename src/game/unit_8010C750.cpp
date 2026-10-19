// Game unit at 0x8010C750-0x8010CD18 (real file name unknown): named script callbacks
// for render objects, pooled entities and state machine pools, registered at
// 0x800FB958 ("CloneRenderObjectCallback", "FixUpRenderObjectCallback",
// "RenderObjectZoneUnloadCallback", "DemolishRenderObjectCallback",
// "CreatePooledEntityCallback", "DemolishPooledEntityCallback",
// "FixUpPooledEntityCallback", "DeallocPooledEntityCallback", "FixUpEntityCallback",
// "DemolishEntityPoolCallback", "SpawnPooledEntityAt", "CloneFSMCallback",
// "DemolishFSMCallback", "DeallocFSMPoolCallback", "FixupFSMPoolCallback"), and the
// function that takes an entity from a pool and places it. Bounds come from the static
// initializers of this unit and the previous one.

#include "game/unk_vec3.mch"

#include "game/Entity.h"
#include "game/unk_803E6D00.h"
#include "game/unk_vec3.h"

// Re-entrancy guard (constructor 0x801F5384, destructor 0x801F53B0; real name
// unknown).
class UnkScopedGuard {
public:
    UnkScopedGuard();
    ~UnkScopedGuard();

    /* 0x0 */ int m0;
};

// Object handled by the render object callbacks (real type unknown; only the
// position of the virtual functions called here is known).
class UnkRenderObject {
public:
    virtual void vf08();
    virtual void vf0C();
    virtual void vf10();
};

// Entity pool entry (real type unknown).
struct UnkPoolEntry {
    /* 0x00 */ Entity* mEntity;
};

// Pool of one entity type (real type unknown).
struct UnkEntityPool {
    /* 0x00 */ u8 unk0[0x1C];
    /* 0x1C */ int m1C;
    /* 0x20 */ EntityInfo* mInfo; // template copied into each spawned entity
};

struct UnkZoneRef {
    /* 0x00 */ u8 unk0[0x18];
    /* 0x18 */ int mZone;
};

// Message filled by SpawnPooledEntityAt (real type unknown).
struct UnkSpawnMessage {
    /* 0x0 */ u8 unk0[0x4];
    /* 0x4 */ int m4;
    /* 0x8 */ int mType;
    /* 0xC */ int mId;
};

struct UnkStateInfo;

// Target that SpawnPooledEntityAt places the entity at (real type unknown);
// fn_801ED788 returns its position.
struct UnkTrackedTarget {
    UnkVec3 fn_801ED788() const;

    /* 0x0 */ int m0;
};

extern "C" {
void fn_800E8240(Entity* entity);
void fn_8012CDFC(Entity* entity);
int fn_8012E420(UnkGlobals803E6D00* globals, Entity* entity, u8 type, int arg);
int fn_8012F408(void* obj, int zone);
void fn_8012F818(void* obj, int arg);
void fn_8012F8B8(void* obj, int arg1, int arg2);
void fn_801B4EA4(Entity* entity, int arg);
void fn_801BC574(UnkStateInfo* info);
void fn_801D4980(void* ptr);
UnkEntityPool* fn_801DB2E4(void* pools, int type);
UnkPoolEntry* fn_801DB194(UnkEntityPool* pool);
void* fn_801F3918(int type, int arg);

int fn_8010C750(void** slot, UnkEntityPool* pool);
int fn_8010C758(void** slot, UnkEntityPool* pool);
int fn_8010C784(UnkRenderObject** slot, UnkEntityPool* pool);
int fn_8010C7C0(UnkRenderObject** slot, UnkEntityPool* pool);
int fn_8010C810(Entity** slot, UnkEntityPool* pool);
int fn_8010C818(Entity** slot, UnkEntityPool* pool);
int fn_8010C87C(Entity** slot, UnkEntityPool* pool);
int fn_8010C8B4(Entity** slot, UnkEntityPool* pool);
int fn_8010C8C8(Entity* entity, UnkEntityPool* pool);
void fn_8010C8F8(UnkEntityPool* pool);
Entity* fn_8010C90C(int type, const UnkVec3* pos, int hidden);
void fn_8010CBFC(int type, UnkTrackedTarget* target);
int fn_8010CC98(void** slot, void* pool);
int fn_8010CCA0(void** slot, void* pool);
int fn_8010CCA8(void** slot, void* pool);
int fn_8010CCB0(UnkStateInfo*** slot, void* pool);
}

// Next id given to a spawned entity (.sbss, also used by the unit at 0x800636A0).
extern int lbl_805FB6AC;

// "CloneRenderObjectCallback"
int fn_8010C750(void** slot, UnkEntityPool* pool) {
    return 1;
}

// "FixUpRenderObjectCallback"
int fn_8010C758(void** slot, UnkEntityPool* pool) {
    fn_8012F818(*slot, 0);
    return 1;
}

// "RenderObjectZoneUnloadCallback"
int fn_8010C784(UnkRenderObject** slot, UnkEntityPool* pool) {
    UnkRenderObject* object = *slot;
    if (object != 0) {
        object->vf10();
    }
    return 1;
}

// "DemolishRenderObjectCallback"
int fn_8010C7C0(UnkRenderObject** slot, UnkEntityPool* pool) {
    UnkRenderObject* object = *slot;
    if (object != 0) {
        object->vf10();
        fn_801D4980(object);
    }
    return 1;
}

// "CreatePooledEntityCallback"
int fn_8010C810(Entity** slot, UnkEntityPool* pool) {
    return 0;
}

// "DemolishPooledEntityCallback"
int fn_8010C818(Entity** slot, UnkEntityPool* pool) {
    Entity* entity = *slot;
    if (entity != 0) {
        entity->vf40();
        fn_801D4980(entity);
        *slot = 0;
    }
    return 1;
}

// "FixUpPooledEntityCallback"
int fn_8010C87C(Entity** slot, UnkEntityPool* pool) {
    Entity* entity = *slot;
    if (entity != 0) {
        fn_8012F8B8(entity, 1, 0);
    }
    return 1;
}

// "DeallocPooledEntityCallback"
int fn_8010C8B4(Entity** slot, UnkEntityPool* pool) {
    (*slot)->mInfo = pool->mInfo;
    return 1;
}

// "FixUpEntityCallback"
int fn_8010C8C8(Entity* entity, UnkEntityPool* pool) {
    fn_8012F8B8(entity, 0, pool->m1C);
    return 1;
}

// "DemolishEntityPoolCallback"
void fn_8010C8F8(UnkEntityPool* pool) {
    if (pool->mInfo != 0) {
        fn_801D4980(pool->mInfo);
    }
}

inline void UnkCopyInfo(EntityInfo* dst, const EntityInfo* src) {
    dst->mPos.x = src->mPos.x;
    dst->mPos.y = src->mPos.y;
    dst->mPos.z = src->mPos.z;
    dst->mRot.x = src->mRot.x;
    dst->mRot.y = src->mRot.y;
    dst->mRot.z = src->mRot.z;
    dst->mRot.w = src->mRot.w;
    dst->mTypeMask = src->mTypeMask;
    dst->mFlags = src->mFlags;
    dst->m24 = src->m24;
    dst->m28 = src->m28;
    dst->m2C = src->m2C;
    dst->m30 = src->m30;
    dst->m32 = src->m32;
    dst->mType = src->mType;
    dst->m35 = src->m35;
    dst->m36 = src->m36;
    dst->m37 = src->m37;
    dst->m38 = src->m38;
    dst->m39 = src->m39;
    dst->m3A = src->m3A;
    dst->m3B = src->m3B;
    dst->m3C = src->m3C;
    dst->m3D = src->m3D;
}

inline UnkVec3 UnkVec3One() {
    UnkVec3 v;
    v.x = v.y = v.z = 1.0f;
    return v;
}

// Takes an entity of the given type from its pool and places it at pos.
Entity* fn_8010C90C(int type, const UnkVec3* pos, int hidden) {
    UnkScopedGuard guard;
    UnkVec3 position = *pos;
    UnkEntityPool* pool = fn_801DB2E4(lbl_803E6D00.m0->mWireGroups + 0xE4, type);
    if (pool == 0) {
        return 0;
    }
    UnkPoolEntry* entry = fn_801DB194(pool);
    if (entry == 0) {
        return 0;
    }
    Entity* entity = entry->mEntity;
    if (entity == 0) {
        return 0;
    }
    if (hidden != 0) {
        entity->mInfo->mFlags |= 0x4000000;
    }
    entity->mBC = entry;
    if (fn_8012E420(&lbl_803E6D00, entity, entity->mInfo->mType, 0) == 0) {
        return 0;
    }
    UnkCopyInfo(entity->mInfo, pool->mInfo);
    entity->mC8 = position;
    entity->m18 = position;
    entity->m8 = UnkVec3One();
    entity->mInfo->mPos = position;
    entity->mInfo->mFlags |= 0x8000;
    entity->mInfo->mFlags |= 0x10000000;
    entity->mInfo->mFlags |= 0x4000;
    entity->mInfo->mFlags |= 0x10000000;
    if (lbl_803E6D00.mE0 != 0 && fn_8012F408(&lbl_803E6D00, lbl_803E6D00.mE0->mZone)) {
        entity->mInfo->m30 = lbl_803E6D00.mE0->mZone;
    }
    entity->mInfo->m32 = 0;
    fn_801B4EA4(entity, 0);
    fn_8012CDFC(entity);
    fn_800E8240(entity);
    return entity;
}

// "SpawnPooledEntityAt"
void fn_8010CBFC(int type, UnkTrackedTarget* target) {
    UnkVec3 pos = target->fn_801ED788();
    Entity* entity = fn_8010C90C(type, &pos, 0);
    if (entity != 0) {
        entity->m30 = lbl_805FB6AC;
        lbl_805FB6AC++;
        UnkSpawnMessage* message = (UnkSpawnMessage*)fn_801F3918(4, 0);
        if (message != 0) {
            message->mId = entity->m30;
            message->mType = type;
            message->m4 = target->m0;
        }
    }
}

// "CloneFSMCallback"
int fn_8010CC98(void** slot, void* pool) {
    return 1;
}

// "DemolishFSMCallback"
int fn_8010CCA0(void** slot, void* pool) {
    return 1;
}

// "DeallocFSMPoolCallback"
int fn_8010CCA8(void** slot, void* pool) {
    return 1;
}

inline void UnkFixupStateInfo(UnkStateInfo* info) {
    if (info != 0) {
        fn_801BC574(info);
    }
}

// "FixupFSMPoolCallback"
int fn_8010CCB0(UnkStateInfo*** slot, void* pool) {
    if (slot != 0 && *slot != 0) {
        UnkStateInfo* info = **slot;
        if (info != 0) {
            UnkFixupStateInfo(info);
        }
    }
    return 1;
}
