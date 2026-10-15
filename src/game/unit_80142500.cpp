// Game unit at 0x80142500-0x80142E94 (real file name unknown): main() and the platform
// layer around it (timer, sleep, frame callbacks, allocators, system initialization,
// fatal error screen, shutdown). Bounds come from the static initializers of the
// previous unit (0x801424B8) and of this one (0x80142E90).

#include <revolution/kpad.h>
#include <revolution/mem.h>
#include <revolution/os.h>
#include <revolution/sc.h>
#include <revolution/dvd.h>
#include <revolution/ai.h>
#include <revolution/ax.h>
#include <revolution/wpad/WPAD.h>
#include <string.h>

#include "types.h"

extern "C" {
OSStateCallback OSSetResetCallback(OSStateCallback callback);

// Real names unknown.
void fn_8033AE04(long* out);
void fn_8026077C(MEMAllocator* allocator, OSHeapHandle heap);
void fn_80264EE8(void);
void fn_80264F70(void);
int fn_80142EF8(char* buf, const char* format, ...);
int fn_801FAED0(int argc, char** argv);
}

// Text shown first on the fatal error screen (real name unknown).
extern char lbl_803C5358[];

// Timer started at construction (real name unknown).
class UnkTimer {
public:
    UnkTimer();
    f32 GetSeconds();
    s64 GetMicroseconds();
    f32 GetSecondsAndRestart();
    s64 GetMicrosecondsAndRestart();

    /* 0x0 */ OSTime mStart;
};

typedef void (*UnkFrameCallback)(void);
typedef void (*UnkShutdownFunc)(int mode);

extern "C" {
void fn_80142A0C(int mode);
}

static UnkFrameCallback sCallbacks[16];
// Used by other units too (real names unknown).
MEMAllocator lbl_803F2870;
MEMAllocator lbl_803F2880;
static int sCallbackCount;
static int sShutdownMode;
static BOOL sInitialized;

static u8 sShutdownEnabled = TRUE;
UnkShutdownFunc lbl_805F90F4 = fn_80142A0C;

// The DOL has "isiexti" as one entry and a null last entry: the comma between "isi"
// and "exti" is missing in the original source.
static const char* sErrorNames[17] = {
    "reset", "machine", "dsi", "isi" "exti", "align", "program", "nofp", "dec",
    "sys", "trace", "perf", "brk", "sysi", "thermi", "protect", "fpe",
};

static inline void UnkSetGQR6(register u32 type, register u32 scale) {
    register u32 value = ((scale << 8 | type) << 16) | (scale << 8 | type);
    asm { mtspr 0x396, value }
}

UnkTimer::UnkTimer() {
    mStart = OSGetTime();
}

f32 UnkTimer::GetSeconds() {
    OSTime now = OSGetTime();
    if ((u64)(mStart - now) < 60000000) {
        mStart = now;
    }
    return (now - mStart) / (f64)OS_TIME_SPEED;
}

s64 UnkTimer::GetMicroseconds() {
    OSTime now = OSGetTime();
    if ((u64)(mStart - now) < 60000000) {
        mStart = now;
    }
    return OSTicksToMicroseconds(now - mStart);
}

f32 UnkTimer::GetSecondsAndRestart() {
    OSTime now = OSGetTime();
    if ((u64)(mStart - now) < 60000000) {
        mStart = now;
    }
    f32 seconds = (now - mStart) / (f64)OS_TIME_SPEED;
    mStart = now;
    return seconds;
}

s64 UnkTimer::GetMicrosecondsAndRestart() {
    OSTime now = OSGetTime();
    if ((u64)(mStart - now) < 60000000) {
        mStart = now;
    }
    OSTime elapsed = now - mStart;
    mStart = now;
    return OSTicksToMicroseconds(elapsed);
}

