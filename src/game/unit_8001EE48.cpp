// Game unit at 0x8001EE48-0x8001FCA8 (real file name unknown): a creature viewer
// created on demand (lbl_805FAF80) that pages through the player's creatures and
// shows the current one on the stands model, like the Creatures section of the
// InventoryBook (unit_80117090.cpp), plus the selected part and its icon. Bounds: the
// previous unit's static initializer ends at 0x8001EE48 and this unit's static
// initializer ends at 0x8001FCA8.

#include "game/unk_vec3.mch"

#include "game/Journal.h"
#include "game/unk_803E6D00.h"
#include "game/unk_actor.h"
#include "game/unk_vec3.h"

// Stands model showing creatures (real type unknown; see unit_80117090.cpp).
struct UnkStands {
    /* 0x000 */ u8 unk0[0xC50];
    /* 0xC50 */ UnkActor* mModels[5];
    /* 0xC64 */ u8 unkC64[0xC6C - 0xC64];
    /* 0xC6C */ u32 mPending[5];
};

// Part of a creature (real type unknown).
struct UnkCreaturePart {
    /* 0x0 */ int mId;
};

// Creature record (real type unknown).
struct UnkCreatureRecord {
    /* 0x0 */ u8 unk0[0x4];
    /* 0x4 */ UnkCreaturePart* mParts[5];
};

// The player's creatures and parts (real type unknown).
struct UnkCreatureList {
    int IndexOf(UnkCreatureRecord* creature) {
        for (int i = 0; i < mCount; i++) {
            if (mItems[i] == creature) {
                return i;
            }
        }
        return -1;
    }

    /* 0x000 */ UnkCreatureRecord* mItems[10];
    /* 0x028 */ UnkCreaturePart* mParts[(0x6B8 - 0x28) / 4];
    /* 0x6B8 */ int mCount;
    /* 0x6BC */ int mPartCount;
};

// Player data (real type unknown).
struct UnkPlayerData {
    /* 0x0000 */ u8 unk0[0x114];
    /* 0x0114 */ UnkCreatureRecord* m114;
    /* 0x0118 */ u8 unk118[0xA30 - 0x118];
    /* 0x0A30 */ UnkCreatureList mCreatures;
};

// Monster description returned by the database (0x50 bytes, real type unknown; see
// unit_801196F4.cpp).
struct UnkMonsterDesc {
    /* 0x00 */ u8 unk0[0x50];
};

// Creature viewer (0x3C bytes, real type unknown).
struct UnkCreatureViewer {
    UnkCreatureViewer() : mCreature(0) {}
    ~UnkCreatureViewer();

    /* 0x00 */ u8 unk0[0x4];
    /* 0x04 */ UnkCreatureRecord* mCreature;
    /* 0x08 */ UnkActor* mActor;
    /* 0x0C */ UnkStands* mStands;
    /* 0x10 */ int mIndex;
    /* 0x14 */ int mShownIndex;
    /* 0x18 */ int mCount;
    /* 0x1C */ int mShown;
    /* 0x20 */ int mOpened;
    /* 0x24 */ char mName[0x10];
    /* 0x34 */ char mPosition[0x8];
};

extern "C" {
extern UnkPlayerData* lbl_805FBBE0;
extern void* lbl_805FBBC0;
extern void* lbl_805FB654;

// Pending creature requests and their count (shared with other units).
extern u32 lbl_803C4E28[];
extern int lbl_805FB034;
extern int lbl_805FB038;

void fn_80142EF8(char* buf, const char* fmt, ...);
void fn_80184028(UnkTexture* texture);
void fn_8003DCE0(UnkActor* model);
void fn_8004C718(UnkStands* stands, int index, void* model);
void fn_8004D760(UnkStands* stands, void* creature);
s32 fn_8004DAF8(UnkStands* stands, int arg1, int arg2);
void fn_8004DCEC(UnkStands* stands, int index);
int fn_8004E300(void* obj, int arg);
void fn_8004EDD0(UnkActor* model, int arg1, int arg2);
void* fn_8004FFB4(u32 pending, void* part);
UnkStands* fn_800E876C(u32 id);
UnkTexture* fn_8011AD88(const UnkMonsterDesc* desc, int highlighted);
void fn_80130418(UnkGlobals803E6D00* globals);
void* fn_8018D1E4(void* obj);
UnkActor* fn_80198F18(void* screens, u32 hash);
void fn_801A3010(void* actor, void* arg);
void* fn_801B7198(void* obj, int arg);
UnkMonsterDesc fn_80216838(void* db, int id);
void fn_8021C968(UnkCreatureRecord* creature, char* buf, int size);
f32 fn_8021CB98(UnkCreatureRecord* creature);
f32 fn_8021CCC0(UnkCreatureRecord* creature);
f32 fn_8021CDC0(UnkCreatureRecord* creature);
f32 fn_8021CEC0(UnkCreatureRecord* creature);
f32 fn_8021CF40(UnkCreatureRecord* creature);
f32 fn_8021D0B8(UnkCreatureRecord* creature);
UnkCreatureRecord* fn_8022E170(UnkPlayerData* player);

void fn_8001F6A4(UnkCreatureViewer* viewer);
}

