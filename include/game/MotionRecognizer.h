#ifndef GAME_MOTIONRECOGNIZER_H
#define GAME_MOTIONRECOGNIZER_H

// Wii Remote motion recognizers (names from RTTI): ControllerAction ->
// MotionRecognizer -> AccelerometerMotionRecognizer (functions at 0x801F8238) ->
// DirectionalSpikeRecognizer (0x801F94B0), and more. They are polymorphic (vtable at
// 0x0); only the functions and fields used by decompiled code are declared, without
// the virtual functions. Recognizers are registered with the manager returned by
// fn_801B3308. Method and field names unknown.

#include "types.h"

// Three-byte argument of AccelerometerMotionRecognizer::fn_801F8628 (real type
// unknown).
struct UnkMotionParam {
    UnkMotionParam(s8 a, s8 b, s8 c) : m0(a), m1(b), m2(c) {}

    s8 m0;
    s8 m1;
    s8 m2;
};

class AccelerometerMotionRecognizer {
public:
    AccelerometerMotionRecognizer(void* resource);
    ~AccelerometerMotionRecognizer();
    void fn_801F8628(UnkMotionParam* param, int arg);
    void fn_801F8F34(f32 value);
    void fn_801F8FA0(f32 value);

    u8 Update() {
        if (m90 && m94 > 0.0f) {
            m8 = m94;
        }
        return m90;
    }

    /* 0x00 */ u8 unk0[0x8];
    /* 0x08 */ f32 m8;
    /* 0x0C */ u8 unkC[0x90 - 0xC];
    /* 0x90 */ u8 m90;
    /* 0x94 */ f32 m94;
    /* 0x98 */ u32 m98;
    /* 0x9C */ u8 unk9C[0x484 - 0x9C];
};

class DirectionalSpikeRecognizer : public AccelerometerMotionRecognizer {
public:
    DirectionalSpikeRecognizer(void* resource);
    ~DirectionalSpikeRecognizer();
};

class UnkMotionManager {
public:
    void fn_801B3310(AccelerometerMotionRecognizer* player);
    void fn_801B3378(AccelerometerMotionRecognizer* player);
};

UnkMotionManager* fn_801B3308();

// Global object at 0x803C4298 (0x168 bytes, real type unknown).
struct UnkGlobals803C4298 {
    /* 0x00 */ u8 unk0[0x8];
    /* 0x08 */ void* m8;
    /* 0x0C */ u8 unkC[0x38 - 0xC];
    /* 0x38 */ void* m38;
    /* 0x3C */ u8 unk3C[0x168 - 0x3C];
};

extern UnkGlobals803C4298 lbl_803C4298;

#endif // GAME_MOTIONRECOGNIZER_H
