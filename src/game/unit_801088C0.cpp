// Game unit at 0x801088C0-0x801091B4 (real file name unknown). Bounds: the previous
// unit's static initializer and its .ctors entry end before 0x801088C0; this unit's
// static initializer (0x80108CB4) registers the eight inline destructors that follow
// it, up to 0x801091B4.
//
// Not linkable yet: this build emits the eight destructors in reverse order of
// registration (0x759C, 0x747C, 0x7464, ... 0x73EC by object address), the DOL in the
// order 0x759C, 0x73EC, 0x7464, 0x744C, 0x7434, 0x741C, 0x7404, 0x747C. Only
// defining the objects in another order gives it, which breaks .bss; class order,
// extern declarations, -ipa file and deferred inlining change nothing.

#include "game/unk_vec3.mch"
#include "game/unk_vec3.h"

// Memory layout settings returned by fn_801D2BF8 (real type unknown): region sizes
// followed by region counts.
struct UnkMemConfig {
    /* 0x00 */ u32 m00;
    /* 0x04 */ u32 m04;
    /* 0x08 */ u32 m08;
    /* 0x0C */ u32 m0C;
    /* 0x10 */ u32 m10;
    /* 0x14 */ u32 m14;
    /* 0x18 */ u32 m18;
    /* 0x1C */ u32 m1C;
    /* 0x20 */ u32 m20;
    /* 0x24 */ u32 m24;
    /* 0x28 */ u32 m28;
    /* 0x2C */ u32 m2C;
    /* 0x30 */ u32 m30;
    /* 0x34 */ u32 m34;
    /* 0x38 */ u32 m38;
    /* 0x3C */ u32 m3C;
    /* 0x40 */ u32 m40;
    /* 0x44 */ u32 m44;
    /* 0x48 */ u32 m48;
    /* 0x4C */ u32 m4C;
    /* 0x50 */ u32 m50;
    /* 0x54 */ u32 m54;
    /* 0x58 */ u32 m58;
    /* 0x5C */ u32 m5C;
    /* 0x60 */ u32 m60;
    /* 0x64 */ u32 m64;
    /* 0x68 */ int m68;
    /* 0x6C */ int m6C;
    /* 0x70 */ int m70;
    /* 0x74 */ int m74;
    /* 0x78 */ int m78;
    /* 0x7C */ int m7C;
    /* 0x80 */ int m80;
    /* 0x84 */ int m84;
    /* 0x88 */ int m88;
    /* 0x8C */ int m8C;
    /* 0x90 */ int m90;
    /* 0x94 */ int m94;
    /* 0x98 */ int m98;
};

extern "C" UnkMemConfig* fn_801D2BF8(void);
extern "C" void* fn_80071BB4(void* a, int b, void* c);

// Array of entries allocated by the heap code (fn_801D4414 or fn_801D4650 by mKind);
// its destructor does not free it (real type unknown).
template <typename T>
struct UnkHeapArray {
    /* 0x0 */ T* mData;
    /* 0x4 */ u32 mCount;
    /* 0x8 */ u32 mKind;

    ~UnkHeapArray() {}
};

// Address range of a heap block.
struct UnkHeapRange {
    /* 0x00 */ u32 mStart;
    /* 0x04 */ u32 mEnd;
    /* 0x08 */ u8 unk8[0x18];

    bool Contains(u32 addr) const {
        bool result = false;
        if (addr >= mStart && addr < mEnd) {
            result = true;
        }
        return result;
    }
};

// Element types of the other arrays (unknown).
struct UnkHeapElem803C73EC {};
struct UnkHeapElem803C741C {};
struct UnkHeapElem803C7464 {};
struct UnkHeapElem803C747C {};
struct UnkHeapElem803C759C {};

struct UnkPnpEntry {
    /* 0x00 */ u32 unk0;
    /* 0x04 */ u32 unk4;
    /* 0x08 */ u32 unk8;
    /* 0x0C */ u32 unkC;
    /* 0x10 */ u32 unk10;
    /* 0x14 */ u32 unk14;
};

struct UnkPnpSlot {
    /* 0x0 */ u32 unk0;
    /* 0x4 */ u32 unk4;
    /* 0x8 */ u32 unk8;
    /* 0xC */ u32 unkC;

    UnkPnpSlot() : unk0(0), unk4(0), unk8(0), unkC(0) {}
};

// Block tagged 'PNP0' (real type unknown).
struct UnkPnpBlock {
    /* 0x00 */ u32 mMagic;
    /* 0x04 */ u32 unk4;
    /* 0x08 */ u32 mSlotCount;
    /* 0x0C */ u32 unkC;
    /* 0x10 */ UnkPnpEntry mEntries[2];
    /* 0x40 */ UnkPnpSlot mSlots[4];
    /* 0x80 */ u8 unk80[0x88];

