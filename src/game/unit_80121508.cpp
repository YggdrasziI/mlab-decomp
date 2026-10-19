// Game unit at 0x80121508-0x80121C90 (real file name unknown): on-screen debug text.
// Two queues of text items (the second one used while bit 1 of lbl_803C4298.m3C is
// set) with 0x400-byte string buffers in the block at lbl_803C2A20: adding an item,
// drawing (with left, centre or right alignment) and clearing. Also the methods of
// Unk8012148C forwarded from unit 0x8012148C. Bounds come from the static initializers
// of this unit and the previous one.

#include "game/unk_vec3.mch"

#include "game/Unk8012148C.h"
#include "game/unk_vec3.h"

// Queued text item (real type unknown).
struct UnkDebugTextItem {
    /* 0x00 */ const char* mText;
    /* 0x04 */ f32 mX;
    /* 0x08 */ f32 mY;
    /* 0x0C */ f32 mScaleX;
    /* 0x10 */ f32 mScaleY;
    /* 0x14 */ u32 mColor;
    /* 0x18 */ int mAlign; // 1: centred, 2: right-aligned
    /* 0x1C */ int m1C;
};

// Node of a pool's list of allocated items; the item follows the two words.
struct UnkPoolNode {
    /* 0x0 */ u32 m0;
    /* 0x4 */ UnkPoolNode* mNext;
    /* 0x8 */ UnkDebugTextItem mItem;
};

// Item pool (real type unknown).
struct UnkItemPool {
    /* 0x00 */ u8 unk0[0x14];
    /* 0x14 */ UnkPoolNode* mFirst;
};

struct UnkGlobals803C2A20 {
    /* 0x000 */ u8 unk0[0x10];
    /* 0x010 */ UnkItemPool* mPool;
    /* 0x014 */ char mText[0x400];
    /* 0x414 */ u32 mTextUsed;
    /* 0x418 */ UnkItemPool* mPool2;
    /* 0x41C */ char mText2[0x400];
    /* 0x81C */ u32 mText2Used;
    /* 0x820 */ u8 mEnabled;
};

struct UnkGlobals803C4298 {
    /* 0x00 */ u8 unk0[0x3C];
    /* 0x3C */ u32 unk3C : 30;
    u32 mSecondQueue : 1;
    u32 unk3C_31 : 1;
};

// Screen size (real type unknown).
struct UnkScreenSize {
    /* 0x0 */ int mWidth;
    /* 0x4 */ int mHeight;
    /* 0x8 */ u8 unk8[0x18 - 0x8];
};

extern UnkGlobals803C2A20 lbl_803C2A20;
extern UnkGlobals803C4298 lbl_803C4298;
extern UnkScreenSize lbl_80412BE0;

// Position and color used by fn_80188F00 (.sbss, shared with other units).
extern int lbl_805FB628;
extern int lbl_805FB62C;
extern u32 lbl_805FB630;

extern "C" {
unsigned long strlen(const char* s);
void fn_80188EDC(const char* text, int* width, int* height);
void fn_80188F00(const char* format, ...);
UnkDebugTextItem* fn_801EB94C(UnkItemPool* pool);
void fn_801EBA20(UnkItemPool* pool, UnkDebugTextItem* item);

void fn_80121508(char* text, int x, int y, u32 color, int align, int arg, f32 scaleX, f32 scaleY);
void fn_80121898(void);
void fn_80121B08(UnkDebugTextItem* item);
}

inline UnkDebugTextItem* UnkFirstItem(UnkItemPool* pool) {
    return pool->mFirst != 0 ? &pool->mFirst->mItem : 0;
}

inline UnkDebugTextItem* UnkNextItem(UnkDebugTextItem* item) {
    UnkPoolNode* next = ((UnkPoolNode*)((u8*)item - 8))->mNext;
    return next != 0 ? &next->mItem : 0;
}

