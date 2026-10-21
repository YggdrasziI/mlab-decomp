// Game unit at 0x800D3428-0x800D4C64 (real file name unknown): Exploration::PathAPI
// (class name from RTTI), the singleton that keeps the path points, the paths and
// the links of a level and hands out path objects (Unk800D50C4) from a pool. The
// start is the end of the static initializer at 0x800D33E0 (previous unit), the end
// is this unit's static initializer at 0x800D4C44. Types in game/PathAPI.h.
// The descending loops test `i > -1` (the DOL's cmpwi -1); `i >= 0` compiles to
// another compare.

#include "game/unk_vec3.mch"
#include "game/Entity.h"
#include "game/PathAPI.h"
#include "game/unk_vec3.h"

using namespace Exploration;

extern "C" {
void fn_80142EF8(char* buf, const char* fmt, ...);
u32 fn_80143460();
// Name of an id (probably a hash lookup).
const char* fn_801EB708(u32 id);

Unk800D50C4* fn_800D39BC(PathAPI* self, u32 id);
void fn_800D3D50(PathAPI* self);
void fn_800D3D6C(PathAPI* self);
void fn_800D41F4(PathAPI* self, const Unk800D5008* path);
void fn_800D424C(PathAPI* self, const Unk800D4C64* point);
}

PathAPI* lbl_805FB180;

// The DOL repeats the body of fn_800D36A0 here: that function comes later in the
// file, and a shared inline helper gives fn_800D36A0 other registers.
void PathAPI::fn_800D3428() {
    if (lbl_805FB180 != 0) {
        for (int i = lbl_805FB180->mPoints.Count() - 1; i > -1; i--) {
            if (lbl_805FB180->mPoints[i].m50 != 0) {
                lbl_8047C760.fn_801C83AC(lbl_805FB180->mPoints[i].m50);
                lbl_805FB180->mPoints[i].m50 = 0;
            }
        }
        delete lbl_805FB180;
    }
    lbl_805FB180 = 0;
    lbl_805FB180 = new PathAPI;
}

void PathAPI::fn_800D36A0() {
    if (lbl_805FB180 != 0) {
        for (int i = lbl_805FB180->mPoints.Count() - 1; i > -1; i--) {
            if (lbl_805FB180->mPoints[i].m50 != 0) {
                lbl_8047C760.fn_801C83AC(lbl_805FB180->mPoints[i].m50);
                lbl_805FB180->mPoints[i].m50 = 0;
            }
        }
        delete lbl_805FB180;
    }
    lbl_805FB180 = 0;
}

void PathAPI::fn_800D3818(f32 dt) {
    int i = -1;
    while (mPool.Next(i)) {
        mPool[i].mObj.fn_800D5174(dt);
    }
}

int PathAPI::fn_800D3930() {
    return mPoints.Count();
}

Unk800D50C4* PathAPI::fn_800D393C(int i) {
    mObj.fn_800D5110(&mCtx, &mPoints[i]);
    return &mObj;
}

Unk800D50C4* fn_800D39BC(PathAPI* self, u32 id) {
    int i;
    for (i = self->mPoints.Count() - 1; i > -1; i--) {
        if (id == self->mPoints[i].m0) {
            break;
        }
    }
    if (i >= 0) {
        self->mObj.fn_800D5110(&self->mCtx, &self->mPoints[i]);
    }
    return &self->mObj;
}

Unk800D50C4* PathAPI::fn_800D3AC4(const UnkVec3& pos) {
    Unk800D4C64* closest = 0;
    f32 closestDist = 100000.0f;
    for (int i = mPoints.Count() - 1; i > -1; i--) {
        f32 dist = (mPoints[i].m34 - pos).LengthSq();
        if (dist < closestDist) {
            closest = &mPoints[i];
            closestDist = dist;
        }
    }
    if (closest != 0) {
        Unk800D4BB4* entry = mPool.Alloc();
        if (entry != 0) {
            entry->mObj.fn_800D5110(&entry->mCtx, closest);
            return &entry->mObj;
        }
        return 0;
    }
    return 0;
}

void PathAPI::fn_800D3C60(Unk800D50C4* obj) {
    int i = -1;
    while (mPool.Next(i)) {
        if (obj == &mPool[i].mObj) {
            mPool.Free(i);
            return;
        }
    }
}

