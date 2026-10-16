#ifndef GAME_UNK_ACTOR_H
#define GAME_UNK_ACTOR_H

#include "game/unk_vec3.h"
#include "types.h"

// Game actor whose vtable pointer is at 0x138 (real type unknown). Used by the sewer
// items and the load progress items.
struct UnkActorBase {
    /* 0x00 */ void* m0;
    /* 0x04 */ u8 unk4[0x10 - 0x4];
    /* 0x10 */ int m10;
    /* 0x14 */ u8 unk14[0x64 - 0x14];
    /* 0x64 */ u32 m64;
    /* 0x68 */ UnkVec3 mPos;
    /* 0x74 */ UnkVec3 m74;
    /* 0x80 */ u8 unk80[0xA0 - 0x80];
    /* 0xA0 */ f32 mA0;
    /* 0xA4 */ u8 unkA4[0x138 - 0xA4];
};

class UnkActor : public UnkActorBase {
public:
    virtual void vf08();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C(int arg);
    virtual void vf30(int arg);
};

#endif // GAME_UNK_ACTOR_H
