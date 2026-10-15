#ifndef GAME_EXTERNS_H
#define GAME_EXTERNS_H

// Game functions called from decompiled units but not decompiled yet. Names and
// signatures are unknown; the signatures follow the register use at the call sites.
// extern "C" keeps the address-based names of symbols.txt.

#include "types.h"

// Formats into a buffer, printf-style (called with a format string and varargs).
extern "C" void fn_80142EF8(char* buf, const char* fmt, ...);

#endif // GAME_EXTERNS_H
