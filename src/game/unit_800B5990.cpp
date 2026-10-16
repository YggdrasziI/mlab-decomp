// Game unit at 0x800B5990-0x800B5E64 (real file name unknown): a button sequence
// challenge (real class name unknown). It draws up to ten random buttons, shows them
// as "A>B>C>Z>" text and checks that the player presses them in order before the
// timer runs out. Bounds come from the static initializers of this unit and the
// previous one.

#include "game/unk_803C4298.h"
#include "game/unk_vec3.h"

class UnkButtonSequence {
public:
    UnkButtonSequence();
    ~UnkButtonSequence();
    void fn_800B59E8(f32 dt);
    void fn_800B5B20(f32 time);
    void fn_800B5B30();
    f32 fn_800B5B44();
    int fn_800B5B4C();
    char* fn_800B5B54();
    void fn_800B5CB0();

    /* 0x00 */ int mState;
    /* 0x04 */ int mButtons[10];
    /* 0x2C */ f32 mTimer;
    /* 0x30 */ int mIndex;
    /* 0x34 */ char mText[0x20];
};

extern "C" {
u32 fn_80143460(void);
int fn_80070458(void);
int fn_80147554(int button);
}

// Number of buttons per difficulty (real name unknown).
extern int lbl_80346248[];

static inline int GetButton(int index) {
    int button = 0;
    switch (index) {
    case 0:
        button = 0x1089;
        break;
    case 1:
        button = 0x108A;
        break;
    case 2:
        button = 0x109C;
        break;
    case 3:
        button = 0x109B;
        break;
    }
    return button;
}

static inline int GetPlayerButton(int button) {
    return button + ((int)lbl_803C4298.m38 << 4);
}

UnkButtonSequence::UnkButtonSequence() {
    mState = 0;
    mTimer = 0.0f;
    mIndex = 0;
}

UnkButtonSequence::~UnkButtonSequence() {}

void UnkButtonSequence::fn_800B59E8(f32 dt) {
    fn_800B5CB0();
    switch (mState) {
    case 0:
        break;
    case 1: {
        mState = 2;
        mIndex = 0;
        for (int i = 0; i < 10; i++) {
            mButtons[i] = 0;
        }
        int j;
        int count = lbl_80346248[fn_80070458()];
        for (j = 0; j < count; j++) {
            mButtons[j] = GetButton(fn_80143460() & 3);
        }
        break;
    }
    case 2:
        mTimer -= dt;
        break;
    case 3:
        break;
    }
}

void UnkButtonSequence::fn_800B5B20(f32 time) {
    mTimer = time;
    mState = 1;
}

void UnkButtonSequence::fn_800B5B30() {
    mState = 0;
    mTimer = 0.0f;
}

f32 UnkButtonSequence::fn_800B5B44() {
    return mTimer;
}

int UnkButtonSequence::fn_800B5B4C() {
    return mState;
}

char* UnkButtonSequence::fn_800B5B54() {
    for (int i = 0; i < 0x20; i++) {
        mText[i] = ' ';
    }
    int pos = 0;
    for (int i = mIndex; i < 10; i++) {
        pos = i * 2;
        switch (mButtons[i]) {
        case 0x1089:
            *(mText + pos) = 'A';
            break;
        case 0x108A:
            *(mText + pos) = 'B';
            break;
        case 0x109C:
            *(mText + pos) = 'C';
            break;
        case 0x109B:
            *(mText + pos) = 'Z';
            break;
        default:
            *(mText + pos + 1) = 0;
            return mText;
        }
        *(mText + pos + 1) = '>';
    }
    *(mText + pos + 1) = 0;
    return mText;
}

void UnkButtonSequence::fn_800B5CB0() {
    if (mState != 2) {
        return;
    }
    int pressed = 0;
    int any = 0;
    for (int i = 0; i < 4; i++) {
        if (fn_80147554(GetPlayerButton(GetButton(i)))) {
            pressed = GetButton(i);
            if (!any) {
                any = 1;
            }
        }
    }
    if (mTimer <= 0.0f) {
        mState = 4;
        return;
    }
    if (any) {
        if (pressed == mButtons[mIndex]) {
            mIndex++;
            if (mIndex >= 10 || mButtons[mIndex] == 0) {
                mState = 3;
                mTimer = 0.0f;
            }
        } else {
            mTimer = 0.0f;
            mState = 4;
        }
    }
}
