// Game unit at 0x8001A17C-0x8001AFB4 (real file name unknown): script callbacks that
// forward to the current mini-game (fn_80074D48), MiniGame::vf24 and vf28, and the lab
// screen texts (progress bar, selections, statistics). Bounds: the previous unit's
// static initializer ends at 0x8001A17C and this unit's static initializer ends at
// 0x8001AFB4.

#include "game/unk_vec3.mch"

#include "game/BuzzSawExperiment.h"
#include "game/unk_803C4298.h"
#include "game/unk_803E6D00.h"
#include "game/unk_actor.h"
#include "game/unk_vec3.h"

// Selection of the lab (real type unknown).
struct UnkLabSelection {
    /* 0x00 */ int m0;
    /* 0x04 */ u8 unk4[0xC - 0x4];
    /* 0x0C */ int mC;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 m11;
};

// Statistics copied from the lab (real type unknown).
struct UnkLabStats {
    /* 0x00 */ int m0;
    /* 0x04 */ int m4;
    /* 0x08 */ int m8;
    /* 0x0C */ int mC;
    /* 0x10 */ int m10;
};

// The lab (real type unknown).
struct UnkLab {
    /* 0x00 */ u8 unk0[0x3C];
    /* 0x3C */ UnkLabSelection* mSelection;
    /* 0x40 */ u8 unk40[0x6C - 0x40];
    /* 0x6C */ UnkLabStats mStats;
};

// Text hashes of the two monsters' five slots (.rodata, unsplit: the .rodata of the
// game units cannot be split yet, see notes/SPLITS.md).
struct UnkSlotHashes {
    u32 m[2][5];
};
extern const UnkSlotHashes lbl_803476C8;

extern "C" {
extern UnkLab* lbl_805FBBC8;
extern void* lbl_805FB654;

MiniGame* fn_80074D48(void);
u32 fn_80074D50(void);
void fn_80074D74(void);
Experiment* fn_80021750(void);

void fn_80142EF8(char* buf, const char* fmt, ...);
void* fn_8018D1E4(void* obj);
UnkActor* fn_80198F18(void* screens, u32 hash);
void fn_8018F618(u32 hash, int visible);
void fn_80190A60(u32 hash, int arg);
void fn_80190E48(u32 hash, const char* text);
void fn_80190E50(u32 hash, int value);
void fn_801903CC(u32 hash, u32 arg);
void fn_801911C8(u32 hash, int value);
void fn_80191B68(u32 hash, int value);
void fn_801A3FA0(UnkActor* actor, f32 value);
void fn_801A403C(UnkActor* actor, f32 value);
void fn_801A4050(UnkActor* actor, f32 value);
void fn_801A4078(UnkActor* actor, f32 value);
f32 fn_801A4084(UnkActor* actor);
void fn_800F9790(int arg);
int fn_800F98B8(void);
int fn_800F98F0(void);
int fn_800F99B4(void);
int fn_800F9A48(void);
int fn_800F9AE4(void);
int fn_800F9B08(void);
int fn_800F9B2C(void);
int fn_800F9B44(int index);
int fn_800F9C7C(int index);
int fn_800F9E0C(int index);
int fn_800F9EEC(int index);
int fn_800FA174(int index, int slot);
int fn_800FA350(void);
int fn_800FA358(void);
int fn_800FA3CC(void);
int fn_800FA440(void);
void fn_8021947C(UnkLab* lab, int arg1, int arg2);
int fn_802191CC(UnkLab* lab, int arg1, int arg2);
}

extern "C" void fn_8001A17C(void) {
    if (fn_80074D50() == 1) {
        fn_80074D74();
    } else {
        MiniGame* game = fn_80074D48();
        if (game != 0) {
            game->vf0C();
        }
    }
}

extern "C" void fn_8001A1C8(void) {
    MiniGame* game = fn_80074D48();
    if (game != 0 && lbl_803C4298.m18 == 5) {
        game->vf14(lbl_803E6D00.mDeltaTime);
        game->vf44();
    }
}

extern "C" void fn_8001A240(void) {}

extern "C" void fn_8001A244(void) {
    if (fn_80074D50() == 1) {
        fn_80074D74();
    } else {
        MiniGame* game = fn_80074D48();
        if (game != 0) {
            game->vf10();
        }
    }
}

extern "C" void fn_8001A290(void) {
    if (fn_80074D50() == 1) {
        if (fn_80074D48() != 0) {
            fn_80074D48()->vf28();
        }
        fn_80074D74();
    } else {
        MiniGame* game = fn_80074D48();
        if (game != 0) {
            game->vf18();
        }
    }
}

void MiniGame::vf28() {}

