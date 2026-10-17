// Game unit at 0x801604FC-0x80160BD4 (real file name unknown): a pool of 32 sound
// emitters addressed by handles (index in the low 5 bits, a generation counter above).
// The start is the end of the previous unit (its empty static initializer at
// 0x801604F8); the end is this unit's empty static initializer, which comes from the
// global pool object at 0x8040E260 and its empty inline constructor (hypothesis: this
// unit does not include the shared unk_vec3.h).

#include "types.h"

// Position of an emitter (real type unknown).
struct UnkEmitterPos {
    f32 x, y, z;
    UnkEmitterPos() {}
    UnkEmitterPos(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkEmitterPos(const UnkEmitterPos& other) : x(other.x), y(other.y), z(other.z) {}
    bool operator==(const UnkEmitterPos& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

// Parameters given to fn_8016195C (real type unknown).
struct UnkSoundParams {
    /* 0x0 */ f32 m0;
    /* 0x4 */ f32 m4;
    /* 0x8 */ f32 mVolume;
};

// Object at 0x8040E5F0 (real type unknown).
struct UnkSoundPlayer;

extern "C" {
extern UnkSoundPlayer lbl_8040E5F0;
void fn_8016195C(UnkSoundPlayer* player, int type, int arg, UnkSoundParams* params);
u32 fn_80143460();
}

// Returns whether the emitter can be heard, its two output values and its gain.
typedef int (*UnkEmitterCallback)(UnkEmitterPos* pos, UnkSoundParams* out, f32* gain);

// Emitter states: 0 free, 1 looping (updated every eighth frame unless moved), 2 one
// shot (freed after it plays).
struct UnkEmitter {
    /* 0x00 */ u32 mHandle;
    /* 0x04 */ s16 mState;
    /* 0x06 */ s16 mNext;
    /* 0x08 */ f32 mVolume;
    /* 0x0C */ s32 mDirty;
    /* 0x10 */ UnkEmitterPos mPos;
};

// Emitter pool (real name unknown).
class UnkEmitterPool {
public:
    UnkEmitterPool() {}

    u32 fn_801604FC();
    void fn_8016056C(u32 handle, const UnkEmitterPos* pos);
    void fn_8016060C(u32 handle, f32 volume);
    void fn_80160650(const UnkEmitterPos* pos, f32 volume);
    void fn_801606A4(UnkEmitter* emitter);
    void fn_80160808();
    void fn_801608C8();
    void fn_801609CC(u32 handle);
    void fn_80160A28();
    void fn_80160AFC();

    UnkEmitter* Alloc() {
        UnkEmitter* emitter;
        if (mFree < 0) {
            emitter = 0;
        } else {
            emitter = &mEmitters[mFree];
            mFree = emitter->mNext;
        }
        return emitter;
    }
    UnkEmitter* Find(u32 handle) {
        if (handle == 0) {
            return 0;
        }
        UnkEmitter* emitter = &mEmitters[handle & 0x1F];
        if (emitter->mHandle != handle) {
            return 0;
        }
        return emitter;
    }
    void Free(UnkEmitter* emitter) {
        emitter->mHandle += 0x20;
        emitter->mState = 0;
        emitter->mNext = mFree;
        mFree = emitter->mHandle & 0x1F;
    }

    /* 0x000 */ UnkEmitter mEmitters[32];
    /* 0x380 */ s32 mFree;
    /* 0x384 */ s32 mFrame;
    /* 0x388 */ UnkEmitterCallback mCallback;
};

UnkEmitterPool lbl_8040E260;

u32 UnkEmitterPool::fn_801604FC() {
    UnkEmitter* emitter = Alloc();
    if (emitter == 0) {
        return 0;
    }
    emitter->mState = 1;
    emitter->mVolume = 0.0f;
    UnkEmitterPos zero;
    zero.x = zero.y = zero.z = 0.0f;
    emitter->mPos = zero;
    emitter->mDirty = 1;
    return emitter->mHandle;
}

void UnkEmitterPool::fn_8016056C(u32 handle, const UnkEmitterPos* pos) {
    UnkEmitter* emitter = Find(handle);
    if (emitter == 0) {
        return;
    }
    if (emitter->mState != 1) {
        return;
    }
    if (emitter->mPos == *pos) {
        return;
    }
    emitter->mPos = *pos;
    emitter->mDirty = 1;
}

void UnkEmitterPool::fn_8016060C(u32 handle, f32 volume) {
    UnkEmitter* emitter = Find(handle);
    if (emitter == 0) {
        return;
    }
    if (emitter->mState != 1) {
        return;
    }
    emitter->mVolume = volume;
}

void UnkEmitterPool::fn_80160650(const UnkEmitterPos* pos, f32 volume) {
    UnkEmitter* emitter = Alloc();
    if (emitter == 0) {
        return;
    }
    emitter->mState = 2;
    emitter->mVolume = volume;
    emitter->mPos = *pos;
}

void UnkEmitterPool::fn_801606A4(UnkEmitter* emitter) {
    if (mCallback == 0) {
        return;
    }
    if (emitter->mState == 1 && emitter->mDirty == 0) {
        if (mFrame % 8 != (s32)emitter->mHandle % 8) {
            return;
        }
    }
    emitter->mDirty = 0;
    UnkSoundParams out;
    UnkSoundParams params;
    UnkEmitterPos pos = emitter->mPos;
    f32 gain;
    if (!mCallback(&pos, &out, &gain)) {
        return;
    }
    f32 out0 = out.m0;
    f32 out4 = out.m4;
    params.m0 = out0;
    params.m4 = out4;
    f32 volume = 0.001f * (gain * emitter->mVolume);
    if (emitter->mState == 1) {
        f32 r = (f32)fn_80143460() * 2.3283064e-10f;
        volume *= 2.0f * (r - 0.5f);
    }
    params.mVolume = volume;
    fn_8016195C(&lbl_8040E5F0, 9, 0, &params);
    emitter->mDirty = 1;
}

void UnkEmitterPool::fn_80160808() {
    for (int i = 0; i < 32; i++) {
        UnkEmitter* emitter = &mEmitters[i];
        switch (emitter->mState) {
        case 1:
            fn_801606A4(emitter);
            break;
        case 2:
            fn_801606A4(emitter);
            Free(emitter);
            break;
        }
    }
    mFrame++;
}

void UnkEmitterPool::fn_801608C8() {
    for (int i = 0; i < 32; i++) {
        mEmitters[i].mHandle = i | 0x20;
        mEmitters[i].mNext = i + 1;
        mEmitters[i].mState = 0;
    }
    mEmitters[31].mNext = -1;
    mFree = 0;
    mFrame = 0;
    mCallback = 0;
}

void UnkEmitterPool::fn_801609CC(u32 handle) {
    UnkEmitter* emitter = Find(handle);
    if (emitter == 0) {
        return;
    }
    Free(emitter);
}

void UnkEmitterPool::fn_80160A28() {
    for (int i = 0; i < 32; i++) {
        if (mEmitters[i].mState != 0) {
            Free(&mEmitters[i]);
        }
    }
}

void UnkEmitterPool::fn_80160AFC() {
    fn_80160A28();
}
