// Game unit at 0x8003FB24-0x80040090 (real file name unknown): the debug menu object
// of DebugMenuActionSelectionCallback. The start is the end of the previous unit (its
// static initializer at 0x8003FB04); the end is this unit's static initializer.

#include "game/unk_vec3.mch"
#include "game/ActionSelectionCallback.h"
#include "game/externs.h"
#include "game/unk_vec3.h"
#include <string.h>

// Node held by the menu page; the menu keeps a pointer to its member at 0x8 (real
// types unknown).
struct UnkMenuNodeData {
    /* 0x0 */ u8 unk0[0x4];
};
struct UnkMenuNode {
    /* 0x0 */ u8 unk0[0x8];
    /* 0x8 */ UnkMenuNodeData mData;
};

struct UnkMenuPage {
    /* 0x00 */ u8 unk0[0x14];
    /* 0x14 */ UnkMenuNode* m14;
};

// One line of a menu (real type unknown).
struct UnkMenuItem {
    /* 0x00 */ u8 unk0[0x60];
    /* 0x60 */ char mLabel[0x6C];
    /* 0xCC */ s32 mCC;
    /* 0xD0 */ u8 unkD0[0x4];
    /* 0xD4 */ UnkDebugMenuEntry* mEntry;
};

typedef void (*UnkMenuCallback)(UnkMenuItem* item);

// Menu object (0x70 bytes, functions at 0x801B373C-0x801B3940; real type unknown).
class UnkMenu {
public:
    UnkMenu();
    ~UnkMenu();
    void fn_801B381C(const char* label, int a, int b, int c, UnkMenuCallback callback, void* sub,
                     void* user);
    void fn_801B38BC();
    UnkMenuItem* fn_801B3938();

    /* 0x00 */ char mTitle[0x60];
    /* 0x60 */ UnkMenuPage* m60;
    /* 0x64 */ UnkMenuNodeData* m64;
    /* 0x68 */ u8 unk68[0x70 - 0x68];
};

// Global objects (real types unknown).
struct UnkMenuOwner {
    /* 0x000 */ u8 unk0[0x260];
    /* 0x260 */ void* m260;
};

struct UnkActionData {
    /* 0x00 */ u32 m0;
    /* 0x04 */ u32 mNameHash;
    /* 0x08 */ u32 m8;
    /* 0x0C */ s16 mC;
    /* 0x0E */ s16 mDamage;
    /* 0x10 */ s8 mCost;
    /* 0x11 */ u8 unk11[3];
};

struct UnkActionTable {
    /* 0x000 */ u8 unk0[0x430];
    /* 0x430 */ UnkActionData* mActions;
};

struct UnkActionDatabase {
    /* 0x0 */ u8 unk0[0xC];
    /* 0xC */ UnkActionTable* mData;
};

struct UnkActionOwner {
    /* 0x0 */ u32 mId;
};

struct UnkActionInfo {
    /* 0x0 */ UnkActionOwner* m0;
    /* 0x4 */ u32 m4;
};

// One action available to the fighter (real type unknown).
struct UnkActionSlot {
    /* 0x0 */ UnkActionInfo* mInfo;
    /* 0x4 */ s32 mIndex;
};

// Object given as m8 of the callback (real type unknown).
struct UnkFighter {
    /* 0x00 */ u8 unk0[0x20];
    /* 0x20 */ u8 m20[0x24];
    /* 0x44 */ s32 m44;
};

extern "C" {
extern UnkMenuOwner* lbl_805FB058;
extern UnkActionDatabase* lbl_805FBBC0;

void fn_80040EFC(int index, char* buf);
int fn_8021415C(void* list, UnkActionSlot* slots, int max, int arg);
void fn_80216838(char* buf, UnkActionDatabase* db, u32 id);
const char* fn_8020E310(u32 hash);
const char* fn_8020E438(u32 id);
u8 fn_8021EF70(UnkActionOwner* owner);
void fn_8003FF7C(UnkMenuItem* item);
}

