// Game unit at 0x80042C18-0x80042C38 (real file name unknown): no code besides the
// static initializer of the shared vector. Bounds come from the static initializers
// of this unit and the previous one.

#include "game/unk_vec3.h"

// NONMATCHING (link only): code and data match, but nothing references this object
// and its static initializer is a local symbol, so the linker drops the whole object
// (FORCEACTIVE cannot name a local symbol). The original linker kept it.
