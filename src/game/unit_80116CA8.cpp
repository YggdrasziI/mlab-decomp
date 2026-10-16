// Game unit at 0x80116CA8-0x80117090 (real file name unknown): a vertical progress
// bar drawn with three actors found by hash (top, bottom, bar; real name of the
// class unknown). The bar is scaled to the progress and moved between the top and
// bottom markers. Bounds come from the static initializers of this unit and the
// previous one.
// NonMatching: fn_80116D40 (97.6 %), fn_80116E54 (99.5 %) and fn_80116F08 (98.7 %)
// differ only by register numbering (mostly float registers).

#include "game/unk_actor.h"
#include "game/unk_vec3.h"

class UnkProgressBar {
public:
    UnkProgressBar();
    ~UnkProgressBar();
    void fn_80116D40(u32 topHash, u32 bottomHash);
    void fn_80116E54(int step, int total);
    void fn_80116EF0(int value);
    void fn_80116F08();

    /* 0x00 */ UnkVec3 mTop;
    /* 0x0C */ UnkVec3 mBottom;
    /* 0x18 */ u32 mBarHash;
    /* 0x1C */ int mMax;
    /* 0x20 */ int mValue;
    /* 0x24 */ f32 mBarHeight;
    /* 0x28 */ f32 mHeight;
};

extern "C" {
extern void* lbl_805FB654;
void* fn_8018D1E4(void* obj);
UnkActor* fn_80198F18(void* set, u32 hash);
}

static inline void SetScale(UnkActor* actor, UnkVec3 scale) {
    actor->m74 = scale;
}

UnkProgressBar::UnkProgressBar() {
    mBarHash = 0;
    mTop = UnkVec3Zero();
    mBottom = UnkVec3Zero();
    mMax = 0;
    mValue = 0;
    mBarHeight = 0.0f;
    mHeight = 0.0f;
}

UnkProgressBar::~UnkProgressBar() {}

void UnkProgressBar::fn_80116D40(u32 topHash, u32 bottomHash) {
    void* set = fn_8018D1E4(lbl_805FB654);
    if (set == 0) {
        return;
    }
    UnkActor* top = fn_80198F18(set, topHash);
    UnkActor* bottom = fn_80198F18(set, bottomHash);
    if (top != 0) {
        mTop = UnkVec3(top->mPos.x, top->mPos.y, top->mPos.z);
        mTop.y = top->m74.y * (1.0f - top->mA0) + top->mPos.y;
    }
    if (bottom != 0) {
        mBottom = UnkVec3(bottom->mPos.x, bottom->mPos.y, bottom->mPos.z);
        mBottom.y = bottom->mPos.y - bottom->mA0 * bottom->m74.y;
    }
    mHeight = mBottom.y - mTop.y;
}

void UnkProgressBar::fn_80116E54(int step, int total) {
    mBarHeight = mHeight;
    if (mHeight > 0.0f && total > 0 && total > step) {
        mMax = step * ((total - 1) / step);
        mBarHeight = mHeight * ((f32)step / (f32)total);
    } else {
        mMax = 0;
    }
    fn_80116F08();
}

void UnkProgressBar::fn_80116EF0(int value) {
    if (value > mMax) {
        return;
    }
    mValue = value;
    fn_80116F08();
}

void UnkProgressBar::fn_80116F08() {
    if (mBarHash == 0) {
        return;
    }
    void* set = fn_8018D1E4(lbl_805FB654);
    if (set == 0) {
        return;
    }
    UnkActor* bar = fn_80198F18(set, mBarHash);
    if (bar == 0) {
        return;
    }
    if (mMax > 0) {
        f32 offset = (mHeight - mBarHeight) * ((f32)mValue / (f32)mMax);
        UnkVec3 scale(bar->m74.x, mBarHeight, bar->m74.z);
        if (mValue < mMax) {
            bar->mPos.y = offset + mTop.y;
        } else {
            bar->mPos.y = mBottom.y - mBarHeight;
        }
        SetScale(bar, scale);
        bar->vf2C(0x18);
    } else {
        UnkVec3 scale(bar->m74.x, mHeight, bar->m74.z);
        bar->mPos.y = mTop.y;
        SetScale(bar, scale);
        bar->vf2C(0x18);
    }
}
