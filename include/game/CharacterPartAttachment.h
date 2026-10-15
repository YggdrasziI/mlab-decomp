#ifndef GAME_CHARACTERPARTATTACHMENT_H
#define GAME_CHARACTERPARTATTACHMENT_H

// Classes "XCharacterPartAttachment" and "CharacterPartAttachment" (names from
// RTTI). The base class's functions are at 0x8020E9CC-0x8020EA34 (unit not split
// yet); the derived class is in the game unit at 0x80049628-0x800496DC. Real file
// names unknown; method names unknown (vfXX = vtable offset).

#include "types.h"

class XCharacterPartAttachment {
public:
    XCharacterPartAttachment(void* arg);
    virtual ~XCharacterPartAttachment();
    // Returns entry `index` of a static table of names (0x80388D70).
    virtual const char* vf0C(int index);

    /* 0x0 */ // vtable
    /* 0x4 */ void* m4;
};

// 8 bytes, built with new by fn_8004B5EC's unit.
class CharacterPartAttachment : public XCharacterPartAttachment {
public:
    CharacterPartAttachment(void* arg);
    virtual ~CharacterPartAttachment();
};

#endif // GAME_CHARACTERPARTATTACHMENT_H
