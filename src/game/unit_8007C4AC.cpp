// Game unit at 0x8007C4AC-0x8007CED8 (real file name unknown): an image whose rows
// are revealed as a coordinate crosses the bounding box of an entity, with one or two
// entities moved together and shown or hidden by mode (real names unknown). Bounds
// come from the static initializers of this unit and the previous one; only the
// unit at 0x80077D1C (not split) calls these functions.

#include "game/unk_vec3.mch"
#include "game/Entity.h"
#include "game/unk_vec3.h"

// Image description at 0x4 in UnkImage (same layout as in game/unit_8007A394.cpp).
struct UnkImageInfo {
    /* 0x0 */ u16 mWidth;
    /* 0x2 */ u16 mHeight;
};

struct UnkImage {
    /* 0x00 */ u32 m0;
    /* 0x04 */ UnkImageInfo mInfo;
};

// Object returned by Entity::vf0C (flag 2 hides it, as in the other units).
struct UnkEntityState {
    /* 0x0 */ u8 unk0[0x8];
    /* 0x8 */ u32 mFlags;
};

struct UnkBox {
    /* 0x00 */ u8 unk0[0x30];
    /* 0x30 */ UnkVec3 mMin;
    /* 0x3C */ UnkVec3 mMax;
};

struct UnkBoxOwner {
    /* 0x00 */ u8 unk0[0x10];
    /* 0x10 */ UnkBox* mBox;
};

struct UnkVec2 {
    f32 x, y;
};

extern "C" {
UnkImage* fn_80070400(Entity* entity);
int fn_80079440(UnkImageInfo* info);
void fn_8007D004(void* dest, UnkImageInfo* info, int row, int count);
u8* fn_8015A210(UnkImageInfo* info, int arg1, int* pitch, int arg3);
void fn_8015A2E8(UnkImageInfo* info, int arg1, int arg2);
void fn_801B4EA4(Entity* entity, int arg);
UnkBoxOwner* fn_801DA758(void* state);
double floor(double x);
}

static inline void HideEntity(Entity* entity) {
    if (entity->vf0C() != 0) {
        ((UnkEntityState*)entity->vf0C())->mFlags |= 2;
    }
    entity->mInfo->mFlags |= 2;
}

static inline void ShowEntity(Entity* entity) {
    if (entity->vf0C() != 0) {
        ((UnkEntityState*)entity->vf0C())->mFlags &= ~2;
    }
    entity->mInfo->mFlags &= ~2;
}

class UnkImageReveal {
public:
    UnkImageReveal();
    ~UnkImageReveal();
    void fn_8007C50C();
    void fn_8007C5B4(UnkVec2* min, UnkVec2* max);
    EntityInfo* fn_8007C5D8();
    void fn_8007C5E4(int* width, int* height);
    int fn_8007C5FC(Entity* entity, Entity* extra, UnkWire* wire, const UnkVec3& extraOffset,
                    const UnkVec3& offset);
    UnkVec3 fn_8007C8CC(const UnkVec3& pos);
    void fn_8007C958(const UnkVec3& pos);
    void fn_8007CB68(int mode);
    int fn_8007CDBC(void* dest, f32 x);

    // Inline: its locals take the stack slots below the temporaries of
    // fn_8007C958, as in the DOL.
    void UpdateBounds() {
        UnkBox* box = fn_801DA758(mEntity->vf0C())->mBox;
        UnkVec3 min = box->mMin;
        UnkVec3 max = box->mMax;
        mMin.x = min.x;
        mMin.y = min.z;
        mMax.x = max.x;
        mMax.y = max.z;
        UnkVec3 entityPos = mEntity->mInfo->mPos;
        mMin.x += entityPos.x;
        mMin.y += entityPos.z;
        mMax.x += entityPos.x;
        mMax.y += entityPos.z;
    }

    // Shows or hides the two entities; inlined in fn_8007C5FC, called through
    // fn_8007CB68 elsewhere.
    void ApplyMode(int mode) {
        switch (mode) {
        case 0:
            if (mExtra != 0) {
                HideEntity(mExtra);
            }
            ShowEntity(mEntity);
            break;
        case 1:
            if (mExtra != 0) {
                ShowEntity(mExtra);
            }
            HideEntity(mEntity);
            break;
        case 2:
            if (mExtra != 0) {
                ShowEntity(mExtra);
            }
            ShowEntity(mEntity);
            break;
        }
    }

