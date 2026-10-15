#ifndef GAME_UNKLOGGER_H
#define GAME_UNKLOGGER_H

// Test log writer (real name unknown). Its functions start at 0x80143158; strings
// "fail.log", "NoTestNameSet" and "***** Logging Done *****" are used by it. The
// global instance is built by the static initializer at 0x801424B8.

#include "types.h"
#include <stdio.h>

class UnkLogger {
public:
    UnkLogger();
    ~UnkLogger();

    /* 0x000 */ FILE* mFile;
    /* 0x004 */ char m4[0x100];
    /* 0x104 */ char m104[0x200];
    /* 0x304 */ int m304;
    /* 0x308 */ int m308;
    /* 0x30C */ char mMonths[12][4];
    /* 0x33C */ char mDays[7][4];
    /* 0x358 */ char m358[0x100];
    /* 0x458 */ char mTestName[0x100];
};

extern UnkLogger lbl_803F22D8;

#endif // GAME_UNKLOGGER_H
