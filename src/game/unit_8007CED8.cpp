// Game unit at 0x8007CED8-0x8007D4DC (real file name unknown): a meter from 0 to 100
// that rises with the cells of an image that were hit and falls with those missed,
// and shows its level on five screen actors (real names unknown). Bounds come from the
// static initializers of the previous unit (0x8007CEB8) and of this unit
// (0x8007D4BC); the unit at 0x80077D1C (not split) calls every function but
// fn_8007D004, which game/unit_8007C4AC.cpp calls.

#include "game/unk_vec3.mch"
#include "game/unk_actor.h"
#include "game/unk_vec3.h"

// Image description passed to fn_8007D004 (same layout as in game/unit_8007C4AC.cpp).
struct UnkImageInfo {
    /* 0x0 */ u16 mWidth;
    /* 0x2 */ u16 mHeight;
};

// Owner of the meter (returned by fn_80078BB0; real type unknown).
struct UnkMeterOwner {
    /* 0x000 */ u8 unk0[0x75C];
    /* 0x75C */ void* m75C;
};

// Screen set returned by fn_8018D1E4: its first word is the hash of its name.
struct UnkScreenSet {
    /* 0x0 */ u32 mHash;
};

class UnkMeter {
public:
    UnkMeter();
    ~UnkMeter();
    f32 fn_8007CF94();
    f32 fn_8007CFA4();
    f32 fn_8007CFAC();
    f32 fn_8007CFD8();
    void fn_8007D24C();
    void fn_8007D2BC(f32 scale);
    void fn_8007D2C4();
    void fn_8007D2C8();

    /* 0x00 */ f32 mScale;
    /* 0x04 */ int mCount;
    /* 0x08 */ int mHits;
    /* 0x0C */ int mTotalCount;
    /* 0x10 */ int mTotalHits;
    /* 0x14 */ f32 mLevel;
    /* 0x18 */ f32 mGain;
    /* 0x1C */ f32 mLoss;
    /* 0x20 */ int mShown;
};

// Gain and loss per difficulty (index from fn_80070458), and the color of the shown
// actors. They sit in .rodata among the objects of the unit at 0x80077D1C, so they
// are not this unit's.
extern const f32 lbl_803474D8[4];
extern const f32 lbl_803474E8[4];
extern const f32 lbl_803474C8[4];

// Text hashes of the five actors (.rodata, unsplit: the .rodata of the game units
// cannot be split yet, see notes/SPLITS.md). Not const here: the DOL stores mShown
// before it copies the table, as if the copy could alias the meter (a const table
// lets the store move after the copy).
struct UnkMeterHashes {
    u32 m[5];
};
extern UnkMeterHashes lbl_80347D60;

extern "C" {
extern void* lbl_805FB654;

int fn_80070458(void);
UnkMeterOwner* fn_80078BB0(void);
void fn_80070FAC(UnkMeterOwner* owner, f32 value);
f32 fn_8007C138(void* target);
void fn_800712B4(void);
void fn_800712B8(void);
u8* fn_8015A210(UnkImageInfo* info, int arg1, int* pitch, int arg3);
void fn_8015A2E8(UnkImageInfo* info, int arg1, int arg2);
UnkScreenSet* fn_8018D1E4(void* obj);
UnkActor* fn_80198F18(void* screens, u32 hash);
}

UnkMeter::UnkMeter() {
    mScale = 0.0f;
    mCount = 0;
    mHits = 0;
    mTotalCount = 0;
    mTotalHits = 0;
    mLevel = 50.0f;
    mShown = 0;
    int difficulty = fn_80070458();
    mGain = lbl_803474D8[difficulty];
    mLoss = lbl_803474E8[difficulty];
}

UnkMeter::~UnkMeter() {}

f32 UnkMeter::fn_8007CF94() {
    return mLevel / 100.0f;
}

f32 UnkMeter::fn_8007CFA4() {
    return mLevel;
}

f32 UnkMeter::fn_8007CFAC() {
    return mTotalHits;
}

f32 UnkMeter::fn_8007CFD8() {
    return mTotalCount;
}