    /* 0x00 */ Entity* mExtra;
    /* 0x04 */ Entity* mEntity;
    /* 0x08 */ UnkWire* mWire;
    /* 0x0C */ UnkVec3 mExtraOffset;
    /* 0x18 */ UnkVec3 mOffset;
    /* 0x24 */ UnkImage* mImage;
    /* 0x28 */ UnkVec2 mMin;
    /* 0x30 */ UnkVec2 mMax;
    /* 0x38 */ int mRow;
    /* 0x3C */ u8 m3C;
};

UnkImageReveal::UnkImageReveal() {
    mExtra = 0;
    mEntity = 0;
    mWire = 0;
    mImage = 0;
    mRow = 0;
    m3C = 0;
}

UnkImageReveal::~UnkImageReveal() {}

void UnkImageReveal::fn_8007C50C() {
    int pitch;
    u8* pixels = fn_8015A210(&mImage->mInfo, 0, &pitch, 0);
    for (int y = 0; y < mImage->mInfo.mHeight; y++) {
        u8* row = pixels + y * pitch;
        for (int x = 0; x < mImage->mInfo.mWidth / 2; x++) {
            row[x] &= 0xDD;
        }
    }
    fn_8015A2E8(&mImage->mInfo, 0, 1);
}

// minY and maxY are declared before minX and maxX for the DOL's register numbers.
void UnkImageReveal::fn_8007C5B4(UnkVec2* min, UnkVec2* max) {
    f32 minY;
    f32 minX = mMin.x;
    minY = mMin.y;
    min->x = minX;
    f32 maxY;
    f32 maxX = mMax.x;
    min->y = minY;
    maxY = mMax.y;
    max->x = maxX;
    max->y = maxY;
}

EntityInfo* UnkImageReveal::fn_8007C5D8() {
    return mEntity->mInfo;
}

void UnkImageReveal::fn_8007C5E4(int* width, int* height) {
    UnkImageInfo* info = &mImage->mInfo;
    *width = info->mWidth;
    *height = info->mHeight;
}

int UnkImageReveal::fn_8007C5FC(Entity* entity, Entity* extra, UnkWire* wire,
                                const UnkVec3& extraOffset, const UnkVec3& offset) {
    mWire = wire;
    mEntity = entity;
    mExtra = extra;
    mExtraOffset = extraOffset;
    mOffset = offset;
    int mode = 0;
    if (extra == 0) {
        mode = 1;
    }
    mImage = fn_80070400(mEntity);
    int result = fn_80079440(&mImage->mInfo);
    // The DOL normalizes the mode to 0 or 1 here (neg/or/srwi).
    ApplyMode((bool)mode);
    return result;
}

UnkVec3 UnkImageReveal::fn_8007C8CC(const UnkVec3& pos) {
    UnkWirePoint* points = mWire->mPoints;
    UnkVec3 first = points[0].mPos;
    UnkVec3 delta = pos - first;
    UnkVec3 result = points[mWire->mPointCount - 1].mPos;
    result += delta;
    return result;
}

void UnkImageReveal::fn_8007C958(const UnkVec3& pos) {
    UnkVec3 p;
    Entity* extra = mExtra;
    if (extra != 0) {
        p = pos + mExtraOffset;
        extra->SetPosition(p);
        fn_801B4EA4(mExtra, 0);
    }
    Entity* entity = mEntity;
    p = pos + mOffset;
    entity->SetPosition(p);
    fn_801B4EA4(mEntity, 0);
    UpdateBounds();
}

void UnkImageReveal::fn_8007CB68(int mode) {
    ApplyMode(mode);
}

int UnkImageReveal::fn_8007CDBC(void* dest, f32 x) {
    if (mMax.x < x) {
        return 0;
    }
    UnkImageInfo* info = &mImage->mInfo;
    int height = info->mHeight;
    if (mMin.x > x) {
        if (mRow < height) {
            fn_8007D004(dest, info, mRow, height - mRow);
            mRow = height;
            return 1;
        }
        return 0;
    }
    f32 range = mMin.x - mMax.x;
    f32 t = x - mMax.x;
    t /= range;
    int row = (f32)floor(t * height);
    fn_8007D004(dest, info, mRow, row - mRow);
    mRow = row;
    return 0;
}
