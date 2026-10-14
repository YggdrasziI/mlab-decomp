#ifndef GAME_UNIT_8001FCA8_H
#define GAME_UNIT_8001FCA8_H

// Declarations for the game unit at 0x8001FCA8-0x80020480 (real file name unknown).
// Virtual functions are named after their vtable offset (vfXX); real names unknown.
// Class names come from the RTTI type names in .data; member names are unknown and
// use offsets or the address of the original function.

#include "types.h"

// Three-float vector with an inline constructor. Each game unit that includes this
// header gets its own (0, 1, 0) copy in .bss, built by the unit's static initializer.
// Real type and object names unknown.
struct UnkVec3 {
    f32 x, y, z;
    UnkVec3(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

static UnkVec3 sUnkVec3(0.0f, 1.0f, 0.0f);

// One input source (0xC bytes), refreshed by fn_8001FCA8. Real name unknown.
struct UnkInputSource {
    /* 0x0 */ u32 unk0;
    /* 0x4 */ f32 mValue;
    /* 0x8 */ u32 mType;

    void fn_8001FCA8();
    f32 GetValue() const { return mValue; }
};

class CAbstractInputEventStage {
public:
    /* 0x00 */ f32 m0;
    /* 0x04 */ f32 m4;
    /* 0x08 */ s32 mSourceCount;
    /* 0x0C */ UnkInputSource mSources[4];
    /* 0x3C */ s32 m3C;
    /* 0x40 */ f32 m40;
    /* 0x44 */ f32 m44;
    /* 0x48 */ s32 mKind;

    CAbstractInputEventStage();

    virtual void vf08();
    virtual void vf0C(f32 dt) = 0;
    virtual void vf10();
    virtual f32 vf14();

    f32 fn_8002030C(f32 value);

    /* 0x4C */ // vtable
};

class CAbstractInputButtonJustReleased : public CAbstractInputEventStage {
public:
    CAbstractInputButtonJustReleased();
    virtual void vf0C(f32 dt);
    virtual void vf10();

    /* 0x50 */ f32 m50;
};

class CAbstractInputButtonJustPressed : public CAbstractInputEventStage {
public:
    CAbstractInputButtonJustPressed();
    virtual void vf0C(f32 dt);
    virtual void vf10();

    /* 0x50 */ f32 m50;
};

class CAbstractInputButtonHeld : public CAbstractInputEventStage {
public:
    CAbstractInputButtonHeld();
    virtual void vf0C(f32 dt);
};

class CAbstractInputTwoButtonToggle : public CAbstractInputEventStage {
public:
    CAbstractInputTwoButtonToggle();
    virtual void vf0C(f32 dt);
    virtual void vf10();

    /* 0x50 */ s32 m50;
    /* 0x54 */ f32 m54;
    /* 0x58 */ f32 m58;
};

class CAbstractInputRotateCW : public CAbstractInputEventStage {
public:
    CAbstractInputRotateCW();
    virtual void vf0C(f32 dt);
    virtual void vf10();
    virtual f32 vf14() {
        if (m58 == 0.0f) {
            return 0.0f;
        }
        return 0.25f * m5C / m58;
    }

    /* 0x50 */ s32 m50;
    /* 0x54 */ f32 m54;
    /* 0x58 */ f32 m58;
    /* 0x5C */ f32 m5C;
};

class CAbstractInputRotateCCW : public CAbstractInputEventStage {
public:
    CAbstractInputRotateCCW();
    virtual void vf0C(f32 dt);
    virtual void vf10();
    virtual f32 vf14() {
        if (m58 == 0.0f) {
            return 0.0f;
        }
        return 0.25f * m5C / m58;
    }

    /* 0x50 */ s32 m50;
    /* 0x54 */ f32 m54;
    /* 0x58 */ f32 m58;
    /* 0x5C */ f32 m5C;
};

class CAbstractInputAnyButtonsHeld : public CAbstractInputEventStage {
public:
    CAbstractInputAnyButtonsHeld();
    virtual void vf0C(f32 dt);
};

class CAbstractInputAnyButtonsReleased : public CAbstractInputEventStage {
public:
    CAbstractInputAnyButtonsReleased();
    virtual void vf0C(f32 dt);
    virtual void vf10();

    /* 0x50 */ f32 mPrev[4];
};

class CAbstractInputAnyButtonsJustPressed : public CAbstractInputEventStage {
public:
    CAbstractInputAnyButtonsJustPressed();
    virtual void vf0C(f32 dt);
    virtual void vf10();

    /* 0x50 */ f32 mPrev[4];
};

class CAbstractInputToggle : public CAbstractInputEventStage {
public:
    CAbstractInputToggle();
    virtual void vf0C(f32 dt);
    virtual void vf10();

    /* 0x50 */ f32 m50;
};

#endif // GAME_UNIT_8001FCA8_H
