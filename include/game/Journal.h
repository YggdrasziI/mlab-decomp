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
    virtual void vf1C();

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
    virtual void vf1C();

    u8 unk[0x50 - sizeof(section)];
};

class QuestLog : public section {
public:
    QuestLog();
    virtual ~QuestLog();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();

    u8 unk[0x34 - sizeof(section)];
};

class Recipes : public section {
public:
    Recipes();
    virtual ~Recipes();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();

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
