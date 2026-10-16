#ifndef GAME_JOURNAL_H
#define GAME_JOURNAL_H

// Class "Journal" (RTTI) and its four sections "Badges", "Statistics", "QuestLog"
// and "Recipes" (RTTI names; sizes from the allocations in the Journal
// constructor). The sections' functions are at 0x8011C460 and from 0x8011D5C4 (units
// not split yet). Real file names unknown.

#include "game/book.h"
#include "game/section.h"

class Badges : public section {
public:
    Badges();
    virtual ~Badges();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();

    u8 unk[0x134C - sizeof(section)];
};

class Statistics : public section {
public:
    Statistics();
    virtual ~Statistics();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();

    u8 unk[0x50 - sizeof(section)];
};

// Quest of the quest manager (real type unknown).
struct UnkQuest {
    /* 0x00 */ u32 mNameHash;
    /* 0x04 */ u8 unk4[0x8 - 0x4];
    /* 0x08 */ int mState;
    /* 0x0C */ int mProgress;
    /* 0x10 */ int mGoal;
};

// Texture returned by fn_80109330 (real type unknown; reference counted).
struct UnkTexture {
    /* 0x00 */ u8 unk0[0x24];
    /* 0x24 */ int mRefCount;
};

// Unit 0x8011EA20 (game/unit_8011EA20.cpp).
class QuestLog : public section {
public:
    QuestLog();
    virtual ~QuestLog();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();

    static UnkTexture* fn_8011ECCC(s8 icon);
    static UnkTexture* fn_8011ED38(s8 icon);
    int fn_8011EDAC();
    const char* fn_8011EDE0();
    const char* fn_8011EEF4();
    int fn_8011EFB8();
    int fn_8011EFE0(int index);
    int fn_8011EFFC(int index);
    int fn_8011F080(int index);
    char* fn_8011F104(int index);
    int fn_8011F1C4();
    UnkTexture* fn_8011F1D8();
    const char* fn_8011F1E0(int index);
    int fn_8011F2FC();
    const char* fn_8011F32C();

    /* 0x1C */ char mProgressText[3][5];
    /* 0x2B */ u8 mHasMainQuest;
    /* 0x2C */ int mQuestCount;
    /* 0x30 */ UnkTexture* mIcon;
};

class Recipes : public section {
public:
    Recipes();
    virtual ~Recipes();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();

    u8 unk[0xA68C - sizeof(section)];
};

class Journal : public book {
public:
    Journal();
    virtual ~Journal();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual int vf1C(u32 hash);
    virtual void vf20(u32 hash);
    virtual int vf24(u32 hash);
    virtual section* vf28(u32 hash);

    /* 0x08 */ section* mSections[4];
    /* 0x18 */ int mCurrent;
    /* 0x1C */ u8 mOpen;
};

#endif // GAME_JOURNAL_H
