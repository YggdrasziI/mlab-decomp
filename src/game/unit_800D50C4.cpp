// Game unit at 0x800D50C4-0x800D6B0C (real file name unknown): the path follower
// Unk800D50C4 (types in game/PathAPI.h): it moves a cursor along the paths of
// Exploration::PathAPI, reads the fields of the point it stands on, manages the
// hazard of that point and searches the point graph (depth first, with the list
// of visited points). The start is the end of the static initializer at
// 0x800D50A4 (previous unit), the end this unit's static initializer at
// 0x800D6AEC. Most functions keep their address-based C names: other units call
// them by those names.
// NonMatching: nine functions differ in register numbering or the place of one
// load (commented below); the other functions match.

#include "game/unk_vec3.mch"
#include "game/Entity.h"
#include "game/PathAPI.h"
#include "game/unk_vec3.h"

extern "C" void* memset(void* dst, int value, u32 size);

// Hazard handle kept in Unk800D4C64::m50 (real type unknown).
struct UnkHazardType {
    /* 0x0 */ u32 m0;
};

struct UnkHazard {
    /* 0x0 */ u8 unk0[0x4];
    /* 0x4 */ UnkHazardType* m4;
};

typedef Unk800D50C4 Follower;

extern "C" {
Unk800D4C64* fn_800D5660(Follower* self);
u32 fn_800D5680(Follower* self);
u32 fn_800D56A0(Follower* self);
u32 fn_800D56C0(Follower* self);
int fn_800D56E0(Follower* self);
u32 fn_800D56F8(Follower* self);
UnkVec3* fn_800D5718(Follower* self);
UnkVec3 fn_800D5724(Follower* self);
int fn_800D5800(Follower* self);
u32 fn_800D581C(Follower* self);
void fn_800D5838(Follower* self, u32 value);
u32 fn_800D5850(Follower* self, int ignoreLocks);
u8 fn_800D5924(Follower* self);
int fn_800D5944(Follower* self);
u32 fn_800D5964(Follower* self);
u32 fn_800D5984(Follower* self);
u32 fn_800D59A4(Follower* self, int i);
u32 fn_800D59D4(Follower* self);
void fn_800D59F4(Follower* self, u8 on);
void fn_800D5B1C(Follower* self, void* type, const UnkVec3* pos);
void fn_800D5BD4(Follower* self, u32 type);
void fn_800D5C48(Follower* self, int on);
void fn_800D5D5C(Follower* self, int pause);
void fn_800D5DA4(Follower* self, int stop);
int fn_800D5DCC(Follower* self);
u32 fn_800D5DE4(Follower* self, Unk800D4C64* point, int ignoreLocks);
int fn_800D5FDC(Follower* self, Unk800D4C64* point, u32 targetId, int ignoreLocks);
int fn_800D61D8(Follower* self, Unk800D4C64* point, u32 pathId, int ignoreLocks);
u32 fn_800D63CC(Follower* self, int ignoreLocks);
int fn_800D648C(Follower* self, u32 targetId, int ignoreLocks);
int fn_800D6534(Follower* self, Follower* other, int ignoreLocks);
int fn_800D675C(Follower* self, int exit, int ignoreLocks);
int fn_800D68CC(Follower* self, int exit, u32 lock);
int fn_800D6998(Follower* self, int exit);
void fn_800D6A00(Follower* self);
void fn_800D6A18(Follower* self, f32 speed);
void fn_800D6A24(Follower* self);
}

// Free helpers (not UnkVec3 members): the member forms keep a pointer to the
// temporary where the DOL reads the stack slot directly.
static inline UnkVec3 Scaled(const UnkVec3& v, f32 s) {
    UnkVec3 result;
    result.x = v.x * s;
    result.y = v.y * s;
    result.z = v.z * s;
    return result;
}

static inline UnkVec3 Normalize(const UnkVec3& v) {
    f32 lengthSq = v.LengthSq();
    if (lengthSq == 0.0f) {
        return UnkVec3Zero();
    }
    return v * UnkInvSqrtf(lengthSq);
}

// UnkSqrtf returning f32: one frsp, as in the DOL.
static inline f32 SqrtF(f32 x) {
    f32 result;
    if (x <= 0.0f) {
        result = 0.0f;
    } else {
        double guess = __frsqrte(x);
        guess = 0.5 * (guess * (3.0 - x * guess * guess));
        guess = 0.5 * (guess * (3.0 - x * guess * guess));
        result = guess * x;
    }
    return result;
}

