// Game unit at 0x8003CA78-0x8003CF40 (real file name unknown): a flicker effect over
// four "Mob%d_%d_%d" scene actors and a "Flicker" node. The start is the end of the
// previous unit (its static initializer at 0x8003CA58); the end is this unit's static
// initializer. The unit at 0x80039CF8 builds arrays of this class with
// __construct_array.

#include "game/unk_vec3.mch"
#include "game/unk_actor.h"
#include "game/externs.h"
#include "game/unk_vec3.h"

// Flicker state over four actors (real name unknown). mState is 1/2 while the first
// two actors alternate, 4/8 while the last two do.
class UnkFlicker {
public:
    UnkFlicker();
    ~UnkFlicker();
    void fn_8003CA78();
    bool fn_8003CAE0();
    u8 fn_8003CB08();
    void fn_8003CB1C();
    void fn_8003CBB0(f32 dt);
    void fn_8003CE20(int a, int b);
    UnkVec3* fn_8003CF14();

    /* 0x00 */ u8 mDone;
    /* 0x04 */ f32 mTime;
    /* 0x08 */ UnkActor* mActors[4];
    /* 0x18 */ void* mNode;
    /* 0x1C */ s32 mState;
    /* 0x20 */ f32 m20;
    /* 0x24 */ f32 m24;
    /* 0x28 */ f32 m28;
    /* 0x2C */ u8 mFading;
    /* 0x2D */ u8 m2D;
    /* 0x2E */ u8 mFaded;
    /* 0x30 */ f32 mFadeTime;
};

extern "C" {
extern void* lbl_805FB654;

void fn_8018F618(void* obj, int visible);
void* fn_8018D1E4(void* obj);
UnkActor* fn_80198F18(void* scene, u32 hash);
void* fn_80198F1C(void* scene, u32 hash);
u32 fn_801EB704(const char* text);
u32 fn_80143460();
void fn_8019BBA4(UnkActor* actor, void* obj, int arg);
}

void UnkFlicker::fn_8003CA78() {
    fn_8018F618(mActors[0]->m0, 0);
    fn_8018F618(mActors[1]->m0, 0);
    fn_8018F618(mActors[2]->m0, 1);
    mState = 4;
    mTime = 0.0f;
}

bool UnkFlicker::fn_8003CAE0() {
    return (mState & 0xC) || mFading;
}

u8 UnkFlicker::fn_8003CB08() {
    u8 done = mDone;
    mDone = 0;
    return done;
}

void UnkFlicker::fn_8003CB1C() {
    mFading = 1;
    mTime = 0.0f;
    mState = 1;
    fn_8018F618(mActors[0]->m0, 1);
    fn_8018F618(mActors[1]->m0, 0);
    fn_8018F618(mActors[2]->m0, 0);
    fn_8018F618(mActors[3]->m0, 0);
    if (mNode) {
        fn_8019BBA4(mActors[0], mNode, 10);
    }
}

void UnkFlicker::fn_8003CBB0(f32 dt) {
    mTime += dt;
    f32 limit;
    if (mState & 3) {
        limit = m20;
    } else if (mState == 4) {
        limit = m24;
    } else {
        limit = m28;
    }
    if (mFading) {
        if (mTime >= mFadeTime) {
            mActors[0]->vf30(2);
            mFaded = 1;
        } else {
            mActors[0]->m8C = 1.0f - mTime / mFadeTime;
            mActors[0]->vf2C(0x20);
        }
    } else if (mTime > limit) {
        mTime -= limit;
        switch (mState) {
        case 1:
            fn_8018F618(mActors[0]->m0, 0);
            fn_8018F618(mActors[1]->m0, 1);
            mState = 2;
            break;
        case 2:
            fn_8018F618(mActors[1]->m0, 0);
            fn_8018F618(mActors[0]->m0, 1);
            mState = 1;
            break;
        case 4:
            fn_8018F618(mActors[2]->m0, 0);
            fn_8018F618(mActors[3]->m0, 1);
            mState = 8;
            mDone = 1;
            break;
        case 8:
            fn_8018F618(mActors[3]->m0, 0);
            fn_8018F618(mActors[0]->m0, 1);
            mState = 1;
            break;
        }
    }
}

UnkFlicker::UnkFlicker()
    : mDone(0), mTime(0.0f), mNode(0), mState(1), m20(0.35f), m24(0.35f), m28(0.15f), mFading(0), m2D(0),
      mFaded(0), mFadeTime(1.0f) {
    m20 = 0.25f;
    m24 = 0.5f;
    m28 = 0.15f;
    mFadeTime = 0.4f;
}

UnkFlicker::~UnkFlicker() {}

void UnkFlicker::fn_8003CE20(int a, int b) {
    void* scene = fn_8018D1E4(lbl_805FB654);
    for (int i = 0; i < 4; i++) {
        char name[0x18];
        fn_80142EF8(name, "Mob%d_%d_%d", a, i, b);
        mActors[i] = fn_80198F18(scene, fn_801EB704(name));
    }
    mNode = fn_80198F1C(scene, fn_801EB704("Flicker"));
    mTime = (f32)(fn_80143460() % (int)(100.0f * m20)) / 100.0f;
}

UnkVec3* UnkFlicker::fn_8003CF14() {
    return &mActors[0]->mPos;
}