// Offset of the cursor on some targets (.rodata, unsplit).
extern const UnkVec3 lbl_80346020;

// The viewer, the index of the selected part in the player's part list and its icon
// (real names unknown).
UnkCreatureViewer* lbl_805FAF80;
int lbl_805FAF84;
UnkTexture* lbl_805FAF88;
// Id of the selected part (real name unknown).
int lbl_805F8288 = -1;

inline void UnkReleaseModel(UnkStands* stands, int index) {
    UnkActor* model = stands->mModels[index];
    fn_8004C718(stands, index, 0);
    fn_8003DCE0(model);
}

inline UnkCreatureViewer::~UnkCreatureViewer() {
    if (mStands != 0) {
        for (int i = 0; i < 5; i++) {
            UnkReleaseModel(mStands, i);
        }
    }
    mCreature = 0;
    mStands = 0;
}

extern "C" void fn_8001EE48(void) {
    if (lbl_805FAF80 == 0) {
        lbl_805FAF80 = new UnkCreatureViewer;
        lbl_805FAF80->mActor = 0;
        lbl_805FAF80->mStands = 0;
        lbl_805FAF80->mShown = 0;
        lbl_805FAF80->mOpened = 0;
        lbl_805FAF80->mCreature = fn_8022E170(lbl_805FBBE0);
        lbl_805FAF80->mIndex = lbl_805FBBE0->mCreatures.IndexOf(lbl_805FAF80->mCreature);
        lbl_805FAF80->mShownIndex = lbl_805FAF80->mIndex;
        lbl_805FAF84 = 0;
        lbl_805FAF80->mCount = lbl_805FBBE0->mCreatures.mCount;
        fn_80142EF8(lbl_805FAF80->mPosition, "%d/%d", lbl_805FAF80->mIndex + 1,
                    lbl_805FAF80->mCount);
        fn_8001F6A4(lbl_805FAF80);
    }
}

extern "C" void fn_8001EF50(void) {
    if (lbl_805FAF80 != 0) {
        delete lbl_805FAF80;
        lbl_805FAF80 = 0;
    }
}

extern "C" void fn_8001EFFC(void) {
    if (lbl_805FAF80 == 0) {
        return;
    }
    if (lbl_805FAF80->mShown == 0 && lbl_805FAF80->mOpened == 0) {
        int ready = 1;
        for (int i = 0; i < lbl_805FB034; i++) {
            if (lbl_803C4E28[i] == 0) {
                ready = 0;
            }
        }
        if (ready) {
            UnkStands* stands = lbl_805FAF80->mStands;
            for (int i = 0; i < 5; i++) {
                if (stands->mPending[i] != 0 && lbl_805FAF80->mStands->mPending[i] < 1000) {
                    ready = 0;
                }
            }
        }
        if (ready) {
            for (int i = 0; i < 5; i++) {
                UnkReleaseModel(lbl_805FAF80->mStands, i);
            }
            lbl_805FAF80->mCreature = lbl_805FBBE0->mCreatures.mItems[lbl_805FAF80->mIndex];
            if (lbl_805FAF80->mCreature != 0) {
                fn_8004D760(lbl_805FAF80->mStands, lbl_805FAF80->mCreature);
                fn_8004DAF8(lbl_805FAF80->mStands, 1, 0);
                lbl_805FB038 = 5;
            }
            if (lbl_805FAF80->mActor != 0) {
                fn_801A3010(lbl_805FAF80->mActor, lbl_805FAF80->mStands);
                lbl_805FAF80->mActor->vf2C(0x1A);
            }
            lbl_805FAF80->mShownIndex = lbl_805FAF80->mIndex;
        }
    }
    if (lbl_805FAF80->mActor != 0 && fn_801B7198((u8*)lbl_805FAF80->mActor + 0x140, 0) != 0) {
        fn_80130418(&lbl_803E6D00);
        for (int i = 0; i < lbl_805FB034; i++) {
            if (lbl_803C4E28[i] == 0) {
                return;
            }
        }
        for (int i = 0; i < 5; i++) {
            fn_8004DCEC(lbl_805FAF80->mStands, i);
            if (lbl_805FAF80->mStands->mPending[i] != 0) {
                void* model = fn_8004FFB4(lbl_805FAF80->mStands->mPending[i],
                                          lbl_805FAF80->mCreature->mParts[i]);
                if (model != 0) {
                    fn_8004C718(lbl_805FAF80->mStands, i, 0);
                    fn_8004C718(lbl_805FAF80->mStands, i, model);
                    if (lbl_805FAF80->mStands->mModels[i] != 0) {
                        fn_8004EDD0(lbl_805FAF80->mStands->mModels[i], 1, 0);
                        *(u32*)((u8*)lbl_805FAF80->mStands->mModels[i] + 0x15C) |= 0x400000;
                    }
                }
                lbl_805FAF80->mStands->mPending[i] = 0;
            }
        }
        for (int i = 0; i < lbl_805FB034; i++) {
            lbl_803C4E28[i] = 0;
        }
        lbl_805FB034 = 0;
        if (lbl_805FAF80->mIndex == lbl_805FAF80->mShownIndex) {
            lbl_805FAF80->mShown = 1;
        }
    }
    if (lbl_805FAF80->mActor != 0) {
        UnkActor* actor = lbl_805FAF80->mActor;
        UnkVec3 pos = *(UnkVec3*)((u8*)actor + 0x14);
        void* target = fn_801B7198((u8*)actor + 0x140, 0);
        if (target != 0 && fn_8004E300(target, 1) == 13) {
            pos += UnkVec3(lbl_80346020.x, lbl_80346020.y, lbl_80346020.z);
        }
        lbl_805FAF80->mActor->mPos = UnkVec3(pos);
        lbl_805FAF80->mActor->vf2C(0x18);
    }
}

