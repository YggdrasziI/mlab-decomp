// From DarkRTA/rb3 src/sdk/RVL_SDK/src/nand/NANDSecret.c (commit b9dfa78b, no license
// file; reuse allowed by the project owner). Only the function present in this DOL is
// kept; includes adapted to this tree.
#include <revolution/nand.h>
#include <revolution/os.h>

s32 NANDSecretGetUsage(const char* path, s32* fsBlock, s32* inode) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }
    else {
        char absPath[64] = "";
        nandGenerateAbsPath(absPath, path);
        return nandConvertErrorCode(ISFS_GetUsage(absPath, fsBlock, inode));
    }
}
