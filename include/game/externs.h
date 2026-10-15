#ifndef GAME_EXTERNS_H
#define GAME_EXTERNS_H

// Game functions called from decompiled units but not decompiled yet. Names and
// signatures are unknown; the signatures follow the register use at the call sites.
// extern "C" keeps the address-based names of symbols.txt.

#include "types.h"

// Formats into a buffer, printf-style (called with a format string and varargs).
extern "C" void fn_80142EF8(char* buf, const char* fmt, ...);

// Sends an event identified by a 32-bit hash (called about 300 times, always with the
// manager object lbl_805FB6C8 as first argument).
extern "C" void fn_801BA190(void* mgr, void* sender, int arg2, u32 hash, int arg4, int arg5, int arg6,
                            int arg7);
extern void* lbl_805FB6C8;

// Called with the object at the start of Entity::mA4 and 1.
extern "C" void fn_801BB478(void* obj, int arg);

// fn_800D5924(fn_800D39BC(lbl_805FB180)) tells whether a condition holds (meaning
// unknown).
extern "C" void* fn_800D39BC(void* obj);
extern "C" int fn_800D5924(void* obj);
extern void* lbl_805FB180;

#endif // GAME_EXTERNS_H
