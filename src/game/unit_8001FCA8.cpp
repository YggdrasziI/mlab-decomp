// Game unit at 0x8001FCA8-0x80020480 (real file name unknown): the input source
// update, the CAbstractInputEventStage base and the stage sequence. Bounds come
// from the unit's static initializer and its own .sdata2 constants.

#include "game/unit_8001FCA8.h"

#include <math.h>

void UnkInputSource::fn_8001FCA8() {
    switch (mType) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        mValue = fn_80147800(unk0) > 0.005f ? 1.0f : 0.0f;
        break;
    case 14: {
        f32 y, x;
        fn_80147704(unk0, &x, &y);
        if (x < -0.005f) {
            mValue = -x;
        } else {
            mValue = 0.0f;
        }
        break;
    }
    case 15: {
        f32 y, x;
        fn_80147704(unk0, &x, &y);
        if (x > 0.005f) {
            mValue = x;
        } else {
            mValue = 0.0f;
        }
        break;
    }
    case 16: {
        f32 y, x;
        fn_80147704(unk0, &x, &y);
        // The double conversion gives the frsp of the original.
        f32 v = (double)(y > -0.005f ? 0.0f : y);
        if (v < 0.0f) {
            mValue = -v;
        } else {
            mValue = 0.0f;
        }
        break;
    }
    case 17: {
        f32 y, x;
        fn_80147704(unk0, &x, &y);
        if (y > 0.005f) {
            mValue = y;
        } else {
            mValue = 0.0f;
        }
        break;
    }
    case 18: {
        f32 y, x;
        fn_80147704(unk0, &x, &y);
        if (x < -0.005f) {
            mValue = -x;
        } else {
            mValue = 0.0f;
        }
        break;
    }
    case 19: {
        f32 y, x;
        fn_80147704(unk0, &x, &y);
        if (x > 0.005f) {
            mValue = x;
        } else {
            mValue = 0.0f;
        }
        break;
    }
    case 20: {
        f32 y, x;
        fn_80147704(unk0, &x, &y);
        // The double conversion gives the frsp of the original.
        f32 v = (double)(y > -0.005f ? 0.0f : y);
        if (v < 0.0f) {
            mValue = -v;
        } else {
            mValue = 0.0f;
        }
        break;
    }
    case 21: {
        f32 y, x;
        fn_80147704(unk0, &x, &y);
        if (y > 0.005f) {
            mValue = y;
        } else {
            mValue = 0.0f;
        }
        break;
    }
    case 23: {
        f32 y, x;
        fn_80147704(unk0, &x, &y);
        if (x < 0.005f && x > -0.005f) {
            x = 0.0f;
        }
        mValue = x;
        break;
    }
    case 24: {
        f32 y, x;
        fn_80147704(unk0, &x, &y);
        if (y < 0.005f && y > -0.005f) {
            y = 0.0f;
        }
        mValue = y;
        break;
    }
    case 25: {
        f32 y, x;
        fn_80147704(unk0, &x, &y);
        if (x < 0.005f && x > -0.005f) {
            x = 0.0f;
        }
        mValue = x;
        break;
    }
    case 26: {
        f32 y, x;
        fn_80147704(unk0, &x, &y);
        if (y < 0.005f && y > -0.005f) {
            y = 0.0f;
        }
        mValue = y;
        break;
    }
    case 27: {
        f32 y, x;
        fn_80147704(unk0, &x, &y);
        mValue = -atan2f(y, x);
        break;
    }
    case 28: {
        f32 y, x;
        fn_80147704(unk0, &x, &y);
        mValue = -atan2f(y, x);
        break;
    }
    }
}

void CAbstractInputEventStage::vf08() {
    m40 = 0.0f;
    m44 = 1.0f;
    m3C = 0;
    m0 = 0.0f;
    m4 = 0.0f;
    mSourceCount = 0;
}

f32 UnkInputSequence::fn_80020068() {
    return mC;
}

void UnkInputSequence::fn_80020070(f32 dt) {
    if (mCount == 1) {
        mTimer += dt;
        mStages[0]->vf0C(dt);
        mC = mStages[0]->m4;
        if (mTimer < m3C) {
            mC = 0.0f;
        } else if (mTimer > m40 && m40 > 0.0f) {
            mTimer = 0.0f;
            mIndex = 0;
            mC = 0.0f;
            m8 = 0.0f;
            mStages[0]->vf10();
        }
        m8 = mC;
    } else if (mIndex < mCount) {
        mStages[mIndex]->vf0C(dt);
        if (m44 == 1) {
            mTimer += dt;
        }
        if (m44 == 0 && mIndex == 0 && mStages[mIndex]->m4 != 0.0f && m8 == 0.0f) {
            mTimer = 0.0f;
            m44 = 1;
            m8 = mStages[mIndex]->m4;
        } else {
            f32 value = mStages[mIndex]->m4;
            if (value != 0.0f) {
                m8 = value;
            } else if (value == 0.0f && m8 != 0.0f) {
                mIndex++;
                if (mIndex >= mCount && mTimer >= m3C) {
                    mC = m8;
                }
                if (mIndex < mCount) {
                    mStages[mIndex]->vf10();
                }
                m8 = 0.0f;
            }
        }
        if (m40 > 0.0f && mTimer >= m40) {
            mTimer = 0.0f;
            m44 = 0;
            mIndex = 0;
            mC = 0.0f;
            m8 = 0.0f;
        }
    } else {
        mStages[0]->vf10();
        m44 = 0;
        mIndex = 0;
        mC = 0.0f;
        fn_80020070(dt);
    }
}

void CAbstractInputEventStage::vf10() {
    m4 = 0.0f;
    m0 = 0.0f;
}

CAbstractInputEventStage::CAbstractInputEventStage() {}

f32 CAbstractInputEventStage::fn_8002030C(f32 value) {
    if (value < m40) {
        return 0.0f;
    }
    if (value > m44) {
        return 0.0f;
    }
    return value;
}

void UnkInputSequence::fn_80020334() {
    for (int i = 0; i < mCount; i++) {
        CAbstractInputEventStage* stage = mStages[i];
        switch (stage->mKind) {
        case 0:
            new (stage) CAbstractInputButtonHeld;
            break;
        case 1:
            new (stage) CAbstractInputButtonJustReleased;
            break;
        case 2:
            new (stage) CAbstractInputButtonJustPressed;
            break;
        case 3:
            new (stage) CAbstractInputTwoButtonToggle;
            break;
        case 4:
            new (stage) CAbstractInputAnyButtonsHeld;
            break;
        case 5:
            new (stage) CAbstractInputAnyButtonsReleased;
            break;
        case 6:
            new (stage) CAbstractInputAnyButtonsJustPressed;
            break;
        case 7:
            new (stage) CAbstractInputRotateCW;
            break;
        case 8:
            new (stage) CAbstractInputRotateCCW;
            break;
        case 9:
            new (stage) CAbstractInputToggle;
            break;
        }
    }
}

f32 CAbstractInputEventStage::vf14() {
    return 0.0f;
}
