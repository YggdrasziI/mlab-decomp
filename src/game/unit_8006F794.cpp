// Game unit at 0x8006F794-0x8006FA44 (real file name unknown): only a binary tree
// node destructor (probably an MSL map/set instance used by the unit at 0x8009F66C;
// its real name is unknown) and the static initializer. Bounds come from the static
// initializers of this unit and the previous one.

#include "game/unk_vec3.h"

// The DOL inlines the recursion three levels deep; this build needs inline_depth(3)
// for that (the original flags or source form are unknown).
#pragma inline_depth(3)

// Node of a binary tree (real type unknown).
struct UnkTreeNode {
    /* 0x0 */ UnkTreeNode* mLeft;
    /* 0x4 */ UnkTreeNode* mRight;
};

extern "C" void fn_8006F794(void* tree, UnkTreeNode* node) {
    if (node->mLeft != 0) {
        fn_8006F794(tree, node->mLeft);
    }
    if (node->mRight != 0) {
        fn_8006F794(tree, node->mRight);
    }
    delete node;
}