UnkDebugMenu::UnkDebugMenu() : m84(0), mHasSelection(0), m98(0) {
    for (int i = 0; i < 10; i++) {
        mEntries[i].mOwner = this;
    }
}

UnkDebugMenu::~UnkDebugMenu() {
    delete m84;
}

void UnkDebugMenu::fn_8003FC00(void* arg) {
    UnkMenu* menu = new UnkMenu;
    m84 = menu;
    strncpy(menu->mTitle, "Action Selection Menu", 0x5F);
    menu->mTitle[0x5F] = 0;
}

void UnkDebugMenu::fn_8003FC6C(void* arg) {
    UnkFighter* fighter = (UnkFighter*)arg;
    char header[0x100];
    char line[0x100];
    UnkActionSlot slots[11];
    char typeName[0x80];
    char actionName[0x80];
    char info[0x50];
    char name[0x40];

    m84->fn_801B38BC();
    m84->fn_801B381C("Other options", 0, 0, 0, 0, lbl_805FB058->m260, this);
    fn_80040EFC(fighter->m44, name);
    fn_80142EF8(header, "-----Actions - select for %s-----", name);
    m84->fn_801B381C(header, 0, 0, 0, 0, 0, this);
    int count = fn_8021415C(fighter->m20, slots, 10, 0);
    bool found = false;
    UnkMenuNode* node = m84->m60->m14;
    UnkMenuNodeData* current;
    if (node) {
        current = &node->mData;
    } else {
        current = 0;
    }
    for (int i = 0; i < count; i++) {
        UnkActionInfo* action = slots[i].mInfo;
        u32 id = action->m0->mId;
        fn_80216838(info, lbl_805FBBC0, id);
        UnkActionData data = lbl_805FBBC0->mData->mActions[slots[i].mIndex];
        fn_80142EF8(typeName, "%s", fn_8020E310(data.mNameHash));
        fn_80142EF8(actionName, "%s", fn_8020E438(id));
        s32 cost = data.mCost;
        s32 damage = data.mDamage;
        if (action->m0 != 0) {
            u8 scale = fn_8021EF70(action->m0);
            cost = (f32)(cost * scale) / 100.0f;
            damage = (f32)(damage * scale) / 100.0f;
        }
        fn_80142EF8(line, "%s - %s cost(%d) dmg(%d)", actionName, typeName, cost, damage);
        mEntries[i].mAction = slots[i].mInfo;
        mEntries[i].mIndex = slots[i].mIndex;
        m84->fn_801B381C(line, 0, 0, 0, fn_8003FF7C, 0, &mEntries[i]);
        if (strcmp(m84->fn_801B3938()->mLabel, line) == 0) {
            found = true;
        }
    }
    if (!found) {
        m84->m64 = current;
    }
}

void UnkDebugMenu::fn_8003FF30() {
    if (lbl_8047C7D8.m0 == 0) {
        lbl_8047C7D8.fn_801B4090(m84);
        m98 = 1;
    }
}

extern "C" void fn_8003FF7C(UnkMenuItem* item) {
    UnkDebugMenuEntry* entry = item->mEntry;
    UnkDebugMenu* owner = entry->mOwner;
    owner->mSelection.m0 = entry->mIndex;
    owner->mSelection.m8 = ((UnkActionInfo*)entry->mAction)->m4;
    owner->mHasSelection = 1;
}

u32 UnkDebugMenu::fn_8003FFA4(UnkActionSelection* selection) {
    if (mHasSelection == 1) {
        selection->m0 = mSelection.m0;
        selection->m4 = mSelection.m4;
        selection->m8 = mSelection.m8;
    }
    return mHasSelection;
}

s32 UnkDebugMenu::fn_8003FFD0() const {
    UnkMenuItem* item = m84->fn_801B3938();
    if (item == 0) {
        return -1;
    }
    if (item->mEntry == 0 || item->mCC == 0) {
        return -1;
    }
    return item->mEntry->mIndex;
}

void UnkDebugMenu::fn_80040028() {
    mHasSelection = 0;
    lbl_8047C7D8.fn_801B40E4();
    m98 = 0;
}
