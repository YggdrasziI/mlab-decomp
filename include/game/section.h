#ifndef GAME_SECTION_H
#define GAME_SECTION_H

// Class "section" (name from its RTTI at 0x805F8F00), defined in the game unit at
// 0x80116BE8-0x80116CA8 (real file name unknown). Its vtable holds the destructor
// and four pure virtual functions (the derived vtables are 0x1C bytes; the 4 bytes
// after this one are padding); names and signatures unknown.

#include "types.h"

class section {
public:
    section();
    virtual ~section();
    virtual void vf0C() = 0;
    virtual void vf10() = 0;
    virtual void vf14() = 0;
    virtual void vf18() = 0;

    // Formats "m4/m8" into mText and returns it.
    const char* fn_80116C44();

    /* 0x0 */ // vtable
    /* 0x4 */ int m4;
    /* 0x8 */ int m8;
    /* 0xC */ char mText[0x10]; // size from QuestLog's first member at 0x1C
};

#endif // GAME_SECTION_H
