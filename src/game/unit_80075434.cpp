// Game unit at 0x80075434-0x800756E0 (real file name unknown): a table of up to 20
// clamped values identified by id, each with up to 10 threshold conditions parsed
// from comparison strings ("<", "<=", ">", ">="). Bounds come from the static
// initializers of this unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/unk_vec3.h"
#include <string.h>

// Real names unknown.
struct UnkThresholdCondition {
    /* 0x0 */ f32 mThreshold;
    /* 0x4 */ int mType;
    /* 0x8 */ int mArg;
};

struct UnkThresholdValue {
    /* 0x00 */ f32 mValue;
    /* 0x04 */ u32 mId;
    /* 0x08 */ f32 mMax;
    /* 0x0C */ f32 mMin;
    /* 0x10 */ UnkThresholdCondition mConditions[10];
    /* 0x88 */ u32 mConditionCount;
};

class UnkThresholdTable {
public:
    UnkThresholdTable() { Reset(); }

    void Reset() {
        memset(mValues, 0, sizeof(mValues));
        mCount = 0;
    }

    UnkThresholdValue* Find(u32 id) {
        for (u32 i = 0; i < mCount; i++) {
            if (id == mValues[i].mId) {
                return &mValues[i];
            }
        }
        return 0;
    }

    /* 0x000 */ UnkThresholdValue mValues[20];
    /* 0xAF0 */ u32 mCount;
};

static UnkThresholdTable lbl_803C5DB0;

// Whether a value passes a condition. Its result is never used: only the compare
// trees of its switch are left in fn_80075590, as in the DOL.
static inline bool UnkTest(const f32& value, const UnkThresholdCondition& condition) {
    switch (condition.mType) {
    case 0:
        return value >= condition.mThreshold;
    case 1:
        return value > condition.mThreshold;
    case 2:
        return value <= condition.mThreshold;
    case 3:
        return value < condition.mThreshold;
    }
    return false;
}

extern "C" {

void fn_80075434(void) {
    lbl_803C5DB0.Reset();
}

void fn_80075478(u32 id, f32 value, f32 min, f32 max) {
    UnkThresholdValue* entry = &lbl_803C5DB0.mValues[lbl_803C5DB0.mCount++];
    entry->mId = id;
    entry->mMax = max;
    entry->mMin = min;
    if (value < entry->mMin) {
        value = entry->mMin;
    }
    if (value > entry->mMax) {
        value = entry->mMax;
    }
    entry->mValue = value;
    entry->mConditionCount = 0;
}

void fn_800754D0(u32 id, const char* op, f32 threshold, int arg) {
    int type = 0;
    if (op[0] == '<') {
        if (op[1] == '=') {
            type = 2;
        } else {
            type = 3;
        }
    } else if (op[0] == '>') {
        type = op[1] != '=';
    }
    UnkThresholdValue* entry = lbl_803C5DB0.Find(id);
    UnkThresholdCondition* condition = &entry->mConditions[entry->mConditionCount++];
    condition->mThreshold = threshold;
    condition->mType = type;
    condition->mArg = arg;
}

// NonMatching (96.7 %): the DOL stores the previous value to the stack after the
// loop guard; our build stores it before. The two values are kept in a stack array
// (the slots and frame size match).
void fn_80075590(u32 id, f32 delta) {
    UnkThresholdValue* entry = lbl_803C5DB0.Find(id);
    f32 values[3];
    values[2] = entry->mValue;
    f32 value = values[2] + delta;
    if (value < entry->mMin) {
        value = entry->mMin;
    }
    if (value > entry->mMax) {
        value = entry->mMax;
    }
    entry->mValue = value;
    for (u32 i = 0; i < entry->mConditionCount; i++) {
        values[1] = entry->mValue;
        bool was = UnkTest(values[2], entry->mConditions[i]);
        bool is = UnkTest(values[1], entry->mConditions[i]);
    }
}

}