// Point the cursor moves to.
static inline UnkVec3* TargetPos(Unk800D50C4Ctx* ctx) {
    if (ctx->mIndex == -4) {
        return &ctx->mPath->m4[1]->m34;
    }
    if (ctx->mIndex == -3) {
        return &ctx->mPath->m4[0]->m34;
    }
    return &ctx->mPath->Point(ctx->mIndex).mPos;
}

// Exits of a point that can be taken (a set lock closes an exit unless
// ignoreLocks).
static inline u32 ExitMask(Unk800D4C64* point, int ignoreLocks) {
    u32 mask = 0;
    if (point != 0) {
        for (int i = 0; i < 4; i++) {
            if (point->mPaths[i] != 0 && (point->mLocks[i] == 0 || ignoreLocks != 0)) {
                mask |= 1 << i;
            }
        }
    }
    return mask;
}

// Point at the other end of an exit.
static inline Unk800D4C64* OtherEnd(Unk800D4C64* point, int exit) {
    Unk800D4C64* other = 0;
    if (point != 0) {
        Unk800D5008* path = point->mPaths[exit];
        if (path != 0) {
            other = path->m4[0];
            if (point == other) {
                other = path->m4[1];
            }
        }
    }
    return other;
}

static inline bool IsVisited(Follower* self, Unk800D4C64* point) {
    bool found = false;
    for (int i = 0; i < self->mVisitedCount; i++) {
        if (self->mVisited[i] == point) {
            found = true;
            break;
        }
    }
    return found;
}

static inline void Visit(Follower* self, Unk800D4C64* point) {
    self->mVisited[self->mVisitedCount] = point;
    self->mVisitedCount++;
}

Unk800D50C4::Unk800D50C4() {
    m0 = 0;
}

Unk800D50C4::~Unk800D50C4() {}

void Unk800D50C4::fn_800D5110(Unk800D50C4Ctx* ctx, Unk800D4C64* point) {
    m0 = ctx;
    ctx->mSpeed = 0.0f;
    m0->mForward = 1;
    m0->mState = 0;
    m0->mIndex = -1;
    m0->mPath = 0;
    m0->mPoint = point;
    m0->mPos = m0->mPoint->m34;
}

// 97.6 %: the DOL computes the point base of Point() before loading the count (the
// base and the count take the other registers); accessor and TargetPos spellings
// tried. The final scale goes through a double (`(double)step`, the DOL's frsp).
void Unk800D50C4::fn_800D5174(f32 dt) {
    Unk800D50C4Ctx* ctx = m0;
    if (ctx->mPoint != 0 || ctx->mState != 0) {
        return;
    }
    f32 step = ctx->mSpeed * dt;
    f32 stepSq = step * step;
    UnkVec3 delta = *TargetPos(ctx) - ctx->mPos;
    f32 distSq = delta.LengthSq();
    ctx->mPos += Scaled(Normalize(delta), step);
    if (stepSq <= distSq) {
        return;
    }
    while (stepSq > distSq) {
        ctx = m0;
        ctx->mPos = *TargetPos(ctx);
        step -= SqrtF(distSq);
        stepSq = step * step;
        fn_800D6A24(this);
        ctx = m0;
        if (ctx->mPoint != 0) {
            ctx->mPos = ctx->mPoint->m34;
            return;
        }
        delta = *TargetPos(ctx) - ctx->mPos;
        distSq = delta.LengthSq();
    }
    UnkVec3 move = Scaled(Normalize(delta), (double)step);
    m0->mPos += move;
}

Unk800D4C64* fn_800D5660(Follower* self) {
    if (self->m0->mPoint != 0) {
        return (Unk800D4C64*)self->m0->mPoint->mA4;
    }
    return 0;
}

u32 fn_800D5680(Follower* self) {
    if (self->m0->mPoint != 0) {
        return self->m0->mPoint->m1A8;
    }
    return 0;
}

u32 fn_800D56A0(Follower* self) {
    if (self->m0->mPoint != 0) {
        return self->m0->mPoint->m1A4;
    }
    return 0;
}

u32 fn_800D56C0(Follower* self) {
    if (self->m0->mPoint != 0) {
        return self->m0->mPoint->m58;
    }
    return 1;
}

int fn_800D56E0(Follower* self) {
    return self->m0->mPoint != 0;
}

u32 fn_800D56F8(Follower* self) {
    if (self->m0->mPoint != 0) {
        return self->m0->mPoint->m0;
    }
    return 0;
}