extern "C" void fn_8001F4FC(int delta) {
    if (lbl_805FAF80 != 0) {
        lbl_805FAF80->mShownIndex = lbl_805FAF80->mIndex;
        lbl_805FAF80->mIndex = lbl_805FAF80->mIndex + delta;
        if (lbl_805FAF80->mIndex >= lbl_805FAF80->mCount) {
            lbl_805FAF80->mIndex %= lbl_805FAF80->mCount;
        }
        while (lbl_805FAF80->mIndex < 0) {
            lbl_805FAF80->mIndex += lbl_805FAF80->mCount;
        }
        lbl_805FAF80->mCreature = lbl_805FBBE0->mCreatures.mItems[lbl_805FAF80->mIndex];
        fn_80142EF8(lbl_805FAF80->mPosition, "%d/%d", lbl_805FAF80->mIndex + 1,
                    lbl_805FAF80->mCount);
        lbl_805FAF80->mShown = 0;
        lbl_805FAF80->mOpened = 0;
    }
}

extern "C" f32 fn_8001F5CC(void) {
    if (lbl_805FAF80 != 0 && lbl_805FAF80->mCreature != 0) {
        return fn_8021CCC0(lbl_805FAF80->mCreature);
    }
    return 0.0f;
}

extern "C" f32 fn_8001F5F0(void) {
    if (lbl_805FAF80 != 0 && lbl_805FAF80->mCreature != 0) {
        return fn_8021CDC0(lbl_805FAF80->mCreature);
    }
    return 0.0f;
}

extern "C" f32 fn_8001F614(void) {
    if (lbl_805FAF80 != 0 && lbl_805FAF80->mCreature != 0) {
        return fn_8021CEC0(lbl_805FAF80->mCreature);
    }
    return 0.0f;
}

extern "C" f32 fn_8001F638(void) {
    if (lbl_805FAF80 != 0 && lbl_805FAF80->mCreature != 0) {
        return fn_8021CB98(lbl_805FAF80->mCreature);
    }
    return 0.0f;
}

extern "C" f32 fn_8001F65C(void) {
    if (lbl_805FAF80 != 0 && lbl_805FAF80->mCreature != 0) {
        return fn_8021CF40(lbl_805FAF80->mCreature);
    }
    return 0.0f;
}

extern "C" f32 fn_8001F680(void) {
    if (lbl_805FAF80 != 0 && lbl_805FAF80->mCreature != 0) {
        return fn_8021D0B8(lbl_805FAF80->mCreature);
    }
    return 0.0f;
}

