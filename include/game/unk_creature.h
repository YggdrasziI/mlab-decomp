#ifndef GAME_UNK_CREATURE_H
#define GAME_UNK_CREATURE_H

// Creature entity driven by the named state callbacks of the units at 0x80014350 and
// 0x80016400 (real type unknown). The player creature is lbl_803B53A0.mD614.

#include "game/Entity.h"

class UnkCreature : public Entity {
public:
    virtual void vfB8();
    virtual void vfBC();
    virtual void vfC0();
    virtual void vfC4();
    virtual void vfC8();
    virtual void vfCC();
    virtual void vfD0();
    virtual void vfD4();
    virtual void vfD8();
    virtual void vfDC();
    virtual void vfE0();
    virtual void vfE4();
    virtual void vfE8();
    virtual void vfEC();
    virtual void* vfF0();

    u32 HasType(u32 mask) const { return mInfo->mTypeMask & mask; }
    f32 GetC44() const { return mC44; }
    f32 GetC40() const { return mC40; }

    /* 0x14C */ u8 unk14C[0x158 - 0x14C];
    /* 0x158 */ int m158;
    /* 0x15C */ u8 unk15C[0x6B0 - 0x15C];
    /* 0x6B0 */ f32 m6B0; // shake gauge
    /* 0x6B4 */ int m6B4; // shake count; also an event hash and a flag
    /* 0x6B8 */ u8 unk6B8[0x838 - 0x6B8];
    /* 0x838 */ u32 m838;
    /* 0x83C */ u8 unk83C[0x844 - 0x83C];
    /* 0x844 */ UnkVec3 m844;
    /* 0x850 */ u8 unk850[0x87C - 0x850];
    /* 0x87C */ int m87C; // patrol mode
    /* 0x880 */ int m880; // patrol loop mode
    /* 0x884 */ int* m884; // patrol path (directions, -1 terminated)
    /* 0x888 */ u32 m888;
    /* 0x88C */ int m88C; // patrol index
    /* 0x890 */ int m890; // patrol length
    /* 0x894 */ u8 m894; // patrol forward
    /* 0x895 */ u8 m895;
    /* 0x896 */ u8 unk896[0x9D4 - 0x896];
    /* 0x9D4 */ int m9D4; // inverts the directional input
    /* 0x9D8 */ u32 m9D8;
    /* 0x9DC */ u8 unk9DC[0x9E4 - 0x9DC];
    /* 0x9E4 */ f32 m9E4; // timer
    /* 0x9E8 */ UnkVec3 m9E8; // target node position
    /* 0x9F4 */ f32 m9F4;
    /* 0x9F8 */ u8 m9F8;
    /* 0x9F9 */ u8 unk9F9[0xA00 - 0x9F9];
    /* 0xA00 */ void* mA00; // movement controller
    /* 0xA04 */ u8 unkA04[0xBDF - 0xA04];
    /* 0xBDF */ u8 mBDF;
    /* 0xBE0 */ u8 unkBE0[0xC40 - 0xBE0];
    /* 0xC40 */ f32 mC40;
    /* 0xC44 */ f32 mC44;
    /* 0xC48 */ u8 unkC48[0xC54 - 0xC48];
    /* 0xC54 */ void* mC54;
    /* 0xC58 */ u8 unkC58[0xC80 - 0xC58];
    /* 0xC80 */ u8 mC80;
    /* 0xC81 */ u8 mC81;
    /* 0xC82 */ u8 mC82;
};

#endif // GAME_UNK_CREATURE_H