void fn_800D3D50(PathAPI* self) {
    self->mPaths.Clear();
    self->mPoints.Clear();
    self->mLinks.Clear();
}

void fn_800D3D6C(PathAPI* self) {
    for (int i = self->mPoints.Count() - 1; i > -1; i--) {
        for (int k = 0; k < 4; k++) {
            if (self->mPoints[i].m4[1][k] == 0) {
                self->mPoints[i].m4[0][k] = 0;
            } else {
                for (int j = self->mPaths.Count() - 1; j > -1; j--) {
                    if (self->mPoints[i].m4[1][k] == self->mPaths[j].m0) {
                        self->mPoints[i].m4[0][k] = (u32)&self->mPaths[j];
                        for (int n = 0; n < 2; n++) {
                            if (self->mPaths[j].mC[n] == self->mPoints[i].m0) {
                                self->mPaths[j].m4[n] = &self->mPoints[i];
                                break;
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
    for (int j = self->mPaths.Count() - 1; j > -1; j--) {
        if (self->mPaths[j].m4[0] == 0 || self->mPaths[j].m4[1] == 0) {
            char buf[256];
            fn_80142EF8(buf, "Path %s not fully connected\n", fn_801EB708(self->mPaths[j].m0));
        }
    }
    self->mObj.fn_800D5110(&self->mCtx, self->mPoints.mData);
}

void fn_800D41F4(PathAPI* self, const Unk800D5008* path) {
    self->mPaths.Add(*path);
}

void fn_800D424C(PathAPI* self, const Unk800D4C64* point) {
    self->mPoints.Add(*point);
    for (int i = 0; i < point->m90; i++) {
        self->fn_800D42F4(point->m68[i], point->m0);
    }
}

void PathAPI::fn_800D42F4(int key, u32 value) {
    int found = -1;
    for (int i = mLinks.Count() - 1; i > -1; i--) {
        if (key == mLinks[i].mKey) {
            found = i;
            break;
        }
    }
    if (found >= 0) {
        mLinks[found].mValues[mLinks[found].mCount] = value;
        mLinks[found].mCount++;
    } else {
        UnkPathLink link;
        link.mKey = key;
        link.mValues[0] = value;
        link.mCount = 1;
        mLinks.Add(link);
    }
}

PathAPI::~PathAPI() {}

u32 PathAPI::fn_800D4588(const UnkVec3& pos) {
    Unk800D4C64* closest = 0;
    f32 closestDist = 100000.0f;
    for (int i = mPoints.Count() - 1; i > -1; i--) {
        f32 dist = (mPoints[i].m34 - pos).LengthSq();
        if (dist < closestDist) {
            closest = &mPoints[i];
            closestDist = dist;
        }
    }
    if (closest != 0) {
        return closest->m0;
    }
    return 0;
}

void PathAPI::fn_800D46B0() {
    for (int i = mPoints.Count() - 1; i > -1; i--) {
        mPoints[i].mA0 = 0;
    }
}

u32 PathAPI::fn_800D4730(int key) {
    u32 result = 0;
    for (int i = mLinks.Count() - 1; i > -1; i--) {
        if (key == mLinks[i].mKey) {
            UnkPathLink& link = mLinks[i];
            int index = (int)(fn_80143460() * 2.3283064e-10f) % link.mCount;
            result = mLinks[i].mValues[index];
            int j;
            for (j = mPoints.Count() - 1; j > -1; j--) {
                if (result == mPoints[j].m0) {
                    break;
                }
            }
            if (mPoints[j].m54 != 0) {
                result = 0;
                for (int n = 0; n < mLinks[i].mCount; n++) {
                    index += 1 % mLinks[i].mCount;
                    result = mLinks[i].mValues[index];
                    for (j = mPoints.Count() - 1; j > -1; j--) {
                        if (result == mPoints[j].m0) {
                            break;
                        }
                    }
                    if (mPoints[j].m54 == 0) {
                        break;
                    }
                    result = 0;
                }
            }
            break;
        }
    }
    return result;
}

// Out of line at the end of the file: the DOL has these four functions after the
// last PathAPI function, in this order (generated copies would follow their first
// user).
Unk800D5008::Unk800D5008() {
    mCount = 0;
}

Unk800D5008::~Unk800D5008() {}

Unk800D4BB4::Unk800D4BB4() {}

Unk800D4BB4::~Unk800D4BB4() {}