UnkVec3* fn_800D5718(Follower* self) {
    return &self->m0->mPos;
}

// 95.5 %: same base/count registers as fn_800D5174.
UnkVec3 fn_800D5724(Follower* self) {
    Unk800D50C4Ctx* ctx = self->m0;
    if (ctx->mPoint != 0) {
        return sUnkVec3;
    }
    return *TargetPos(ctx) - ctx->mPos;
}

int fn_800D5800(Follower* self) {
    int result = -1;
    if (self->m0->mPath != 0) {
        result = self->m0->mPath->m14;
    }
    return result;
}

u32 fn_800D581C(Follower* self) {
    u32 result = 0;
    if (self->m0->mPoint != 0) {
        result = self->m0->mPoint->mA0;
    }
    return result;
}

void fn_800D5838(Follower* self, u32 value) {
    if (self->m0->mPoint != 0) {
        self->m0->mPoint->mA0 = value;
    }
}

u32 fn_800D5850(Follower* self, int ignoreLocks) {
    Unk800D4C64* point = self->m0->mPoint;
    if (point == 0) {
        return 0;
    }
    return ExitMask(point, ignoreLocks);
}

u8 fn_800D5924(Follower* self) {
    if (self->m0->mPoint != 0) {
        return self->m0->mPoint->m9C;
    }
    return 1;
}

int fn_800D5944(Follower* self) {
    if (self->m0->mPoint != 0) {
        return self->m0->mPoint->m40;
    }
    return 1;
}

u32 fn_800D5964(Follower* self) {
    if (self->m0->mPoint != 0) {
        return self->m0->mPoint->m98;
    }
    return 0;
}

u32 fn_800D5984(Follower* self) {
    if (self->m0->mPoint != 0) {
        return self->m0->mPoint->m94;
    }
    return 0;
}

u32 fn_800D59A4(Follower* self, int i) {
    if (self->m0->mPoint != 0 && i < 3) {
        return self->m0->mPoint->m44[(u8)i];
    }
    return 0;
}

u32 fn_800D59D4(Follower* self) {
    if (self->m0->mPoint != 0) {
        return self->m0->mPoint->m60;
    }
    return 0;
}

// 97.2 %: the DOL computes &point->m50 after the `on` test, and has the case 4
// temporary below the case 0x10 one (case order and an inline helper tried).
void fn_800D59F4(Follower* self, u8 on) {
    if (self->m0->mPoint != 0) {
        self->m0->mPoint->m9C = on;
        Unk800D4C64* point = self->m0->mPoint;
        if (point != 0) {
            int* handle = &point->m50;
            if (on && point->m50 == 0) {
                switch (point->m40) {
                case 4:
                    lbl_8047C760.fn_801C80E0((void*)0x8BEBBC3D, &UnkVec4(point->m34, 0.0f), handle);
                    break;
                case 0x10:
                    lbl_8047C760.fn_801C80E0((void*)0xE8774AA7, &UnkVec4(point->m34, 0.0f), handle);
                    break;
                }
            } else if (!on && point->m50 != 0) {
                lbl_8047C760.fn_801C83AC(point->m50);
                self->m0->mPoint->m50 = 0;
            }
        }
    }
}

void fn_800D5B1C(Follower* self, void* type, const UnkVec3* pos) {
    if (self->m0->mPoint != 0) {
        if (self->m0->mPoint->m50 != 0) {
            lbl_8047C760.fn_801C83AC(self->m0->mPoint->m50);
            self->m0->mPoint->m50 = 0;
        }
        int* handle = &self->m0->mPoint->m50;
        lbl_8047C760.fn_801C80E0(type, &UnkVec4(*pos, 0.0f), handle);
    }
}

void fn_800D5BD4(Follower* self, u32 type) {
    Unk800D4C64* point = self->m0->mPoint;
    if (point != 0) {
        UnkHazard* hazard = (UnkHazard*)point->m50;
        if (hazard != 0 && type == hazard->m4->m0) {
            lbl_8047C760.fn_801C83AC((int)hazard);
            self->m0->mPoint->m50 = 0;
        }
    }
}

// 97.1 %: the DOL computes &point->m50 after the `on` test.
void fn_800D5C48(Follower* self, int on) {
    Unk800D4C64* point = self->m0->mPoint;
    if (point != 0) {
        int* handle = &point->m50;
        if (on && point->m50 == 0) {
            switch (point->m40) {
            case 4:
                lbl_8047C760.fn_801C80E0((void*)0x8BEBBC3D, &UnkVec4(point->m34, 0.0f), handle);
                break;
            case 0x10:
                lbl_8047C760.fn_801C80E0((void*)0xE8774AA7, &UnkVec4(point->m34, 0.0f), handle);
                break;
            }
        } else if (!on && point->m50 != 0) {
            lbl_8047C760.fn_801C83AC(point->m50);
            self->m0->mPoint->m50 = 0;
        }
    }
}