extern "C" {

long fn_801427F0(void) {
    long t;
    fn_8033AE04(&t);
    return t;
}

void fn_80142818(s64 usec) {
    OSSleepTicks(OSMicrosecondsToTicks(usec));
}

void fn_80142870(u8 enabled) {
    sShutdownEnabled = enabled;
}

void fn_80142878(UnkFrameCallback callback) {
    sCallbacks[sCallbackCount++] = callback;
}

void fn_80142898(UnkFrameCallback callback) {
    int i;
    for (i = 0; i < sCallbackCount; i++) {
        if (sCallbacks[i] == callback) {
            break;
        }
    }
    if (i < sCallbackCount) {
        sCallbackCount--;
        for (; i < sCallbackCount; i++) {
            sCallbacks[i] = sCallbacks[i + 1];
        }
    }
}

void fn_80142970(void) {
    for (int i = 0; i < sCallbackCount; i++) {
        sCallbacks[i]();
    }
}

void* fn_801429CC(u32 size) {
    return MEMAllocFromAllocator(&lbl_803F2880, size);
}

BOOL fn_801429DC(void* ptr) {
    MEMFreeToAllocator(&lbl_803F2880, ptr);
    return TRUE;
}

void fn_80142A0C(int mode) {
    sShutdownMode = mode;
}

void fn_80142A14(void) {
    if (lbl_805F90F4) {
        lbl_805F90F4(1);
    }
}

void fn_80142A30(void) {
    if (lbl_805F90F4) {
        lbl_805F90F4(3);
    }
}

void fn_80142A4C(u16 error, OSContext* context, u32 dsisr, u32 dar) {
    char buf[0x1000];
    char* p;

    OSDumpContext(context);
    GXColor bg = {0, 0, 255, 0};
    GXColor fg = {255, 255, 255, 0};
    p = buf;
    fn_80142EF8(p, "%s", lbl_803C5358);
    p += strlen(p);
    fn_80142EF8(p, "%d (%s) %08x %08x\n", error, error < 17 ? sErrorNames[error] : "?", dsisr, dar);
    p += strlen(p);
    u32 i = 0;
    for (u32* sp = (u32*)OSGetStackPointer(); sp != NULL && (u32)sp != 0xFFFFFFFF && i++ < 16;
         sp = (u32*)*sp) {
        fn_80142EF8(p, "0x%08x: 0x%08x 0x%08x\n", sp, sp[0], sp[1]);
        p += strlen(p);
    }
    OSFatal(fg, bg, buf);
}

void fn_80142BBC(void) {
    if (sInitialized) {
        return;
    }
    sInitialized = TRUE;

    OSInit();
    SCInit();
    while (SCCheckStatus() != SC_STATUS_OK) {
    }
    DVDInit();
    OSInitFastCast();
    UnkSetGQR6(OS_GQR_TYPE_U8, 8);

    OSHeapHandle heap;
    void* arenaLo = OSGetMEM1ArenaLo();
    void* arenaHi = OSGetMEM1ArenaHi();
    arenaLo = (void*)OSRoundUp32B(arenaLo);
    arenaHi = (void*)OSRoundDown32B(arenaHi);
    void* heapStart = OSInitAlloc(arenaLo, arenaHi, 1);
    OSSetMEM1ArenaLo(heapStart);
    OSSetMEM1ArenaHi(arenaHi);
    heap = OSCreateHeap(heapStart, arenaHi);
    OSSetCurrentHeap(heap);
    fn_8026077C(&lbl_803F2870, heap);

    void* mem2Lo = OSGetMEM2ArenaLo();
    void* mem2Hi = OSGetMEM2ArenaHi();
    MEMHeapHandle frmHeap = MEMCreateFrmHeapEx(mem2Lo, (u32)mem2Hi - (u32)mem2Lo, 0);
    OSSetMEM2ArenaLo(mem2Hi);
    MEMInitAllocatorForFrmHeap(&lbl_803F2880, frmHeap, 32);
    WPADRegisterAllocator(fn_801429CC, fn_801429DC);
    KPADInit();
    AIInit(NULL);
    AXInit();
    fn_80264EE8();
    fn_80264F70();

    OSSetResetCallback(fn_80142A14);
    OSSetPowerCallback(fn_80142A30);
    OSErrorHandler handler = (OSErrorHandler)fn_80142A4C;
    OSSetErrorHandler(0, handler);
    OSSetErrorHandler(1, handler);
    OSSetErrorHandler(2, handler);
    OSSetErrorHandler(3, handler);
    OSSetErrorHandler(5, handler);
    OSSetErrorHandler(11, handler);
    OSSetErrorHandler(13, handler);
    OSSetErrorHandler(14, handler);
    OSSetErrorHandler(15, handler);
    OSSetErrorHandler(6, handler);
    OSSetErrorHandler(10, handler);
    OSSetErrorHandler(12, handler);
}

static inline int UnkGetShutdownMode(void) {
    return sShutdownEnabled ? sShutdownMode : 0;
}

int fn_80142DC4(void) {
    return UnkGetShutdownMode();
}

int fn_80142DE0(void) {
    return sShutdownMode;
}

void fn_80142DE8(int mode) {
    sShutdownMode = mode;
}

int main(int argc, char** argv) {
    fn_80142BBC();
    int result = fn_801FAED0(argc, argv);
    switch (UnkGetShutdownMode()) {
    case 1:
        OSRestart(0);
        break;
    case 2:
        OSReturnToMenu();
        break;
    case 3:
        OSShutdownSystem();
        break;
    }
    return result;
}

} // extern "C"

// Runs the system initialization before the other static initializers that follow
// in link order (real name unknown).
static struct UnkInitializer {
    UnkInitializer() { fn_80142BBC(); }
} sInitializer;
