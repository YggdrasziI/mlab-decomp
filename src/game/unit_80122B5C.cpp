// Game unit at 0x80122B5C-0x801230DC (real file name unknown): controller input
// helpers. A stick reader keeps the normalized direction, its length (at most 1) and
// button flags for one controller channel; free functions map a game button to
// controller bits and test or consume presses. Bounds come from the static
// initializers of this unit and the previous one.

#include "game/ActionSelectionCallback.h"
#include "game/unk_803C4298.h"
#include "game/unk_math.h"
#include "game/unk_vec3.h"

// Stick and button state of one controller channel (real name unknown).
class UnkStickInput {
public:
    UnkStickInput();
    void fn_80122B98();
    UnkStickInput(int channel);
    void fn_80122BD8();
    void fn_80122E2C(UnkVec3* dir, f32* length);

    /* 0x00 */ UnkVec3 mDir;
    /* 0x0C */ f32 mLength;
    /* 0x10 */ u32 mButtons;
    /* 0x14 */ u32 mPrevButtons;
    /* 0x18 */ int mChannel;
};

extern "C" {
extern u8 lbl_805FAF64;

void fn_80147704(int channel, f32* x, f32* y);
u32 fn_801B9B98(int channel);
u32 fn_801B9B6C(int channel);
u32 fn_801B9B40(int channel);
void fn_801B9BC4(int channel, u32 bits);
u32 fn_801B94F8(int channel, int button);
int fn_800E6228(void);
int fn_8000C30C(UnkGlobals803C4298* globals);
}

static inline bool IsNaN(f32 x) {
    return x != x;
}

static inline f32 Sqrt2(f32 x) {
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

static inline double Normalize(UnkVec3& v) {
    f32 lengthSq = v.LengthSq();
    f32 length = 0.0f;
    UnkVec3 dir;
    if (0.0f == lengthSq) {
        dir.x = dir.y = dir.z = 0.0f;
    } else {
        length = Sqrt2(lengthSq);
        f32 inv = 1.0f / length;
        dir.x = v.x * inv;
        dir.y = v.y * inv;
        dir.z = v.z * inv;
    }
    v = dir;
    return length;
}

UnkStickInput::UnkStickInput() {
    mButtons = 0;
    mDir = UnkVec3Zero();
    mLength = 0.0f;
    mPrevButtons = 0;
    mChannel = 0;
}

void UnkStickInput::fn_80122B98() {}

UnkStickInput::UnkStickInput(int channel) {
    mButtons = 0;
    mDir = UnkVec3Zero();
    mLength = 0.0f;
    mPrevButtons = 0;
    mChannel = channel;
}

// NonMatching (97.2 %): float registers of the normalization (length and length
// squared swapped) and the clamp of the length.
void UnkStickInput::fn_80122BD8() {
    if (mChannel >= 0 && mChannel < 4) {
        mDir = UnkVec3Zero();
        mLength = 0.0f;
        mPrevButtons = mButtons;
        mButtons = 0;
        f32 x;
        f32 y;
        fn_80147704(((int)lbl_803C4298.m38 << 4) + 0xE, &x, &y);
        mDir.x = x;
        if (IsNaN(mDir.x)) {
            mDir.x = 0.0f;
        }
        mDir.y = y;
        if (IsNaN(mDir.y)) {
            mDir.y = 0.0f;
        }
        f32 length = Normalize(mDir);
        mLength = length < 1.0f ? length : 1.0f;
        if (fn_801B9B98(mChannel) & 0x40) {
            mButtons |= 0x1;
        }
        if (fn_801B9B98(mChannel) & 0x100) {
            mButtons |= 0x2;
        }
        if (fn_801B9B98(mChannel) & 0x200) {
            mButtons |= 0x4;
        }
        if (fn_801B9B98(mChannel) & 0x80) {
            mButtons |= 0x8;
        }
        if (fn_801B9B98(mChannel) & 0x20) {
            mButtons |= 0x10;
        }
    }
}

// NonMatching (90 %): the DOL loads the three direction floats before storing them.
void UnkStickInput::fn_80122E2C(UnkVec3* dir, f32* length) {
    if (lbl_805FAF64) {
        *dir = UnkVec3Zero();
        *length = 0.0f;
    } else {
        f32 x = mDir.x;
        f32 y = mDir.y;
        f32 z = mDir.z;
        f32 len = mLength;
        dir->x = x;
        dir->y = y;
        dir->z = z;
        *length = len;
    }
}

extern "C" {

u32 fn_80122E88(int button) {
    u32 bits = 0;
    switch (button) {
    case 0:
        break;
    case 1:
        bits |= 0x40;
        break;
    case 2:
        bits |= 0x80;
        break;
    case 3:
        bits |= 0x100;
        break;
    case 4:
        bits |= 0x200;
        break;
    case 5:
        bits |= 0x1;
        break;
    case 6:
        bits |= 0x2;
        break;
    case 7:
        bits |= 0x4;
        break;
    case 8:
        bits |= 0x8;
        break;
    case 9:
        bits |= 0x20;
        break;
    case 10:
        bits |= 0x10;
        break;
    case 11:
        bits |= 0x800;
        break;
    case 12:
        bits |= 0x400;
        break;
    case 13:
        bits |= 0x3C0;
        break;
    case 14:
        bits |= 0xF;
        break;
    case 15:
        bits |= 0x30;
        break;
    case 16:
        bits |= 0xFFF;
        break;
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
        break;
    }
    return bits;
}

static inline u32 GetBits(int button) {
    u32 bits;
    int mode = fn_800E6228();
    if (mode) {
        bits = fn_80122E88(button);
    } else {
        bits = fn_801B94F8(mode, button);
    }
    return bits;
}

int fn_80122F30(int button) {
    u32 bits = GetBits(button);
    return (bits & fn_801B9B98((int)lbl_803C4298.m38)) != 0;
}

u32 fn_80122FA0(int button) {
    if (lbl_8047C7D8.unk0[0]) {
        return 0;
    }
    if (fn_8000C30C(&lbl_803C4298)) {
        return 0;
    }
    u32 bits = GetBits(button);
    u32 pressed = bits & fn_801B9B40(0);
    if (pressed) {
        fn_801B9BC4(0, bits);
    }
    return pressed;
}

int fn_8012304C(int button) {
    u32 bits = GetBits(button);
    return (bits & fn_801B9B6C((int)lbl_803C4298.m38)) != 0;
}
}