void fn_800D5D5C(Follower* self, int pause) {
    if (!pause && self->m0->mState == 1) {
        self->m0->mState = 0;
    } else if (pause && self->m0->mState == 0) {
        self->m0->mState = 1;
    }
}

void fn_800D5DA4(Follower* self, int stop) {
    if (stop) {
        self->m0->mState = 2;
    } else {
        self->m0->mState = 0;
    }
}

int fn_800D5DCC(Follower* self) {
    return self->m0->mState == 2;
}

u32 fn_800D5DE4(Follower* self, Unk800D4C64* point, int ignoreLocks) {
    if (point == 0) {
        return 0;
    }
    if (point->m64 != 0) {
        return 0;
    }
    u32 result = 0;
    u32 mask = ExitMask(point, ignoreLocks);
    for (int i = 0; i < 4; i++) {
        if (mask & (1 << i)) {
            Unk800D4C64* other = OtherEnd(point, i);
            if (other != 0) {
                if (other->m64 != 0 || other->m40 == 0x10) {
                    return other->m0;
                }
                if (!IsVisited(self, other)) {
                    Visit(self, other);
                    result = fn_800D5DE4(self, other, ignoreLocks);
                    if (result != 0) {
                        break;
                    }
                }
            }
        }
    }
    return result;
}

int fn_800D5FDC(Follower* self, Unk800D4C64* point, u32 targetId, int ignoreLocks) {
    if (point == 0) {
        return -1;
    }
    if (point->m0 == targetId) {
        return -1;
    }
    int result = -1;
    u32 mask = ExitMask(point, ignoreLocks);
    for (int i = 0; i < 4; i++) {
        if (mask & (1 << i)) {
            Unk800D4C64* other = OtherEnd(point, i);
            if (other != 0) {
                if (other->m0 == targetId) {
                    return i;
                }
                if (!IsVisited(self, other)) {
                    Visit(self, other);
                    result = fn_800D5FDC(self, other, targetId, ignoreLocks);
                    if (result != -1) {
                        return i;
                    }
                }
            }
        }
    }
    return result;
}

int fn_800D61D8(Follower* self, Unk800D4C64* point, u32 pathId, int ignoreLocks) {
    if (point == 0 || pathId == 0) {
        return -1;
    }
    int result = -1;
    u32 mask = ExitMask(point, ignoreLocks);
    for (int i = 0; i < 4; i++) {
        if (mask & (1 << i)) {
            Unk800D4C64* other = OtherEnd(point, i);
            if (other != 0) {
                if (pathId == point->mPaths[i]->m0) {
                    return i;
                }
                if (!IsVisited(self, other)) {
                    Visit(self, other);
                    result = fn_800D61D8(self, other, pathId, ignoreLocks);
                    if (result != -1) {
                        return i;
                    }
                }
            }
        }
    }
    return result;
}

u32 fn_800D63CC(Follower* self, int ignoreLocks) {
    Unk800D4C64* point = self->m0->mPoint;
    if (point == 0) {
        Unk800D5008* path = self->m0->mPath;
        if (path != 0) {
            if (self->m0->mForward) {
                point = path->m4[0];
            } else {
                point = path->m4[1];
            }
        }
    }
    if (point == 0) {
        return 0;
    }
    if (point->m64 != 0) {
        return 0;
    }
    memset(self->mVisited, 0, sizeof(self->mVisited));
    self->mVisited[0] = point;
    self->mVisitedCount = 1;
    return fn_800D5DE4(self, point, ignoreLocks);
}

// 97.2 %: the DOL loads m0 again for `point` after storing the visited count (store
// orders, inline reset helpers and an index store tried).
int fn_800D648C(Follower* self, u32 targetId, int ignoreLocks) {
    if (targetId == 0) {
        return -1;
    }
    memset(self->mVisited, 0, sizeof(self->mVisited));
    self->mVisitedCount = 1;
    self->mVisited[0] = self->m0->mPoint;
    Unk800D4C64* point = self->m0->mPoint;
    if (point != 0 && point->m0 != targetId) {
        return fn_800D5FDC(self, point, targetId, ignoreLocks);
    }
    return -1;
}

