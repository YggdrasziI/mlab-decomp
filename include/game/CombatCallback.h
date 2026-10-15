#ifndef GAME_COMBATCALLBACK_H
#define GAME_COMBATCALLBACK_H

// Classes "XBaseCombatCallback" and "CombatOMaticDamageCallback" (names from RTTI).
// The base class's functions are at 0x8020F228-0x8020F2A4 (unit not split yet; its
// vtable is at 0x80388E18). Real file names unknown; method names unknown
// (vfXX = vtable offset).

#include "types.h"

class XBaseCombatCallback {
public:
    XBaseCombatCallback(int type);
    virtual ~XBaseCombatCallback();
    virtual void vf0C() = 0;
    virtual void vf10() = 0;
    virtual void vf14();

    /* 0x00 */ // vtable
    /* 0x04 */ void* m4;
    /* 0x08 */ int m8;
};

class CombatOMaticDamageCallback : public XBaseCombatCallback {
public:
    CombatOMaticDamageCallback(u8 arg);
    virtual ~CombatOMaticDamageCallback();
    virtual void vf0C();
    virtual void vf10();

    /* 0x0C */ u8 mC;
    /* 0x0D */ u8 mD;
};

#endif // GAME_COMBATCALLBACK_H
