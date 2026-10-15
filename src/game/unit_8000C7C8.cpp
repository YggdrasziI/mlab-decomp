// Game unit at 0x8000C7C8-0x8000CF8C (real file name unknown): top-level game flow
// helpers: delayed event, the state stack update, start-up of the global managers,
// screen set loading ("ScreenSet load start: zone=%d" / "... end: ..."), and small
// forwarders to the managers. Bounds come from the static initializers of this unit
// and the previous one.

#include "game/unk_803B53A0.h"
#include "game/unk_803C4298.h"
#include "game/unk_803E6D00.h"
#include "game/unk_vec3.h"
#include <string.h>

// Real names unknown.
struct UnkStateInfo {
    /* 0x0 */ u8* mEntries; // 0x30-byte records
};

struct UnkState {
    /* 0x00 */ UnkStateInfo* mInfo;
    /* 0x04 */ int mIndex;
    /* 0x08 */ u8 unk8[0x28 - 0x8];
    /* 0x28 */ u32 mFlags;
};

struct UnkStateEntry {
    /* 0x0 */ u32 m0;
    /* 0x4 */ u32 mId;
};

struct UnkStateStack {
    bool HasPending() const { return mPending != 0; }

    /* 0x000 */ u8 unk0[0x42C];
    /* 0x42C */ void* mPending;
    /* 0x430 */ u8 m430;
};

struct UnkGlobals803C2A20 {
    /* 0x000 */ u8 unk0[0x844];
    /* 0x844 */ void* m844;
    /* 0x848 */ u8 unk848[0xD98 - 0x848];
    /* 0xD98 */ UnkState* mState;
    /* 0xD9C */ UnkStateStack* mStateStack;
    /* 0xDA0 */ u8 unkDA0[0xDA8 - 0xDA0];
    /* 0xDA8 */ void* mDA8;
    /* 0xDAC */ u8 unkDAC[0xDB0 - 0xDAC];
    /* 0xDB0 */ u8* mDB0;
    /* 0xDB4 */ void* mDB4;
    /* 0xDB8 */ void** mDB8;
    /* 0xDBC */ void* mDBC;
};
extern UnkGlobals803C2A20 lbl_803C2A20;

// Global event manager (0xC2C bytes; constructor at 0x801B9C0C).
class UnkEventManager {
public:
    UnkEventManager();
};

inline void* operator new(size_t, void* where) {
    return where;
}

struct UnkScreenSetEntry {
    /* 0x00 */ u8 unk0[0x44];
    /* 0x44 */ int mZone;
};

struct UnkScreenSets {
    /* 0x00 */ u8 unk0[0x20];
    /* 0x20 */ void* mTable;
};

struct UnkZone {
    /* 0x0 */ u8 unk0[0x4];
    /* 0x4 */ int mZone;
};

struct UnkZoneLoad {
    /* 0x00 */ u8 unk0[0x18];
    /* 0x18 */ int mZone;
};

struct UnkWireGroupEx {
    /* 0x000 */ u8 unk0[0x100];
    /* 0x100 */ void* m100;
};

extern UnkEventManager* lbl_805FB6C8;
extern u8* lbl_805FB654;
extern void* lbl_805FB660;
extern void** lbl_805FB658;
extern void* lbl_805FB670;
extern void* lbl_805FB888;
extern void* lbl_805FB910;
extern UnkScreenSets* lbl_805FB218;
extern int lbl_805FB94C;
extern int lbl_805F81C0;
// 0x18-byte object whose first word points to the zone id table (real type unknown).
struct UnkZoneTable {
    /* 0x00 */ u32* mIds;
    /* 0x04 */ u8 unk4[0x18 - 0x4];
};
extern UnkZoneTable lbl_803C7434;
extern u8 lbl_8047C640[];
extern u8 lbl_8048A8F0[];
extern u8 lbl_80359420[];
extern u8 lbl_803F22D8[];

static int lbl_805FAE88;
int lbl_805FAE8C;
static UnkState* lbl_805FAE90;
static u32 lbl_805FAE94;
static int lbl_805FAE98;
static int lbl_805FAE9C;
static f32 lbl_805FAEA0;

int lbl_805F81A8 = 1;

