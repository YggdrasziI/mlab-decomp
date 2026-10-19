// Game unit at 0x8011D1C0-0x8011D5C4 (real file name unknown): the Journal class
// (RTTI), a book of four sections chosen by name hash. Bounds come from the static
// initializers of this unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/Journal.h"
#include "game/unk_vec3.h"

Journal::Journal() {
    mCurrent = -1;
    mOpen = 0;
    mSections[0] = 0;
    mSections[1] = 0;
    mSections[2] = 0;
    mSections[3] = 0;
    mSections[0] = new Badges;
    mSections[1] = new Statistics;
    mSections[2] = new QuestLog;
    mSections[3] = new Recipes;
}

Journal::~Journal() {
    for (int i = 0; i < 4; i++) {
        if (mSections[i] != 0) {
            delete mSections[i];
            mSections[i] = 0;
        }
    }
}

void Journal::vf0C() {
    mOpen = 1;
}

void Journal::vf10() {
    mOpen = 0;
    if (mCurrent != -1) {
        mSections[mCurrent]->vf10();
    }
    mCurrent = -1;
}

void Journal::vf14() {}

void Journal::vf18() {}

void Journal::vf20(u32 hash) {
    if (vf24(hash) == 0) {
        m4 = 0;
        if (mCurrent != 4 && mCurrent != -1 && mOpen) {
            mSections[mCurrent]->vf10();
        }
        mCurrent = vf1C(hash);
        if (mCurrent != -1 && mOpen) {
            mSections[mCurrent]->vf0C();
            m4 = mSections[mCurrent];
        }
    }
}

int Journal::vf1C(u32 hash) {
    switch ((int)hash) {
    case (int)0x880E941C:
        return 0;
    case (int)0xE94C961D:
        return 1;
    case (int)0x0FD3AF4D:
        return 2;
    case (int)0x9C2C1CAF:
        return 3;
    default:
        return -1;
    }
}

section* Journal::vf28(u32 hash) {
    int index = vf1C(hash);
    if (index >= 0) {
        return mSections[index];
    }
    return 0;
}

int Journal::vf24(u32 hash) {
    int index = vf1C(hash);
    return index == mCurrent;
}
