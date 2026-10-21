// Game unit at 0x800D6B0C-0x800D7F24 (real file name unknown): a schedule of up to 20
// timed entries (real names unknown). An active entry counts its timer down; at zero
// it either spawns an object at a path point of Exploration::PathAPI (picked from the
// point links of its key) or sends one of three commands to the creature of its id.
// The start is the end of the static initializer at 0x800D6AEC (previous unit), the
// end this unit's static initializer at 0x800D7F04.
// NonMatching: six functions differ (commented below), mostly in register numbering.

#include "game/unk_vec3.mch"
#include "game/Entity.h"
#include "game/PathAPI.h"
#include "game/unk_803B53A0.h"
#include "game/unk_803C4298.h"
#include "game/unk_803E6D00.h"
#include "game/unk_creature.h"
#include "game/unk_vec3.h"

struct UnkOwner;
struct UnkResource;
struct UnkPooled;

UnkPooled* fn_8000C508(UnkOwner* owner, UnkResource* res, void* arg);

extern "C" {
int fn_8004D170(Entity* entity);
u32 fn_80143460();
int fn_800D83A8(void* obj);
int fn_800E6BD4();
UnkCreature* fn_800E876C(u32 id);
void fn_800E8B94(UnkCreature* creature, int action, u32 point, int a, int b);
void fn_800E8E00(UnkCreature* creature, u32 point, int a, int b);
void fn_800E909C(UnkCreature* creature, int action, u32 point, int a, int b);
void fn_800F1204(UnkCreature* creature, u32 hash, int arg);
Unk800D50C4* fn_800D39BC(Exploration::PathAPI* api, u32 id);
UnkVec3* fn_800D5718(Unk800D50C4* follower);
}

// Entry of the schedule, 0x28 bytes.
struct Unk800D6B0C {
    Unk800D6B0C();

    /* 0x00 */ u32 m0; // creature id
    /* 0x04 */ u32 m4; // path point id
    /* 0x08 */ int m8; // point link key
    /* 0x0C */ f32 mC; // shortest delay
    /* 0x10 */ f32 m10; // longest delay
    /* 0x14 */ f32 m14; // timer
    /* 0x18 */ int m18;
    /* 0x1C */ int m1C;
    /* 0x20 */ u8 m20; // active
    /* 0x24 */ UnkPooled* m24; // spawned object
};

class Unk800D6B44 : public UnkCountedArray<Unk800D6B0C, 20> {
public:
    // Declared: `delete` then tests the pointer twice, as in the DOL.
    ~Unk800D6B44() {}
    void fn_800D6E90(f32 dt);
    void fn_800D76FC(int i);
    void fn_800D795C(int i);
};

extern "C" {
void fn_800D6B44();
void fn_800D6DDC();
void fn_800D6E14(f32 dt);
void fn_800D70EC(const Unk800D6B0C* entry);
void fn_800D7288(u32 id);
void fn_800D7394(u32 id);
void fn_800D75D0(UnkPooled* spawned);
}

Unk800D6B44* lbl_805FB188;

Unk800D6B0C::Unk800D6B0C() {
    m0 = 0;
    m4 = 0;
    m8 = 0;
    mC = 0.0f;
    m10 = 0.0f;
    m14 = 0.0f;
    m18 = -1;
    m1C = -1;
    m20 = 0;
    m24 = 0;
}

void fn_800D6B44() {
    if (lbl_805FB188 != 0) {
        delete lbl_805FB188;
    }
    lbl_805FB188 = 0;
    lbl_805FB188 = new Unk800D6B44;
    lbl_805FB188->Clear();
}

void fn_800D6DDC() {
    if (lbl_805FB188 != 0) {
        delete lbl_805FB188;
    }
    lbl_805FB188 = 0;
}

void fn_800D6E14(f32 dt) {
    if (lbl_805FB188 != 0 && fn_800D83A8(lbl_803B53A0.mD5F0) == 2 && lbl_803C4298.m18 == 5 &&
        !fn_800E6BD4()) {
        lbl_805FB188->fn_800D6E90(dt);
    }
}

// 99.3 %: same instructions; a relocation differs (objdiff).
void Unk800D6B44::fn_800D6E90(f32 dt) {
    if (fn_8004D170(lbl_803B53A0.mD614) != 0) {
        return;
    }
    if (fn_800D83A8(lbl_803B53A0.mD5F0) != 2) {
        return;
    }
    for (int i = 0; i < Count(); i++) {
        if ((*this)[i].m20 && (*this)[i].m24 == 0) {
            (*this)[i].m14 -= dt;
            if ((*this)[i].m14 <= 0.0f) {
                if ((*this)[i].m1C != -1 || (*this)[i].m18 != -1) {
                    fn_800D76FC(i);
                } else {
                    fn_800D795C(i);
                }
            }
        }
    }
}