extern "C" {
void fn_80074D60(int arg);
void fn_801BA190(void* mgr, void* sender, int arg2, u32 hash, int arg4, int arg5, int arg6, int arg7);
void fn_8001B0B8(UnkStateStack* stack, u32 id);
void fn_8001B128(UnkStateStack* stack);
void fn_8001B164(UnkStateStack* stack);
void fn_8001AFB4(UnkStateStack* stack, UnkState** current);
void fn_8001AFCC(UnkStateStack* stack);
void fn_801BB5BC(UnkState* state, f32 dt);
int fn_801432FC(void* obj);
int fn_800D83A8(void* obj);
int fn_8003CF40(void);
void* fn_801D4650(u32 size);
void fn_80006CA8(void* obj);
void fn_8019EA7C(void* obj);
void fn_8018C8F4(void* obj);
void fn_801BC574(UnkStateInfo* info);
void fn_801ABA2C(void);
int fn_801091B4(const char* path);
void fn_8000CFD8(int arg);
void* fn_8018D1E4(void* obj);
void fn_8018D4EC(void* obj, int arg);
void fn_80198C78(void* obj);
void fn_800E6A84(int arg);
void fn_8018D524(void* obj, int arg, void* group);
void fn_801330D4(void* obj, u32 id, int arg);
UnkScreenSetEntry* fn_801EBE74(void* table, int arg, int arg2);
void fn_8018DCA4(void* obj);
UnkZone* fn_8018D1EC(void* obj, int arg);
void* fn_801D3A60(int zone);
void fn_80131A18(void* obj, void* arg, int arg2);
void fn_80133840(void* obj, int zone);
void fn_80132098(void* obj, int zone, int arg, void* callback, int arg2);
int fn_8012F408(void* obj, int zone);
void fn_8000B7A8(void* obj, int arg, int arg2);
void fn_8000B924(void* obj, int arg, int arg2);
void fn_8000B964(void* obj, int arg);
void fn_8018D70C(void* obj, int arg);
void fn_8018DBB8(void* obj, int arg);
int fn_8018D6C8(void* obj);
void* fn_80143310(void* obj);
void* fn_80143330(void* obj);
u32 fn_801EB704(void* name);

void fn_8000CC30(void* logger, const char* format, ...);
void fn_8000CB90(UnkZoneLoad* load, int arg);

void fn_8000C7C8(void) {
    fn_80074D60(1);
}

void fn_8000C7D0(u32 hash, int arg1, int arg2, f32 delay) {
    lbl_805FAE94 = hash;
    lbl_805FAE98 = arg1;
    lbl_805FAE90 = lbl_803C2A20.mState;
    lbl_805FAE9C = arg2;
    lbl_805FAEA0 = delay;
}

void fn_8000C7F4(u32 hash, int arg1, int arg2) {
    fn_801BA190(lbl_805FB6C8, lbl_803C2A20.mState, 0, hash, arg1, arg2, 0, 0);
}

void fn_8000C820(int value) {
    lbl_805FAE88 = value;
}

int fn_8000C828(void) {
    return lbl_805F81A8 == 0;
}

// NonMatching (96 %): the DOL computes the pending test as a 0/1 value
// (neg / or / srwi.) before branching; our build compares the pointer directly.
void fn_8000C838(f32 dt) {
    if (lbl_805F81A8 != 0) {
        if (lbl_805FAEA0 > 0.0f) {
            lbl_805FAEA0 -= dt;
            if (lbl_805FAEA0 <= 0.0f) {
                fn_801BA190(lbl_805FB6C8, lbl_803C2A20.mState, 0, lbl_805FAE94, 0, 0, 0, 0);
                lbl_805FAEA0 = 0.0f;
            }
        }
        UnkStateStack* stack = lbl_803C2A20.mStateStack;
        if (stack->HasPending()) {
            fn_8001B128(stack);
        } else if (stack->m430) {
            fn_8001B164(stack);
        }
        fn_801BB5BC(lbl_803C2A20.mState, dt);
    }
    if (lbl_805F81A8 == 0) {
        if (fn_801432FC(lbl_803F22D8) == 0) {
            lbl_805FAE88 = 1;
        }
    }
}

void fn_8000C940(void) {
    lbl_803C2A20.mState->mFlags |= 0x200;
}

void fn_8000C95C(u32 id) {
    fn_8001B0B8(lbl_803C2A20.mStateStack, id);
}

void fn_8000C970(void) {
    lbl_803C2A20.mStateStack->m430 = 1;
}

int fn_8000C988(void) {
    if (lbl_803C4298.m18 == 5) {
        if (fn_800D83A8((u8*)&lbl_803B53A0 + 0xD5F0) == 6) {
            return fn_8003CF40();
        }
        return 1;
    }
    return 0;
}

void fn_8000C9E8(void) {
    lbl_805FB6C8 = (UnkEventManager*)fn_801D4650(0xC2C);
    if (lbl_805FB6C8 != 0) {
        new (lbl_805FB6C8) UnkEventManager;
    }
    fn_80006CA8(&lbl_803B53A0);
    fn_8019EA7C(lbl_80359420);
    lbl_805FB654 = lbl_803C2A20.mDB0;
    *(u32*)(lbl_805FB654 + 0x84) = 0;
    *(u32*)(lbl_805FB654 + 0x88) = 0;
    *(u32*)(lbl_805FB654 + 0x8C) = 0;
    *(u32*)(lbl_805FB654 + 0x90) = 0;
    fn_8018C8F4(lbl_805FB654);
    lbl_805FB660 = lbl_803C2A20.mDB4;
    lbl_805FB658 = lbl_803C2A20.mDB8;
    lbl_805FB658[1] = lbl_8047C640;
    lbl_805FB670 = lbl_803C2A20.mDBC;
    lbl_805FB888 = lbl_803C2A20.m844;
    lbl_805FB910 = lbl_803C2A20.mDA8;
    UnkState* state = lbl_803C2A20.mState;
    if (state != 0 && state->mInfo != 0) {
        if (state->mInfo != 0) {
            fn_801BC574(state->mInfo);
        }
    }
    fn_8001AFB4(lbl_803C2A20.mStateStack, &lbl_803C2A20.mState);
    fn_8001AFCC(lbl_803C2A20.mStateStack);
    fn_801ABA2C();
    fn_801091B4("ui\\ScreenSets\\ScreenSets.csv");
    memset(lbl_805FB654 + 0x88, 0, 0xC);
    if (lbl_805F81A8 != 0) {
        fn_8000CFD8(2);
    }
}

void fn_8000CB20(int arg) {
    void* previous = fn_8018D1E4(lbl_805FB654);
    fn_8018D4EC(lbl_805FB654, arg);
    if (previous != fn_8018D1E4(lbl_805FB654)) {
        if (previous != 0) {
            fn_80198C78(previous);
        }
        fn_800E6A84(0);
    }
}

void fn_8000CB90(UnkZoneLoad* load, int arg) {
    fn_8018D524(lbl_805FB654, arg,
                ((UnkWireGroupEx*)(lbl_803E6D00.m0->mWireGroups + load->mZone * lbl_805FB94C))->m100);
    fn_801330D4(&lbl_803E6D00, lbl_803C7434.mIds[load->mZone], arg);
    fn_8000CC30(lbl_8048A8F0, "ScreenSet load end: zone=%d", load->mZone);
}

void fn_8000CC30(void* logger, const char* format, ...) {}

void fn_8000CC80(int arg, int id) {
    UnkScreenSetEntry* entry = fn_801EBE74(lbl_805FB218->mTable, id, 0);
    if (entry != 0) {
        int zone = entry->mZone;
        if (lbl_803C4298.m18 == 5 || lbl_803C4298.m18 == 7) {
            fn_8018DCA4(lbl_805FB654);
            UnkZone* current = fn_8018D1EC(lbl_805FB654, arg);
            if (current != 0) {
                if (current->mZone == zone) {
                    return;
                }
                fn_80131A18(&lbl_803E6D00, fn_801D3A60(current->mZone), 1);
            }
            fn_800E6A84(0);
            fn_80133840(&lbl_803E6D00, zone);
            fn_8000CC30(lbl_8048A8F0, "ScreenSet load start: zone=%d", entry->mZone);
            fn_80132098(&lbl_803E6D00, zone, 1, (void*)fn_8000CB90, arg);
        }
    }
}

void fn_8000CD8C(void) {}

int fn_8000CD90(int arg) {
    int result = 0;
    UnkZone* zone = fn_8018D1EC(lbl_805FB654, arg);
    if (zone != 0) {
        result = fn_8012F408(&lbl_803E6D00, zone->mZone);
    }
    return result;
}

void fn_8000CDE8(void) {
    lbl_805FAE8C = 1;
}

int fn_8000CDF4(void) {
    int result = 0;
    if (lbl_805FAE8C != 0) {
        result = 1;
    }
    return result;
}

void fn_8000CE0C(void) {
    fn_8000B7A8(&lbl_803C4298, 0x43, 0);
}

int fn_8000CE20(void) {
    return lbl_805F81C0 == 2;
}

void fn_8000CE34(int arg) {
    fn_8000B924(&lbl_803C4298, 4, arg);
}

void fn_8000CE48(void) {
    fn_8000B964(&lbl_803C4298, 4);
}

void fn_8000CE58(int arg) {
    fn_8018D70C(lbl_805FB654, arg);
}

void fn_8000CE64(int arg) {
    fn_8018DBB8(lbl_805FB654, arg);
}

void fn_8000CE70(void) {
    fn_8018DCA4(lbl_805FB654);
}

int fn_8000CE78(void) {
    if (lbl_805FB654 != 0) {
        return fn_8018D6C8(lbl_805FB654);
    }
    return 0;
}

int fn_8000CE90(u32 id) {
    UnkState* state = lbl_803C2A20.mState;
    UnkStateEntry* entry;
    if (state->mIndex > -1) {
        entry = (UnkStateEntry*)(state->mInfo->mEntries + state->mIndex * 0x30);
    } else {
        entry = 0;
    }
    u32 current;
    if (entry != 0) {
        current = entry->mId;
    } else {
        current = 0;
    }
    return id == current;
}

int fn_8000CEE4(u32 id) {
    return fn_801EB704(fn_80143310(lbl_803F22D8)) == id;
}

int fn_8000CF28(u32 id) {
    return fn_801EB704(fn_80143330(lbl_803F22D8)) == id;
}

}
