// Game unit at 0x8007A394-0x8007A680 (real file name unknown): the screen rectangle
// of a sprite-like object, computed from the size of its image (real names
// unknown). Bounds come from the static initializers of this unit and the previous
// one.
// NonMatching: fn_8007A3F4 (the DOL loads both floats of each pair before storing
// them), fn_8007A4B4 and fn_8007A598 (float register numbers) differ; the other
// functions match.

#include "game/unk_vec3.mch"
#include "game/unk_vec3.h"

// Image description at 0x4 in UnkImage (passed to fn_80079440).
struct UnkImageInfo {
    /* 0x0 */ u16 mWidth;
    /* 0x2 */ u16 mHeight;
};

struct UnkImage {
    /* 0x00 */ u32 m0;
    /* 0x04 */ UnkImageInfo mInfo;
    /* 0x08 */ u8 unk8[0x20 - 0x8];
    /* 0x20 */ u32 mFlags;
};

struct UnkFlagged {
    /* 0x0 */ u8 unk0[0x8];
    /* 0x8 */ u32 mFlags;
};

class UnkImageOwner {
public:
    virtual ~UnkImageOwner();
    virtual UnkFlagged* vf0C();

    /* 0x0 */ // vtable
    /* 0x4 */ UnkImage* m4;
};

struct UnkVec2 {
    f32 x, y;
};

// Scale from pixels to world units, defined in the previous unit (real name
// unknown).
extern const f32 lbl_805FD688;

extern "C" {
UnkImage* fn_80070400(UnkImageOwner* owner);
void fn_80079440(UnkImageInfo* info);
}

class UnkSpriteRect {
public:
    UnkSpriteRect();
    ~UnkSpriteRect();
    void fn_8007A3F4(UnkVec2* min, UnkVec2* max);
    void fn_8007A418(f32* width, f32* height);
    void fn_8007A42C(f32* scaleX, f32* scaleY);
    void fn_8007A440();
    void fn_8007A4B4(UnkImageOwner* owner);
    void fn_8007A598();

    /* 0x00 */ UnkImageOwner* mOwner;
    /* 0x04 */ UnkImage* mImage;
    /* 0x08 */ u32 m8;
    /* 0x0C */ UnkVec2 mMin;
    /* 0x14 */ UnkVec2 mMax;
    /* 0x1C */ f32 mWidth;
    /* 0x20 */ f32 mHeight;
    /* 0x24 */ f32 mScaleY;
    /* 0x28 */ f32 mScaleX;
};

// Never called, so not emitted; it only puts 0.0f first in the constant pool, as in
// the DOL (hypothesis: the original file had unused code using 0.0f here).
static f32 UnusedZero() {
    return 0.0f;
}

UnkSpriteRect::UnkSpriteRect() {
    mOwner = 0;
    mImage = 0;
    m8 = 0;
    mWidth = 80.0f;
    mHeight = 80.0f;
}

UnkSpriteRect::~UnkSpriteRect() {}

void UnkSpriteRect::fn_8007A3F4(UnkVec2* min, UnkVec2* max) {
    *min = mMin;
    *max = mMax;
}

void UnkSpriteRect::fn_8007A418(f32* width, f32* height) {
    *width = mWidth;
    *height = mHeight;
}

void UnkSpriteRect::fn_8007A42C(f32* scaleX, f32* scaleY) {
    *scaleX = mScaleX;
    *scaleY = mScaleY;
}

void UnkSpriteRect::fn_8007A440() {
    UnkImageOwner* owner = mOwner;
    if (owner->vf0C() != 0) {
        owner->vf0C()->mFlags &= ~2;
    }
    owner->m4->mFlags &= ~2;
}

void UnkSpriteRect::fn_8007A4B4(UnkImageOwner* owner) {
    mOwner = owner;
    mImage = fn_80070400(owner);
    UnkImageInfo* info = &mImage->mInfo;
    mScaleX = (s32)info->mWidth / mWidth;
    f32 halfWidth = 0.5f * mWidth;
    f32 halfHeight = 0.5f * mHeight;
    mScaleY = (s32)info->mHeight / mHeight;
    fn_80079440(info);
    mMax.x = halfWidth * lbl_805FD688;
    mMin.x = -halfWidth * lbl_805FD688;
    mMin.y = -halfHeight * lbl_805FD688;
    mMax.y = halfHeight * lbl_805FD688;
}

void UnkSpriteRect::fn_8007A598() {
    mWidth = 80.0f;
    mHeight = 80.0f;
    mImage = fn_80070400(mOwner);
    mScaleX = (s32)mImage->mInfo.mWidth / mWidth;
    f32 halfWidth = 0.5f * mWidth;
    f32 halfHeight = 0.5f * mHeight;
    f32 height = (s32)mImage->mInfo.mHeight;
    mMin.x = -halfWidth * lbl_805FD688;
    mMin.y = -halfHeight * lbl_805FD688;
    mMax.x = halfWidth * lbl_805FD688;
    mMax.y = halfHeight * lbl_805FD688;
    mScaleY = height / mHeight;
}