extern "C" void fn_8001A300(void) {
    MiniGame* game = fn_80074D48();
    if (game != 0 && lbl_803C4298.m18 == 5) {
        game->vf20(lbl_803E6D00.mDeltaTime);
        game->vf44();
    }
}

extern "C" void fn_8001A378(void) {
    if (fn_80074D50() == 1) {
        if (fn_80074D48() != 0) {
            fn_80074D48()->vf24();
        }
        fn_80074D74();
    } else {
        MiniGame* game = fn_80074D48();
        if (game != 0) {
            game->vf1C();
        }
    }
}

void MiniGame::vf24() {}

extern "C" void fn_8001A3E8(void) {
    if (fn_80074D50() == 1) {
        fn_80074D74();
    } else {
        MiniGame* game = fn_80074D48();
        if (game != 0) {
            game->vf2C();
        }
    }
}

extern "C" void fn_8001A434(void) {
    if (fn_80074D50() == 1) {
        fn_80074D74();
    } else {
        MiniGame* game = fn_80074D48();
        if (game != 0) {
            game->vf38();
        }
    }
}

extern "C" void fn_8001A480(void) {
    MiniGame* game = fn_80074D48();
    if (game != 0 && lbl_803C4298.m18 == 5) {
        game->vf34(lbl_803E6D00.mDeltaTime);
        game->vf44();
    }
}

extern "C" void fn_8001A4F8(void) {
    MiniGame* game = fn_80074D48();
    if (game != 0 && lbl_803C4298.m18 == 5) {
        game->vf40(lbl_803E6D00.mDeltaTime);
        game->vf44();
    }
}

extern "C" void fn_8001A570(void) {
    Experiment* experiment = fn_80021750();
    if (experiment != 0) {
        experiment->vf54();
    }
}

extern "C" void fn_8001A5A8(void) {
    if (fn_80074D50() == 1) {
        fn_80074D74();
    } else {
        MiniGame* game = fn_80074D48();
        if (game != 0) {
            game->vf30();
        }
    }
}

extern "C" void fn_8001A5F4(void) {
    if (fn_80074D50() == 1) {
        fn_80074D74();
    } else {
        MiniGame* game = fn_80074D48();
        if (game != 0) {
            game->vf3C();
        }
    }
}

extern "C" void fn_8001A640(void) {
    MiniGame* game = fn_80074D48();
    if (game != 0) {
        game->vf44();
    }
}

extern "C" void fn_8001A678(void) {
    MiniGame* game = fn_80074D48();
    if (game != 0) {
        game->vf44();
    }
}

