// Game unit at 0x800C407C-0x800C42B8 (real file name unknown): the ExWorldWireTile
// class. Bounds come from the static initializers of this unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/Entity.h"
#include "game/unk_803E6D00.h"
#include "game/unk_vec3.h"

// Stride of the UnkGlobals803E6D00Sub::mWireGroups array (real name unknown).
extern int lbl_805FB94C;

// Length of the segment `index` of a wire (real name unknown).
extern "C" f32 fn_8010B05C(UnkWire* wire, int index);

static inline UnkWireGroup* GetWireGroup(int index) {
    return (UnkWireGroup*)(lbl_803E6D00.m0->mWireGroups + index * lbl_805FB94C);
}

ExWorldWireTile::ExWorldWireTile(int type, EntityInfo* info, int init) : ExWorldTile(type, info, init) {
    mWire = 0;
}

ExWorldWireTile::~ExWorldWireTile() {}

// NonMatching (87 %): our build turns the bone loop into a ctr loop with the count
// loaded once; the DOL reloads m94.m0 every iteration and reloads lbl_803E6D00.m0 at
// the top of the outer loop.
void ExWorldWireTile::vfBC() {
    ExWorldTile::vfBC();
    mWire = 0;
    bool found = false;
    for (int i = 0; i <= lbl_803E6D00.m0->m4; i++) {
        UnkWire* wires = GetWireGroup(i)->mWires;
        for (u8 j = 0; j < GetWireGroup(i)->mWireCount; j++) {
            for (int k = 0; k < (int)m94.m0; k++) {
                if (wires[j].mId == m94.mBones[k].m0) {
                    mWire = &wires[j];
                    found = true;
                    mWire->mFlags &= ~1;
                    break;
                }
            }
            if (found) {
                break;
            }
        }
        if (found) {
            break;
        }
    }
}

void ExWorldWireTile::vfC0() {
    ExWorldTile::vfC0();
}

f32 ExWorldWireTile::vfD0() const {
    f32 length = 0.0f;
    for (int i = 0; i != mWire->mPointCount - 1; i++) {
        length += fn_8010B05C(mWire, i);
    }
    return length;
}
