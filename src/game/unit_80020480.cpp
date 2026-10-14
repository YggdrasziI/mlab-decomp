// Game unit at 0x80020480-0x800214D4 (real file name unknown): the
// CAbstractInput* event stages. Bounds come from the unit's static initializer
// and its own .sdata2 constants; class names from RTTI.

#include "game/unit_8001FCA8.h"

CAbstractInputButtonJustReleased::CAbstractInputButtonJustReleased() {}

void CAbstractInputButtonJustReleased::vf10() {
    m4 = 0.0f;
    m0 = 0.0f;
    m50 = 0.0f;
}

void CAbstractInputButtonJustReleased::vf0C(f32 dt) {
    m0 = 1.0f;
    for (int i = 0; i < mSourceCount; i++) {
        mSources[i].fn_8001FCA8();
        m0 *= mSources[i].GetValue();
    }

    if (m0 == 0.0f && m50 != 0.0f) {
        m4 = 1.0f;
    } else {
        m4 = 0.0f;
    }
    m50 = m0;
}

CAbstractInputButtonHeld::CAbstractInputButtonHeld() {}

void CAbstractInputButtonHeld::vf0C(f32 dt) {
    m0 = 1.0f;
    for (int i = 0; i < mSourceCount; i++) {
        mSources[i].fn_8001FCA8();
        m0 *= mSources[i].GetValue();
    }

    if (m40 != 0.0f || m44 != 0.0f) {
        if (m3C != 0) {
            if (m0 < 0.0f) {
                if (m0 > -m40) {
                    m0 = 0.0f;
                }
                if (m0 < -m44) {
                    m0 = 0.0f;
                }
            } else {
                if (m0 < m40) {
                    m0 = 0.0f;
                }
                if (m0 > m44) {
                    m0 = 0.0f;
                }
            }
        } else {
            if (m0 < 0.0f) {
                if (m0 > -m40) {
                    m0 = -m40;
                }
                if (m0 < -m44) {
                    m0 = -m44;
                }
            } else {
                if (m0 < m40) {
                    m0 = m40;
                }
                if (m0 > m44) {
                    m0 = m44;
                }
            }
        }
    }
    m4 = m0;
}

CAbstractInputButtonJustPressed::CAbstractInputButtonJustPressed() {}

void CAbstractInputButtonJustPressed::vf10() {
    m4 = 0.0f;
    m0 = 0.0f;
    m50 = 0.0f;
}

void CAbstractInputButtonJustPressed::vf0C(f32 dt) {
    m0 = 1.0f;
    for (int i = 0; i < mSourceCount; i++) {
        mSources[i].fn_8001FCA8();
        m0 *= mSources[i].GetValue();
    }

    if (m0 != 0.0f && m50 == 0.0f) {
        m4 = m0;
    } else {
        m4 = 0.0f;
    }
    m50 = m0;
}

CAbstractInputTwoButtonToggle::CAbstractInputTwoButtonToggle() {}

void CAbstractInputTwoButtonToggle::vf10() {
    m50 = -1;
    m4 = 0.0f;
    m0 = 0.0f;
    m54 = 0.0f;
    m58 = 0.0f;
}

void CAbstractInputTwoButtonToggle::vf0C(f32 dt) {
    bool pressed[2] = {false, false};

    mSources[0].fn_8001FCA8();
    mSources[1].fn_8001FCA8();

    if (mSources[0].GetValue() != 0.0f && m54 == 0.0f) {
        pressed[0] = true;
    }
    if (mSources[1].GetValue() != 0.0f && m58 == 0.0f) {
        pressed[1] = true;
    }

    if (m50 == -1) {
        if (mSources[0].GetValue() > 0.0f) {
            m50 = 0;
        } else if (mSources[1].GetValue() > 0.0f) {
            m50 = 1;
        }
    } else {
        m4 = 0.0f;
        if (pressed[0] && pressed[1]) {
            m54 = 0.0f;
            m50 = -1;
            m58 = 0.0f;
        } else if (pressed[m50] && !pressed[m50 ^ 1]) {
            m4 = mSources[m50].GetValue();
            m50 ^= 1;
        }
    }

    m54 = mSources[0].GetValue();
    m58 = mSources[1].GetValue();
}