// Counts the cells of rows [row, row + count) of the image: each byte holds two cells
// of four bits (bit 0 clear: a cell to count, bit 1: hit), then clears bits 1 and 5.
// NonMatching (99.0 %): register numbering only. The DOL keeps the row pointer in r6,
// the row index in r4 and the cell pointer in r3; this build has them in r4, r5 and
// r6. Tried: the row pointer as a separate offset, as pixels += ..., as &pixels[...],
// declared before or after the counters, the loop variables and the cell pointer
// declared at the top.
extern "C" void fn_8007D004(UnkMeter* meter, UnkImageInfo* info, int row, int count) {
    if (count == 0) {
        return;
    }
    int pitch;
    u8* pixels = fn_8015A210(info, 0, &pitch, 0);
    u8* rows = pixels + pitch * row;
    meter->mCount = 0;
    meter->mHits = 0;
    int misses = 0;
    for (int y = 0; y < count; y++) {
        u8* p = rows + y * pitch;
        for (int x = 0; x < info->mWidth / 2; x++) {
            int high = *p >> 4;
            int low = *p & 0xF;
            bool highHit = (high & 2) != 0;
            if (!(high & 1)) {
                meter->mCount++;
                if (highHit) {
                    meter->mHits++;
                } else {
                    misses++;
                }
            }
            bool lowHit = (low & 2) != 0;
            if (!(low & 1)) {
                meter->mCount++;
                if (lowHit) {
                    meter->mHits++;
                } else {
                    misses++;
                }
            }
            *p &= 0xDD;
            p++;
        }
    }
    fn_8015A2E8(info, 0, 1);
    meter->mTotalCount += meter->mCount;
    meter->mTotalHits += meter->mHits;
    f32 gain = 1.0f;
    f32 loss = gain;
    if (meter->mCount > 0) {
        if (meter->mCount == meter->mHits) {
            gain = 2.0f;
            fn_800712B8();
        } else if (meter->mHits == 0) {
            loss = 4.0f;
            fn_800712B4();
        }
    }
    meter->mLevel = meter->mLevel + gain * (meter->mGain * (meter->mHits * meter->mScale)) -
                    loss * (meter->mLoss * (misses * meter->mScale));
    if (meter->mLevel > 100.0f) {
        meter->mLevel = 100.0f;
    } else if (meter->mLevel < 0.0f) {
        meter->mLevel = 0.0f;
    }
}

void UnkMeter::fn_8007D24C() {
    mCount = 0;
    mHits = 0;
    mTotalCount = 0;
    mTotalHits = 0;
    mShown = 0;
    mLevel = 50.0f;
    int difficulty = fn_80070458();
    mGain = lbl_803474D8[difficulty];
    mLoss = lbl_803474E8[difficulty];
}

void UnkMeter::fn_8007D2BC(f32 scale) {
    mScale = scale;
}

void UnkMeter::fn_8007D2C4() {}

struct UnkColor {
    UnkColor(f32 r_, f32 g_, f32 b_, f32 a_) : r(r_), g(g_), b(b_), a(a_) {}
    f32 r, g, b, a;
};

// Shows the level on the actors of the five hashes: as many as the level gives (one
// per sixth of the range, at most five) get the color, the others are hidden.
void UnkMeter::fn_8007D2C8() {
    UnkScreenSet* screens = fn_8018D1E4(lbl_805FB654);
    if (screens->mHash != 0xF611F736) {
        return;
    }
    f32 ratio = fn_8007CF94();
    fn_80070FAC(fn_80078BB0(), ratio);
    int shown = 6.0f * fn_8007C138(fn_80078BB0()->m75C);
    if (shown > 5) {
        shown = 5;
    }
    mShown = shown;
    UnkMeterHashes hashes = lbl_80347D60;
    f32 r = 0.0f;
    f32 g = r;
    f32 b = r;
    switch (shown) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        r = lbl_803474C8[0];
        g = lbl_803474C8[1];
        b = lbl_803474C8[2];
        break;
    }
    UnkActor* actor;
    int i;
    for (i = 0; i < shown; i++) {
        actor = fn_80198F18(screens, hashes.m[i]);
        if (actor != 0) {
            actor->vf2C(0x22);
            *(UnkColor*)&actor->unk80 = UnkColor(r, g, b, 1.0f);
        }
    }
    for (; shown < 5; shown++) {
        actor = fn_80198F18(screens, hashes.m[shown]);
        if (actor != 0) {
            actor->vf30(2);
        }
    }
}
