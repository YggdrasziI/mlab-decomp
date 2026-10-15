#ifndef GAME_BOOK_H
#define GAME_BOOK_H

// Class "book" (name from its RTTI at 0x805F8EE8), defined in the game unit at
// 0x8010FD14-0x8010FDA4 (real file name unknown). Its vtable holds the destructor
// and eight pure virtual functions: the derived InventoryBook vtable is 0x2C bytes
// long, so the last zero word of book's 0x30-byte vtable is padding. Names unknown;
// the signatures follow InventoryBook's overrides.

#include "types.h"

class section;

class book {
public:
    book();
    virtual ~book();
    virtual void vf0C() = 0;
    virtual void vf10() = 0;
    virtual void vf14() = 0;
    virtual void vf18() = 0;
    // Index of the section named by a hash, or -1.
    virtual int vf1C(u32 hash) = 0;
    virtual void vf20(u32 hash) = 0;
    virtual int vf24(u32 hash) = 0;
    virtual section* vf28(u32 hash) = 0;

    /* 0x0 */ // vtable
    /* 0x4 */ section* m4;
};

#endif // GAME_BOOK_H