CAbstractInputRotateCW::CAbstractInputRotateCW() {}

void CAbstractInputRotateCW::vf10() {
    m50 = -1;
    m4 = 0.0f;
    m0 = 0.0f;
    m54 = 0.0f;
    m58 = 0.0f;
    m5C = 0.0f;
}

void CAbstractInputRotateCW::vf0C(f32 dt) {
    m0 = 1.0f;
    m54 -= dt;
    if (m54 <= 0.0f) {
        m4 = 0.0f;
        m54 = 0.0f;
    }
    m58 += dt;

    mSources[0].fn_8001FCA8();
    mSources[1].fn_8001FCA8();
    mSources[2].fn_8001FCA8();
    mSources[3].fn_8001FCA8();

    if (m50 == -1) {
        m58 = 0.0f;
        m5C = 0.0f;
        if (fn_8002030C(mSources[0].GetValue()) > 0.0f) {
            m50 = 0;
            m5C += 1.0f;
        } else if (fn_8002030C(mSources[1].GetValue()) > 0.0f) {
            m50 = 1;
            m5C += 1.0f;
        } else if (fn_8002030C(mSources[2].GetValue()) > 0.0f) {
            m50 = 2;
            m5C += 1.0f;
        } else if (fn_8002030C(mSources[3].GetValue()) > 0.0f) {
            m50 = 3;
            m5C += 1.0f;
        }
    } else if (fn_8002030C(mSources[3].GetValue()) > 0.0f) {
        if (m50 == 1) {
            m4 = 1.0f;
            m54 = 0.15f;
            m50 = 3;
            m5C += 1.0f;
        } else {
            m50 = -1;
        }
    } else if (fn_8002030C(mSources[0].GetValue()) > 0.0f) {
        if (m50 == 3) {
            m4 = 1.0f;
            m54 = 0.15f;
            m50 = 0;
            m5C += 1.0f;
        } else {
            m50 = -1;
        }
    } else if (fn_8002030C(mSources[2].GetValue()) > 0.0f) {
        if (m50 == 0) {
            m4 = 1.0f;
            m54 = 0.15f;
            m50 = 2;
            m5C += 1.0f;
        } else {
            m50 = -1;
        }
    } else if (fn_8002030C(mSources[1].GetValue()) > 0.0f) {
        if (m50 == 2) {
            m4 = 1.0f;
            m54 = 0.15f;
            m50 = 1;
            m5C += 1.0f;
        } else {
            m50 = -1;
        }
    }
}

CAbstractInputRotateCCW::CAbstractInputRotateCCW() {}

void CAbstractInputRotateCCW::vf10() {
    m50 = -1;
    m4 = 0.0f;
    m0 = 0.0f;
    m54 = 0.0f;
    m58 = 0.0f;
    m5C = 0.0f;
}

void CAbstractInputRotateCCW::vf0C(f32 dt) {
    m0 = 1.0f;
    m54 -= dt;
    if (m54 <= 0.0f) {
        m4 = 0.0f;
        m54 = 0.0f;
    }
    m58 += dt;

    mSources[0].fn_8001FCA8();
    mSources[1].fn_8001FCA8();
    mSources[2].fn_8001FCA8();
    mSources[3].fn_8001FCA8();

    if (m50 == -1) {
        m58 = 0.0f;
        m5C = 0.0f;
        if (fn_8002030C(mSources[0].GetValue()) > 0.0f) {
            m50 = 0;
            m5C += 1.0f;
        } else if (fn_8002030C(mSources[1].GetValue()) > 0.0f) {
            m50 = 1;
            m5C += 1.0f;
        } else if (fn_8002030C(mSources[2].GetValue()) > 0.0f) {
            m50 = 2;
            m5C += 1.0f;
        } else if (fn_8002030C(mSources[3].GetValue()) > 0.0f) {
            m50 = 3;
            m5C += 1.0f;
        }
    } else if (fn_8002030C(mSources[3].GetValue()) > 0.0f) {
        if (m50 == 0) {
            m4 = 1.0f;
            m54 = 0.15f;
            m50 = 3;
            m5C += 1.0f;
        } else {
            m50 = -1;
        }
    } else if (fn_8002030C(mSources[0].GetValue()) > 0.0f) {
        if (m50 == 2) {
            m4 = 1.0f;
            m54 = 0.15f;
            m50 = 0;
            m5C += 1.0f;
        } else {
            m50 = -1;
        }
    } else if (fn_8002030C(mSources[2].GetValue()) > 0.0f) {
        if (m50 == 1) {
            m4 = 1.0f;
            m54 = 0.15f;
            m50 = 2;
            m5C += 1.0f;
        } else {
            m50 = -1;
        }
    } else if (fn_8002030C(mSources[1].GetValue()) > 0.0f) {
        if (m50 == 3) {
            m4 = 1.0f;
            m54 = 0.15f;
            m50 = 1;
            m5C += 1.0f;
        } else {
            m50 = -1;
        }
    }
}

