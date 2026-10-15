#ifndef GAME_BUGCACHINGCHALLENGE_H
#define GAME_BUGCACHINGCHALLENGE_H

// Classes of the bug-catching challenge (names from RTTI: "BugCachingChallenge::Bug",
// "BugCachingChallenge::ButterFly"). Bug's functions are at 0x800293C8-0x8002A2xx
// (unit not split yet; vtable at 0x8035763C). Real file names unknown; method names
// and signatures unknown (vfXX = vtable offset). Whether BugCachingChallenge is a
// namespace or a class is unknown (both give the same RTTI name and mangling).

#include "types.h"

namespace BugCachingChallenge {

class Bug {
public:
    Bug();
    virtual ~Bug();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();

    /* 0x0 */ // vtable
    /* 0x4 */ f32 m4;
};

class ButterFly : public Bug {
public:
    ButterFly();
    virtual ~ButterFly();
};

} // namespace BugCachingChallenge

#endif // GAME_BUGCACHINGCHALLENGE_H
