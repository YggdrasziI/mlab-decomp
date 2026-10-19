// Game unit at 0x80030604-0x8003107C (real file name unknown): the SewerItem
// subclasses SewerBottle, SewerMetal, SewerRing, SewerBubble and SewerFish (RTTI).
// Bounds come from the static initializers of this unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/SewerItem.h"
#include "game/unk_803C4298.h"

extern "C" {
u32 fn_80143460(void);
u32 fn_80194944(u32 hash, int index);
void fn_80191B68(void* obj, u32 sound);
void fn_801903CC(void* obj, u32 hash);
int fn_80209E70(u32 hash, int arg1, int arg2, int arg3, int arg4, int arg5);
int fn_8020A9A8(int handle);
void fn_8020A398(int handle);
void fn_801B2C08(void* obj, int arg, f32 value);
void* fn_8018D1E4(void* obj);
void* fn_80198F1C(void* obj, u32 hash);
void fn_801AB80C(void* obj, f32 value);
void fn_8019BBA4(UnkActor* actor, void* obj, int arg);
extern void* lbl_805FB654;
}

SewerBottle::SewerBottle() {
    mType = 4;
}

void SewerBottle::vf18() {
    m24 = (m24 + 1) % 4;
    u32 sound = fn_80194944(0x81F6E981, m24);
    if (sound != 0) {
        fn_80191B68(mActor->m0, sound);
    }
}

SewerMetal::SewerMetal() {
    mType = 2;
}

void SewerMetal::vf18() {
    m24 = (m24 + 1) % 4;
    u32 sound = fn_80194944(0xACCFADBF, m24);
    if (sound != 0) {
        fn_80191B68(mActor->m0, sound);
    }
}

SewerRing::SewerRing() {
    mType = 3;
}

void SewerRing::vf18() {
    m24 = (m24 + 1) % 4;
    u32 sound = fn_80194944(0xE3B53643, m24);
    if (sound != 0) {
        fn_80191B68(mActor->m0, sound);
    }
}

SewerBubble::SewerBubble() {
    mType = 5;
    m38 = 0;
}

void SewerBubble::SetM38(int value) {
    m38 = value;
}

void SewerBubble::vf18() {
    if (++m24 >= 7) {
        if (m38 != 0) {
            m24 = 0;
        } else {
            mActor->mPos.z = 9.0f;
            vf14();
            return;
        }
    }
    u32 sound = fn_80194944(0x77A5B362, m24);
    if (sound != 0) {
        fn_80191B68(mActor->m0, sound);
    }
}

void SewerBubble::vf1C(f32 arg) {
    m24 = 0;
    mActor->mPos.z = mHome.z;
    mInterval = 4.0f;
    SewerItem::vf1C(arg);
}

SewerFish::SewerFish() {
    m3C = 1;
    mSound = 0;
    mType = 1;
    mState = 0;
    mRadiusSq = 4096.0f;
}

int SewerFish::vf0C(UnkVec3 pos) {
    if (mState != 0) {
        return 0;
    }
    int hit = 0;
    int ahead = 0;
    UnkVec3 delta = pos - mActor->mPos;
    if (mActive && delta.x > 0.0f) {
        ahead = 1;
    }
    if (ahead && delta.x < 10.0f) {
        hit = 1;
    }
    if (hit) {
        mState = 1;
    }
    return hit;
}

// NonMatching (99.7 %): the DOL keeps mState in r4 for the compare chain, this
// build uses r0 (if chains, switches, early returns and local types tried).
void SewerFish::vf10() {
    SewerItem::vf10();
    int state = mState;
    if (state == 0) {
        return;
    }
    if (state == 3) {
        if (m3C != 0) {
            fn_801B2C08(lbl_803C4298.m38, 4, 0.2f);
        }
        if (mSound <= 0 || fn_8020A9A8(mSound) == 0) {
            mSound = fn_80209E70(0xBE77A95E, 0, 0, 0, 0, 0);
        }
    } else if (state == 1) {
        mState = 2;
        mActor->mPos.z = 6.0f;
        mActor->vf2C(0x18);
        fn_801903CC(mActor->m0, 0xD3601402);
        fn_80209E70(0x741322E8, 0, 0, 0, 0, 0);
    } else if (state == 2) {
        if (mActor->m10 == 0) {
            mState = 3;
            mInterval = 1.0f;
            mSound = fn_80209E70(0xBE77A95E, 0, 0, 0, 0, 0);
        }
    } else if (state == 4) {
        mState = 5;
        mInterval = fn_80143460() % 5 + 8;
        void* obj = fn_80198F1C(fn_8018D1E4(lbl_805FB654), 0x1FC61193);
        if (obj != 0) {
            fn_801AB80C(obj, 1.25f);
            fn_8019BBA4(mActor, obj, 10);
        }
        if (mSound > 0) {
            fn_8020A398(mSound);
        }
    } else if (state == 5) {
        if (mActor->m10 == 0) {
            vf14();
        }
    }
}

SewerItem* SewerFish::vf14() {
    if (mState == 3) {
        mState = 4;
    } else if (mState == 0 || mState == 5) {
        return SewerItem::vf14();
    }
    return mNext;
}

void SewerFish::vf18() {
    m24 = (m24 + 1) % 4;
    u32 sound = 0;
    if (mState == 0) {
        sound = fn_80194944(0x4A95BAA7, m24);
    } else if (mState == 2) {
        sound = fn_80194944(0x33C872C9, 0);
    } else if (mState == 3) {
        sound = fn_80194944(0x20800DC8, m24);
    } else if (mState == 5) {
        sound = fn_80194944(0x20800DC8, 1);
    }
    if (sound != 0) {
        fn_80191B68(mActor->m0, sound);
    }
}

void SewerFish::vf1C(f32 arg) {
    SewerItem::vf1C(arg);
    mState = 0;
    UnkVec3 scale(128.0f, 128.0f, 0.0f);
    mActor->m74 = scale;
}