// 99.7 %: the id and the loop pointer in swapped registers (declaration orders
// tried).
void fn_800D70EC(const Unk800D6B0C* entry) {
    int i;
    int found;
    u32 id;
    Unk800D6B44* schedule;
    schedule = lbl_805FB188;
    if (schedule == 0) {
        return;
    }
    id = entry->m0;
    found = -1;
    for (i = 0; i < schedule->Count(); i++) {
        if (id == (*schedule)[i].m0) {
            found = i;
            break;
        }
    }
    if (found >= 0) {
        (*lbl_805FB188)[found] = *entry;
        return;
    }
    if (lbl_805FB188->Count() < 20) {
        lbl_805FB188->Add(*entry);
    }
}

// 99.1 %: the loop index and pointer in swapped registers.
void fn_800D7288(u32 id) {
    int found;
    int i;
    Unk800D6B44* schedule;
    schedule = lbl_805FB188;
    if (schedule == 0) {
        return;
    }
    found = -1;
    for (i = 0; i < schedule->Count(); i++) {
        if (id == (*schedule)[i].m0) {
            found = i;
            break;
        }
    }
    if (found >= 0) {
        lbl_805FB188->RemoveAt(found);
    }
}

// 99.6 %: loop registers as in fn_800D7288.
void fn_800D7394(u32 id) {
    int found;
    int i;
    Unk800D6B44* schedule;
    schedule = lbl_805FB188;
    if (schedule == 0) {
        return;
    }
    found = -1;
    for (i = 0; i < schedule->Count(); i++) {
        if (id == (*schedule)[i].m0) {
            found = i;
            break;
        }
    }
    if (found >= 0) {
        f32 low = (*lbl_805FB188)[found].mC;
        f32 high = (*lbl_805FB188)[found].m10;
        Unk800D6B0C& entry = (*lbl_805FB188)[found];
        entry.m14 = low + (high - low) * (fn_80143460() * 2.3283064e-10f);
        (*lbl_805FB188)[found].m20 = 1;
        (*lbl_805FB188)[found].m24 = 0;
    }
}

// 97.5 %: the DOL indexes the entries with an offset register where this build
// steps a pointer.
void fn_800D75D0(UnkPooled* spawned) {
    Unk800D6B44* schedule = lbl_805FB188;
    if (schedule == 0) {
        return;
    }
    for (int i = 0; i < schedule->Count(); i++) {
        if ((*schedule)[i].m20 && (*schedule)[i].m24 == spawned) {
            schedule->fn_800D795C(i);
            (*lbl_805FB188)[i].m24 = 0;
            return;
        }
    }
}

// 87.8 %: the DOL loads the count again for each entry access (and clamps with the
// count's register), where this build keeps it; fn_800D795C, written the same way,
// matches. The stack layout matches.
void Unk800D6B44::fn_800D76FC(int i) {
    if (lbl_805FB188 == 0) {
        return;
    }
    if ((*this)[i].m8 != 0) {
        (*this)[i].m4 = lbl_805FB180->fn_800D4730((*this)[i].m8);
    }
    UnkResource* res = lbl_803E6D00.m0->m74;
    Unk800D50C4 follower(*fn_800D39BC(lbl_805FB180, (*this)[i].m4));
    UnkVec3 to;
    UnkVec3 from = *fn_800D5718(&follower);
    to.x = from.x;
    to.y = from.y;
    to.z = 25.0f + from.z;
    from.x -= to.x;
    from.y -= to.y;
    from.z -= to.z;
    (*this)[i].m24 = fn_8000C508((UnkOwner*)&UnkVec3(to), res, &from);
}

void Unk800D6B44::fn_800D795C(int i) {
    (*this)[i].m20 = 0;
    UnkCreature* creature = fn_800E876C((*this)[i].m0);
    if (creature != 0) {
        int key = (*this)[i].m8;
        if ((*this)[i].m1C != -1) {
            fn_800E909C(creature, (*this)[i].m1C, (*this)[i].m4, (int)(*this)[i].mC, (int)(*this)[i].m10);
        } else if ((*this)[i].m18 != -1 && (*this)[i].m4 != 0) {
            fn_800E8B94(creature, (*this)[i].m18, (*this)[i].m4, (int)(*this)[i].mC, (int)(*this)[i].m10);
        } else if ((*this)[i].m4 != 0) {
            fn_800E8E00(creature, (*this)[i].m4, (int)(*this)[i].mC, (int)(*this)[i].m10);
            if ((creature->mInfo->mTypeMask & 0x10) &&
                (creature->m6B4 == 0x4B9FE6EB || creature->m6B4 == 0x711A757D)) {
                fn_800F1204(creature, 0xE6BE4FBE, 0);
            }
        }
        if (key != 0) {
            (*this)[i].m8 = key;
        }
    }
}
