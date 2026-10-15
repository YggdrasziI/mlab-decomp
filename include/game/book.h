#ifndef GAME_BOOK_H
#define GAME_BOOK_H

// Class "book" (name from its RTTI at 0x805F8EE8), defined in the game unit at
// 0x8010FD14-0x8010FDA4 (real file name unknown). Its vtable holds the destructor
// and nine pure virtual functions; their names and signatures are unknown.

#include "types.h"

class book {
public:
    book();
    virtual ~book();
    virtual void vf0C() = 0;
    virtual void vf10() = 0;
    virtual void vf14() = 0;
    virtual void vf18() = 0;
    virtual void vf1C() = 0;
    virtual void vf20() = 0;
    virtual void vf24() = 0;
    virtual void vf28() = 0;
    virtual void vf2C() = 0;

    /* 0x0 */ // vtable
    /* 0x4 */ void* m4;
};

#endif // GAME_BOOK_H
