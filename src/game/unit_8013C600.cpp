// Game unit at 0x8013C600-0x8013CA74 (real file name unknown): a team summary screen.
// Fills the widgets of the screen 0xFEE5B157 once with the values of the team screen
// (0x8013E3F0-0x8013E44C), keeps a blinking cursor actor at its rest position and
// shows a "0:%0.02d" timer on the screen 0x43538242. Bounds come from the static
// initializers of this unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/externs.h"
#include "game/unk_actor.h"
#include "game/unk_vec3.h"

extern "C" {
extern void* lbl_805FB654;
// Timer and its flag, defined in another unit (real names unknown).
extern f32 lbl_805FB410;
extern u8 lbl_805FB414;

void* fn_8018D1E4(void* obj);
void* fn_80198E34(void* screens, u32 hash);
void* fn_80199AEC(void* screen, u32 hash);
void fn_8019EFC8(void* widget, const char* text, int arg);
void fn_801A3FA0(void* widget, f32 value);
void fn_801A3010(void* actor, void* arg);
u32 fn_80140B58(void);
void fn_80140B50(u32 hash);
int fn_801409FC(void);
f32 fn_8013E3FC(void);
f32 fn_8013E404(void);
f32 fn_8013E40C(void);
f32 fn_8013E414(void);
const char* fn_8013E3F0(void);
void* fn_8013E44C(void);
u32 fn_8013E444(void);
void fn_8013F2A4(int arg);
void* fn_801B7198(void* obj, int arg);
int fn_8004E300(void* obj, int arg);
void fn_8018F618(u32 hash, int arg);
void fn_801F5724(int arg);
}

static bool sFilled;
static s32 sBlink;

// Rest position of the cursor, taken from the cursor actor on first use.
static f32 sCursorX = -1.0f;
static f32 sCursorY = -1.0f;
static f32 sCursorZ = -1.0f;

// Constant (0, 0, -0.2) in .rodata among other 12-byte vectors used by units all over
// the game, so probably a global defined elsewhere (real name unknown).
extern const UnkVec3 lbl_80346014;

extern "C" {

void fn_8013C600(void) {
    if (fn_80140B58() == 0x7B89ED91) {
        return;
    }
    sFilled = false;
    sBlink = 0;
    void* screens = fn_8018D1E4(lbl_805FB654);
    if (screens != 0) {
        void* screen = fn_80198E34(screens, 0xFEE5B157);
        if (screen != 0) {
            UnkActor* cursor = (UnkActor*)fn_80199AEC(screen, 0x60B15D91);
            if (cursor != 0) {
                if (-1.0f == sCursorX && -1.0f == sCursorY && -1.0f == sCursorZ) {
                    sCursorX = cursor->mPos.x;
                    sCursorY = cursor->mPos.y;
                    sCursorZ = cursor->mPos.z;
                }
            }
        }
    }
    fn_80140B50(0x7B89ED91);
}

void fn_8013C6BC(void) {
    fn_8018F618(0x60B15D91, 0);
}

void fn_8013C6CC(void) {
    if (fn_801409FC() == 0) {
        return;
    }
    void* screens = fn_8018D1E4(lbl_805FB654);
    if (screens == 0) {
        return;
    }
    void* screen = fn_80198E34(screens, 0xFEE5B157);
    if (screen != 0) {
        if (!sFilled) {
            sFilled = true;
            void* value0 = fn_80199AEC(screen, 0x3E22E495);
            if (value0 != 0) {
                fn_801A3FA0(value0, fn_8013E3FC());
            }
            void* value1 = fn_80199AEC(screen, 0x67E9BE17);
            if (value1 != 0) {
                fn_801A3FA0(value1, fn_8013E404());
            }
            void* value2 = fn_80199AEC(screen, 0xB738EABD);
            if (value2 != 0) {
                fn_801A3FA0(value2, fn_8013E40C());
            }
            void* value3 = fn_80199AEC(screen, 0xE4E00B6F);
            if (value3 != 0) {
                fn_801A3FA0(value3, fn_8013E414());
            }
            void* name = fn_80199AEC(screen, 0x2EADCD52);
            if (name != 0) {
                fn_8019EFC8(name, fn_8013E3F0(), 1);
            }
        }
        UnkActor* cursor = (UnkActor*)fn_80199AEC(screen, 0x60B15D91);
        if (cursor != 0) {
            // The DOL tests one compare three times: the original probably checks
            // the x coordinate three times.
            if (-1.0f == sCursorX && -1.0f == sCursorX && -1.0f == sCursorX) {
                sCursorX = cursor->mPos.x;
                sCursorY = cursor->mPos.y;
                sCursorZ = cursor->mPos.z;
            }
            fn_801A3010(cursor, fn_8013E44C());
            UnkVec3 pos(sCursorX, sCursorY, sCursorZ);
            void* target = fn_801B7198((u8*)cursor + 0x140, 0);
            if (target != 0 && fn_8004E300(target, 1) == 13) {
                pos += UnkVec3(lbl_80346014.x, lbl_80346014.y, lbl_80346014.z);
            }
            cursor->mPos = UnkVec3(pos);
            cursor->vf2C(0x18);
            if (!(cursor->m64 & 2) && sBlink == 0) {
                sBlink = 3;
            }
            if (sBlink > 0 && --sBlink == 0) {
                fn_8018F618(0x379F79BD, 0);
                cursor->vf2C(2);
            }
        }
    }
    if (fn_8013E444() == 1) {
        fn_8013F2A4(1);
    } else {
        fn_8013F2A4(0);
    }
    if (lbl_805FB414 == 1) {
        void* timer = fn_80198E34(screens, 0x43538242);
        if (timer != 0 && fn_80199AEC(timer, 0x9D841CAA) != 0) {
            void* text = fn_80199AEC(timer, 0x29DD4A4E);
            if (text != 0) {
                if (0.0f == lbl_805FB410) {
                    fn_801F5724(0x2000);
                }
                char buf[16];
                fn_80142EF8(buf, "0:%0.02d", (int)lbl_805FB410);
                fn_8019EFC8(text, buf, 1);
            }
        }
    }
}
}