    // 83.7 %: this build unrolls the loop over the two entries, the DOL keeps a ctr
    // loop (tried: the count as a member, a parameter, int or u32, a Clear method, a
    // pointer loop, an inner word loop).
    UnkPnpBlock() {
        mMagic = 0x504E5030;
        unk4 = 2;
        mSlotCount = 4;
        unkC = 0;
        for (int i = 0; i < 2; i++) {
            mEntries[i].unk0 = 0;
            mEntries[i].unk4 = 0;
            mEntries[i].unk8 = 0;
            mEntries[i].unkC = 0;
            mEntries[i].unk10 = 0;
            mEntries[i].unk14 = 0;
        }
        for (u32 i = 0; i < mSlotCount; i++) {
            mSlots[i].unk0 = 0;
            mSlots[i].unk4 = 0;
            mSlots[i].unk8 = 0;
            mSlots[i].unkC = 0;
        }
    }
};

// Heap state (real names unknown). Nothing references the first block by name.
u8 lbl_803C7390[0x50];
UnkHeapArray<UnkHeapElem803C73EC> lbl_803C73EC;
UnkHeapArray<UnkHeapRange> lbl_803C7404;
UnkHeapArray<UnkHeapElem803C741C> lbl_803C741C;
UnkHeapArray<void*> lbl_803C7434;
UnkHeapArray<u8> lbl_803C744C;
UnkHeapArray<UnkHeapElem803C7464> lbl_803C7464;
UnkHeapArray<UnkHeapElem803C747C> lbl_803C747C;
UnkPnpBlock lbl_803C7488;
UnkHeapArray<UnkHeapElem803C759C> lbl_803C759C;

// Byte flags (real names unknown): lbl_805FB210 is used by the unit at 0x801268C8,
// lbl_805FB211 is set once the heap is set up. dtk only accepts an aligned .sbss
// split start, so both belong to the same unit.
u8 lbl_805FB210;
u8 lbl_805FB211;

extern "C" void* fn_801088C0(void* a, int b, void* c) {
    void* result = fn_80071BB4(a, b, c);
    if (result != 0) {
        return result;
    }
    return 0;
}

// Number of heap blocks minus one (real name unknown). The += form keeps the DOL's
// order of the additions.
static inline int GetBlockCount() {
    int n = fn_801D2BF8()->m68 + fn_801D2BF8()->m80;
    n += fn_801D2BF8()->m84;
    n += fn_801D2BF8()->m88;
    n += fn_801D2BF8()->m8C;
    n += fn_801D2BF8()->m90;
    n += fn_801D2BF8()->m94;
    n += fn_801D2BF8()->m98;
    n += fn_801D2BF8()->m6C;
    n += fn_801D2BF8()->m70;
    n += fn_801D2BF8()->m74;
    n += fn_801D2BF8()->m78;
    n += fn_801D2BF8()->m7C;
    return n;
}

// Tells whether addr lies in the heap; with an index, only in that block.
// 96.3 %: the DOL adds the region sizes in another order (it combines the first two
// products and the 0x90400000 base later) and numbers the saved registers
// differently; mwcc reassociates the sum, so parentheses alone do not change it.
extern "C" int fn_801088F0(u32 addr, int index, int* outIndex) {
    if (lbl_805FB211 == 0) {
        if (addr >= 0x90400000) {
            u32 a = fn_801D2BF8()->m50 * fn_801D2BF8()->m24;
            u32 b = fn_801D2BF8()->m4C * fn_801D2BF8()->m20;
            u32 c = (fn_801D2BF8()->m38 - 1) * fn_801D2BF8()->m0C;
            u32 end =
                fn_801D2BF8()->m64 * fn_801D2BF8()->m2C +
                (0x90400000 +
                 (fn_801D2BF8()->m58 * fn_801D2BF8()->m28 +
                  ((fn_801D2BF8()->m48 - 1) * fn_801D2BF8()->m1C +
                   ((fn_801D2BF8()->m44 - 1) * fn_801D2BF8()->m18 +
                    ((fn_801D2BF8()->m40 - 1) * fn_801D2BF8()->m14 +
                     ((fn_801D2BF8()->m3C - 1) * fn_801D2BF8()->m10 +
                      ((fn_801D2BF8()->m34 * fn_801D2BF8()->m08 + fn_801D2BF8()->m04) +
                       (c + fn_801D2BF8()->m00))))) +
                   (b + a))));
            if (addr <= end) {
                return 1;
            }
        }
    } else if (index == -1) {
        for (int i = 0; i < GetBlockCount() + 1; i++) {
            if (lbl_803C7434.mData[i] != 0 && lbl_803C744C.mData[i] != 0 &&
                lbl_803C7404.mData[i].Contains(addr)) {
                if (outIndex != 0) {
                    *outIndex = i;
                }
                return 1;
            }
        }
    } else if (index < GetBlockCount() + 1 && lbl_803C7434.mData[index] != 0 &&
               lbl_803C744C.mData[index] != 0 && lbl_803C7404.mData[index].Contains(addr)) {
        if (outIndex != 0) {
            *outIndex = index;
        }
        return 1;
    }
    return 0;
}
