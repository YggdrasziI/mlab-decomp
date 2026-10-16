// Game unit at 0x80006B40-0x80006B60 (real file name unknown): no code besides the
// static initializer of the shared vector. Bounds come from the static initializers
// of this unit and the next one; it is the first unit of .text.

#include "game/unk_vec3.h"

// Nothing references this object and its static initializer is a local symbol
// (FORCEACTIVE cannot name it), so the linker would drop it: the linker script
// template config/RM7P4F/ldscript.tpl keeps it with FORCEFILES.