extern "C" void fn_8001F6A4(UnkCreatureViewer* viewer) {
    viewer->mActor = fn_80198F18(fn_8018D1E4(lbl_805FB654), 0x2C7D4AEA);
    lbl_803C4E28[0] = 0;
    lbl_803C4E28[1] = 0;
    lbl_803C4E28[2] = 0;
    lbl_803C4E28[3] = 0;
    lbl_803C4E28[4] = 0;
    lbl_805FB038 = 5;
    lbl_805FB034 = 0;
    if (viewer->mStands == 0) {
        viewer->mStands = fn_800E876C(0x4CF9FE5B);
        if (viewer->mStands != 0) {
            if (viewer->mCreature != 0) {
                fn_8004D760(viewer->mStands, viewer->mCreature);
                fn_8004DAF8(viewer->mStands, 1, 0);
                lbl_805FB038 = 5;
            }
            if (viewer->mActor != 0) {
                fn_801A3010(viewer->mActor, viewer->mStands);
                viewer->mActor->vf2C(0x1A);
            }
        }
        viewer->mOpened = 1;
    }
}

extern "C" int fn_8001F794(void) {
    if (lbl_805FBBE0 != 0 && lbl_805FBBE0->mCreatures.mCount > 0) {
        return 1;
    }
    return 0;
}

extern "C" const char* fn_8001F7BC(void) {
    if (lbl_805FAF80 != 0 && lbl_805FAF80->mCreature != 0) {
        fn_8021C968(lbl_805FAF80->mCreature, lbl_805FAF80->mName, 0x10);
        return lbl_805FAF80->mName;
    }
    return 0;
}

extern "C" int fn_8001F80C(void) {
    if (lbl_805FAF80 != 0) {
        return lbl_805FAF80->mShown;
    }
    return 0;
}

extern "C" void fn_8001F828(void) {
    if (lbl_805FAF80 != 0) {
        lbl_805FBBE0->m114 = lbl_805FBBE0->mCreatures.mItems[lbl_805FAF80->mIndex];
    }
}

extern "C" const char* fn_8001F850(void) {
    if (lbl_805FAF80 != 0) {
        return lbl_805FAF80->mPosition;
    }
    return 0;
}

// Selects part `slot` of the current creature: its id and its index in the player's
// part list.
static inline void UnkSelectPart(int slot) {
    if (lbl_805FAF80 == 0) {
        return;
    }
    if (lbl_805FAF80->mCreature != 0) {
        UnkCreaturePart* part = lbl_805FAF80->mCreature->mParts[slot];
        if (part != 0) {
            int id = part->mId;
            lbl_805F8288 = id;
            lbl_805FAF84 = 0;
            if (id == -1) {
                return;
            }
            for (int i = 0; i < lbl_805FBBE0->mCreatures.mPartCount; i++) {
                if (lbl_805FBBE0->mCreatures.mParts[i] == part) {
                    lbl_805FAF84 = i;
                }
            }
            return;
        }
        lbl_805F8288 = -1;
        lbl_805FAF84 = 0;
        return;
    }
    lbl_805F8288 = -1;
    lbl_805FAF84 = 0;
}

extern "C" void fn_8001F86C(void) {
    UnkSelectPart(1);
}

extern "C" void fn_8001F908(void) {
    UnkSelectPart(4);
}

extern "C" void fn_8001F9A4(void) {
    UnkSelectPart(0);
}

extern "C" void fn_8001FA40(void) {
    UnkSelectPart(2);
}

extern "C" void fn_8001FADC(void) {
    UnkSelectPart(3);
}

extern "C" int fn_8001FB78(void) {
    return lbl_805F8288;
}

extern "C" int fn_8001FB80(void) {
    return lbl_805FAF84;
}

static inline void ReleaseTexture(UnkTexture* texture) {
    int count = --texture->mRefCount;
    if (count <= 0 && count == 0) {
        fn_80184028(texture);
    }
}

static inline void UnkReleaseIcon() {
    if (lbl_805FAF88 != 0) {
        ReleaseTexture(lbl_805FAF88);
        lbl_805FAF88 = 0;
    }
}

extern "C" void fn_8001FB88(void) {
    if (lbl_805FAF88 != 0) {
        UnkReleaseIcon();
    }
    if (lbl_805F8288 != -1) {
        UnkMonsterDesc desc = fn_80216838(lbl_805FBBC0, lbl_805F8288);
        lbl_805FAF88 = fn_8011AD88(&desc, 1);
    }
}

extern "C" void fn_8001FC20(void) {
    UnkReleaseIcon();
}

extern "C" UnkTexture* fn_8001FC68(void) {
    if (lbl_805FAF88 != 0) {
        return lbl_805FAF88;
    }
    return 0;
}

extern "C" int fn_8001FC7C(void) {
    return lbl_805FAF80->mShown;
}