// The DOL's test only depends on mPoint: the mPath check cannot change the result
// (reproduced as written).
static inline bool IsOnPath(Unk800D50C4Ctx* ctx) {
    bool onPath = ctx->mPoint == 0;
    if (onPath && ctx->mPath != 0) {
        onPath = true;
    }
    return onPath;
}

// 99.2 %: the inlined fn_800D648C has the same m0 reload as above.
int fn_800D6534(Follower* self, Follower* other, int ignoreLocks) {
    if (other == 0) {
        return -1;
    }
    UnkVec3 delta = self->m0->mPos - other->m0->mPos;
    if (self->m0->mPoint != 0 && other != 0) {
        bool same = false;
        if (delta.x <= 0.0001f && delta.x >= -0.0001f && delta.y <= 0.0001f && delta.y >= -0.0001f &&
            delta.z <= 0.0001f && delta.z >= -0.0001f) {
            same = true;
        }
        if (!same) {
            if (other->m0->mPoint != 0) {
                return fn_800D648C(self, fn_800D56F8(other), ignoreLocks);
            }
            if (IsOnPath(other->m0)) {
                memset(self->mVisited, 0, sizeof(self->mVisited));
                self->mVisitedCount = 1;
                self->mVisited[0] = self->m0->mPoint;
                return fn_800D61D8(self, self->m0->mPoint,
                                   IsOnPath(other->m0) ? other->m0->mPath->m0 : 0, ignoreLocks);
            }
            return -1;
        }
    }
    return -1;
}

// 97.3 %: the DOL loads mPoint again after the exit mask (fn_800D5850 inlined gives
// a smaller function).
int fn_800D675C(Follower* self, int exit, int ignoreLocks) {
    exit = UnkClampT(exit, 0, 3);
    Unk800D50C4Ctx* ctx = self->m0;
    u32 mask;
    if (ctx->mPoint == 0) {
        mask = 0;
    } else {
        mask = ExitMask(ctx->mPoint, ignoreLocks);
    }
    if ((1 << exit) & mask) {
        Unk800D4C64* point = ctx->mPoint;
        Unk800D5008* path = point->mPaths[exit];
        ctx->mPath = path;
        if (point == path->m4[0]) {
            self->m0->mForward = 1;
            self->m0->mIndex = 0;
        } else {
            self->m0->mForward = 0;
            self->m0->mIndex = path->mCount - 1;
        }
        self->m0->mPoint = 0;
        return 1;
    }
    return 0;
}

// 97.8 %: exit, mask and bit registers numbered differently.
int fn_800D68CC(Follower* self, int exit, u32 lock) {
    exit = UnkClampT(exit, 0, 3);
    Unk800D4C64* point = self->m0->mPoint;
    u32 mask;
    if (point == 0) {
        mask = 0;
    } else {
        mask = ExitMask(point, 1);
    }
    if ((1 << exit) & mask) {
        point->mLocks[exit] = lock;
        return 1;
    }
    return 0;
}

// 97.9 %: the point and the other end point in swapped registers.
int fn_800D6998(Follower* self, int exit) {
    int result = 0;
    Unk800D4C64* point = self->m0->mPoint;
    if (point != 0) {
        Unk800D4C64* other = OtherEnd(point, exit);
        if (other != 0 && (other->m64 != 0 || other->m40 == 0x10)) {
            result = 1;
        }
    }
    return result;
}

void fn_800D6A00(Follower* self) {
    self->m0->mForward = !self->m0->mForward;
    fn_800D6A24(self);
}

void fn_800D6A18(Follower* self, f32 speed) {
    self->m0->mSpeed = speed;
}

void fn_800D6A24(Follower* self) {
    if (self->m0->mIndex == self->m0->mPath->mCount - 1 && self->m0->mForward == 1) {
        self->m0->mIndex = -4;
        return;
    }
    if (self->m0->mIndex == 0 && self->m0->mForward == 0) {
        self->m0->mIndex = -3;
        return;
    }
    if (self->m0->mIndex == -4) {
        self->m0->mIndex = -2;
        self->m0->mPoint = self->m0->mPath->m4[1];
        return;
    }
    if (self->m0->mIndex == -3) {
        self->m0->mIndex = -1;
        self->m0->mPoint = self->m0->mPath->m4[0];
        return;
    }
    if (self->m0->mForward) {
        self->m0->mIndex++;
    } else {
        self->m0->mIndex--;
    }
}