void fn_80121508(char* text, int x, int y, u32 color, int align, int arg, f32 scaleX, f32 scaleY) {
#define g (&lbl_803C2A20)
    if (g->mPool == 0 || !g->mEnabled) {
        return;
    }
    UnkDebugTextItem* item;
    if (lbl_803C4298.mSecondQueue) {
        u32 len = strlen(text);
        if (len + g->mText2Used + 1 > 0x400) {
            return;
        }
        item = fn_801EB94C(g->mPool2);
        if (item == 0) {
            return;
        }
        item->mText = &g->mText2[g->mText2Used];
        for (int i = 0; i < 64; i++) {
            g->mText2[g->mText2Used++] = text[i];
            if (text[i] == 0) {
                break;
            }
            if (i == 64) {
                g->mText2[g->mText2Used] = 0;
                break;
            }
        }
    } else {
        u32 len = strlen(text);
        if (len + g->mTextUsed + 1 > 0x400) {
            return;
        }
        item = fn_801EB94C(g->mPool);
        if (item == 0) {
            return;
        }
        item->mText = &g->mText[g->mTextUsed];
        for (int i = 0; i < 64; i++) {
            g->mText[g->mTextUsed++] = text[i];
            if (text[i] == 0) {
                break;
            }
            if (i == 64) {
                g->mText[g->mTextUsed] = 0;
                break;
            }
        }
    }
    item->mX = -0.6666667f + 0.0020833334f * x;
    item->mY = 0.5f - 0.0020833334f * y;
    item->mScaleX = scaleX;
    item->mScaleY = scaleY;
    item->mColor = color;
    item->mAlign = align;
    item->m1C = arg;
#undef g
}

void fn_80121898(void) {}

// 98.8 %: the DOL keeps item in r29 in both loops (next in r28, then r30); this
// build swaps item and next in each loop (block scopes, declaration orders and a
// separate cursor tried).
void Unk8012148C::fn_8012189C() {
    UnkDebugTextItem* item = UnkFirstItem(lbl_803C2A20.mPool);
    while (item != 0) {
        UnkDebugTextItem* next = UnkNextItem(item);
        fn_80121B08(item);
        if (!(lbl_803C4298.mSecondQueue)) {
            fn_801EBA20(lbl_803C2A20.mPool, item);
        }
        item = next;
    }
    if (!(lbl_803C4298.mSecondQueue)) {
        lbl_803C2A20.mTextUsed = 0;
    }
    item = UnkFirstItem(lbl_803C2A20.mPool2);
    while (item != 0) {
        UnkDebugTextItem* next = UnkNextItem(item);
        fn_80121B08(item);
        fn_801EBA20(lbl_803C2A20.mPool2, item);
        item = next;
    }
    lbl_803C2A20.mText2Used = 0;
}

void Unk8012148C::fn_801219F0(f32 a, f32 b) {}

void Unk8012148C::fn_801219F4() {
    if (lbl_803C2A20.mPool != 0) {
        UnkDebugTextItem* item = UnkFirstItem(lbl_803C2A20.mPool);
        while (item != 0) {
            UnkDebugTextItem* next = UnkNextItem(item);
            fn_801EBA20(lbl_803C2A20.mPool, item);
            item = next;
        }
        lbl_803C2A20.mTextUsed = 0;
    }
    if (lbl_803C2A20.mPool2 != 0) {
        UnkDebugTextItem* item = UnkFirstItem(lbl_803C2A20.mPool2);
        while (item != 0) {
            UnkDebugTextItem* next = UnkNextItem(item);
            fn_801EBA20(lbl_803C2A20.mPool2, item);
            item = next;
        }
        lbl_803C2A20.mText2Used = 0;
    }
}

void fn_80121B08(UnkDebugTextItem* item) {
    f32 halfWidth = (f32)lbl_80412BE0.mWidth * 0.5f;
    f32 scale = 1.6f * halfWidth;
    f32 x = halfWidth + item->mX * scale;
    f32 y = (f32)lbl_80412BE0.mHeight * 0.5f + item->mY * -scale;
    int width;
    int height;
    fn_80188EDC(item->mText, &width, &height);
    switch (item->mAlign) {
    case 0:
    default:
        break;
    case 1:
        x -= width / 2;
        break;
    case 2:
        x -= width;
        break;
    }
    lbl_805FB628 = x;
    lbl_805FB62C = y;
    lbl_805FB630 = item->mColor;
    fn_80188F00("%s", item->mText);
}
