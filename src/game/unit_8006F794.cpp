// Game unit at 0x8006F794-0x8006FA44 (real file name unknown): only the node
// destruction of the tile multimap of the buzz saw experiment (an MSL tree function
// instantiated here first; the unit at 0x8009F66C calls it) and the static
// initializer. Bounds come from the static initializers of this unit and the previous
// one.
//
// Built with -ipa file like the unit at 0x8009F66C: the recursion is then inlined
// three levels deep, as in the DOL.

#include "game/unk_vec3.mch"
#include "game/BuzzSawExperiment.h"
#include "game/unk_vec3.h"

template void std::__tree_base3<UnkSawEntry, UnkSawLess, std::allocator<UnkSawEntry>, 1>::destroy(
    std::__tree_base3<UnkSawEntry, UnkSawLess, std::allocator<UnkSawEntry>, 1>::node* p);