// Fills the lab screen.
// 99.7 %: the selection pointer takes r28 and the three slot array pointers r29-r31;
// the DOL gives the selection r31 (declaration orders tried).
extern "C" void fn_8001A6B0(void) {
    UnkLabSelection* selection = lbl_805FBBC8->mSelection;
    if (lbl_805FBBC8 != 0 && selection != 0) {
        fn_800F9790(selection->mC);
    }
    fn_80190E50(0xB7CE8A9D, fn_800F98F0());
    fn_801911C8(0x4B75FCD1, 0);
    f32 third = 0.33333334f;
    int percent = fn_800F98B8();
    char text[0x10];
    fn_80142EF8(text, "%d%%", percent);
    fn_80190E48(0xA03393A2, text);
    UnkActor* bar = fn_80198F18(fn_8018D1E4(lbl_805FB654), 0x43F7F50A);
    fn_801A403C(bar, 0.0f);
    fn_801A4050(bar, 100.0f);
    bar->vf30(0x80000);
    fn_801A3FA0(bar, percent);
    bar->vf18(0.0f);
    fn_801A4078(bar, third);

    if (fn_800F99B4() != 0) {
        fn_80190E50(0xA83D90D0, fn_800F99B4());
        fn_80191B68(0xA98E73C6, fn_800F9AE4());
        fn_8018F618(0x938AC70D, 1);
        fn_8018F618(0xCDC803FB, 0);
    } else if (fn_800F9A48() != 0) {
        fn_80190E50(0xA83D90D0, fn_800F9A48());
        fn_80191B68(0xA98E73C6, fn_800F9B08());
        fn_8018F618(0xCDC803FB, 1);
        fn_8018F618(0x938AC70D, 0);
    } else {
        fn_8018F618(0xA83D90D0, 0);
        fn_8018F618(0xA98E73C6, 0);
        fn_8018F618(0xB204BD0F, 0);
        fn_8018F618(0xCDC803FB, 0);
        fn_8018F618(0x938AC70D, 0);
    }

    fn_80190E50(0x1285C538, fn_800F9B44(0));
    fn_801911C8(0x5C9C9652, 0);
    fn_801911C8(0xC595C7E8, 0);
    fn_80190E50(0x8B8C9482, fn_800F9B44(1));
    fn_801911C8(0xB292F77E, 0);
    fn_801911C8(0x2CF662DD, 0);

    UnkSlotHashes slots = lbl_803476C8;
    u32 shown[2] = {0xE5049928, 0x7C0DC892};
    u32 hidden[2] = {0x5EE7FAD7, 0xC7EEAB6D};
    for (int i = 0; i < 2; i++) {
        if (fn_800F9EEC(i) != 0) {
            for (int j = 0; j < 5; j++) {
                if (fn_800FA174(i, j) != 0) {
                    fn_801903CC(slots.m[i][j], 0xE12D1897);
                } else {
                    fn_801903CC(slots.m[i][j], 0xB4E9B641);
                }
            }
            fn_8018F618(shown[i], 1);
            fn_8018F618(hidden[i], 0);
        } else {
            for (int j = 0; j < 5; j++) {
                fn_801903CC(slots.m[i][j], 0xB4E9B641);
            }
            fn_8018F618(shown[i], 0);
            fn_8018F618(hidden[i], 1);
        }
    }

    if (fn_800FA358() != 0) {
        fn_8018F618(0xF25835CB, 1);
        fn_8018F618(0x2D8AF405, 0);
        fn_8018F618(0x92825F01, 0);
        fn_801903CC(0x522D4954, 0x0FA0645F);
    } else if (fn_800FA3CC() != 0) {
        fn_8018F618(0x2D8AF405, 1);
        fn_8018F618(0xF25835CB, 0);
        fn_8018F618(0x92825F01, 0);
        fn_801903CC(0x522D4954, 0x01A85F3D);
    } else if (fn_800FA440() != 0) {
        fn_8018F618(0x92825F01, 1);
        fn_8018F618(0xF25835CB, 0);
        fn_8018F618(0x2D8AF405, 0);
        fn_801903CC(0x522D4954, 0xC197D336);
    } else {
        fn_8018F618(0x92825F01, 0);
        fn_8018F618(0xF25835CB, 0);
        fn_8018F618(0x2D8AF405, 0);
    }

    UnkLabStats stats = lbl_805FBBC8->mStats;
    fn_801911C8(0x252A1B41, stats.mC);
    fn_80190A60(0x77758B5F, 0);
    fn_80190A60(0x275D8DCA, 0);
    fn_80190A60(0x185208B1, 0);
    fn_80190A60(0x6D1292E3, 0);
    fn_80190A60(0x820A9463, 0);
    fn_80190A60(0x3F7E2AA9, 0);
    fn_80190A60(0x195A4D9E, 0);
    fn_80190A60(0x3119B8A8, 0);
    fn_8018F618(0xF9FBD51B, 1);
    fn_8018F618(0x475990EE, 1);
    fn_8018F618(0x0718ECD0, 1);

    if (fn_800FA350() != 0) {
        fn_80191B68(0x4EAF4810, fn_800FA350());
    } else {
        fn_8018F618(0x4EAF4810, 0);
    }
    if (selection != 0) {
        fn_8021947C(lbl_805FBBC8, selection->m0, selection->m11 - 1);
    }
    if (selection != 0 && fn_802191CC(lbl_805FBBC8, selection->m0, selection->m11 - 1) != 0) {
        fn_8018F618(0xB62E1096, 0);
    } else {
        fn_8018F618(0xB62E1096, 1);
    }
}

static const u32 lbl_805FC860[2] = {0x5C9C9652, 0xB292F77E};
static const u32 lbl_805FC868[2] = {0xC595C7E8, 0x2CF662DD};

// Scales the lab screen counters by the progress bar.
extern "C" void fn_8001AD5C(void) {
    int percent = fn_800F98B8();
    UnkActor* bar = fn_80198F18(fn_8018D1E4(lbl_805FB654), 0x43F7F50A);
    bar->vf2C(0x80000);
    fn_801A3FA0(bar, 0.0f);
    f32 scale = 1.0f - fn_801A4084(bar) / (percent / 100.0f);
    fn_801911C8(0x4B75FCD1, scale * fn_800F9B2C());
    u32 first[2] = {0x5C9C9652, 0xB292F77E};
    u32 second[2] = {0xC595C7E8, 0x2CF662DD};
    for (int i = 0; i < 2; i++) {
        fn_801911C8(first[i], scale * fn_800F9C7C(i));
        fn_801911C8(second[i], scale * fn_800F9E0C(i));
    }
}

extern "C" void fn_8001AF04(void) {
    fn_801911C8(0x4B75FCD1, fn_800F9B2C());
    for (int i = 0; i < 2; i++) {
        fn_801911C8(lbl_805FC860[i], fn_800F9C7C(i));
        fn_801911C8(lbl_805FC868[i], fn_800F9E0C(i));
    }
}