CAbstractInputAnyButtonsHeld::CAbstractInputAnyButtonsHeld() {}

void CAbstractInputAnyButtonsHeld::vf0C(f32 dt) {
    f32 sum = 0.0f;
    m4 = sum;
    for (int i = 0; i < mSourceCount; i++) {
        mSources[i].fn_8001FCA8();
        sum += mSources[i].GetValue();
    }

    if (sum > 0.0f) {
        m4 = 1.0f;
    }
}

CAbstractInputAnyButtonsReleased::CAbstractInputAnyButtonsReleased() {}

void CAbstractInputAnyButtonsReleased::vf10() {
    m4 = 0.0f;
    m0 = 0.0f;
    mPrev[3] = 0.0f;
    mPrev[2] = 0.0f;
    mPrev[1] = 0.0f;
    mPrev[0] = 0.0f;
}

// NONMATCHING (this function and AnyButtonsJustPressed::vf0C): code is right except
// that the DOL schedules the loop increment of i before the two source-pointer
// increments (addi r28 / r31 / r29); every loop shape tried, and all GC 3.0
// compilers, give r31 / r29 / r28.
void CAbstractInputAnyButtonsReleased::vf0C(f32 dt) {
    m4 = 0.0f;
    for (int i = 0; i < mSourceCount; i++) {
        mSources[i].fn_8001FCA8();
        if (mPrev[i] > 0.0f && mSources[i].GetValue() == 0.0f) {
            m4 = 1.0f;
        }
        mPrev[i] = mSources[i].GetValue();
    }
}

CAbstractInputAnyButtonsJustPressed::CAbstractInputAnyButtonsJustPressed() {}

void CAbstractInputAnyButtonsJustPressed::vf10() {
    m4 = 0.0f;
    m0 = 0.0f;
    mPrev[3] = 0.0f;
    mPrev[2] = 0.0f;
    mPrev[1] = 0.0f;
    mPrev[0] = 0.0f;
}

void CAbstractInputAnyButtonsJustPressed::vf0C(f32 dt) {
    m4 = 0.0f;
    for (int i = 0; i < mSourceCount; i++) {
        mSources[i].fn_8001FCA8();
        if (mPrev[i] == 0.0f && mSources[i].GetValue() > 0.0f) {
            m4 = 1.0f;
        }
        mPrev[i] = mSources[i].GetValue();
    }
}

CAbstractInputToggle::CAbstractInputToggle() {}

void CAbstractInputToggle::vf10() {
    m4 = 0.0f;
    m0 = 0.0f;
    m50 = 0.0f;
}

void CAbstractInputToggle::vf0C(f32 dt) {
    m0 = 1.0f;
    for (int i = 0; i < mSourceCount; i++) {
        mSources[i].fn_8001FCA8();
        m0 *= mSources[i].GetValue();
    }

    if (m0 != 0.0f && m50 == 0.0f) {
        if (m4 == 0.0f) {
            m4 = 1.0f;
        } else {
            m4 = 0.0f;
        }
    }
    m50 = m0;
}
