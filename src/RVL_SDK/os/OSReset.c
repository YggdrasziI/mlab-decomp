#include <decomp.h>
#include <revolution/os.h>
#include <revolution/pad.h>
#include <revolution/vi/vi3in1.h>
#include <revolution/os/OSNandbootInfo.h>
#include <revolution/sc_fwd.h>
#include <mem.h>

typedef struct SCIdleModeInfo {
    u8 standby;
    u8 wc24;
} SCIdleModeInfo;

enum {
    OS_STATE_FLAGS_SHUTDOWN_NONE = 0,
    OS_STATE_FLAGS_SHUTDOWN_POWER_OFF,
    OS_STATE_FLAGS_SHUTDOWN_RESTART,
    OS_STATE_FLAGS_SHUTDOWN_RETURN_MENU,
    OS_STATE_FLAGS_SHUTDOWN_LAUNCH_TITLE,
    OS_STATE_FLAGS_SHUTDOWN_IDLE
};

#define DVD_COVER_CLOSED 2

#define SC_STATUS_BUSY 1

#define OS_APP_TYPE_IPL 0x40
#define OS_APP_TYPE_DVD 0x80
#define OS_APP_TYPE_CHANNEL 0x81

#define EXEC_WORK_ARENA_LO ((void*)0x81280000)
#define EXEC_WORK_ARENA_HI ((void*)0x812F0000)

OSThreadQueue __OSActiveThreadQueue AT_ADDRESS(OS_BASE_CACHED | 0x000000DC);

void __OSGetIOSRev(OSIOSRev* rev);

void __OSLaunchMenu(void);
void __OSRelaunchTitle(u32 resetCode);
void __OSReturnToMenul(u32 menuMode, const char* url, ...);
void __OSHotResetForError(void);
u32 __DVDGetCoverStatus(void);
void __DVDPrepareReset(void);

static u32 bootThisDol = 0;
volatile BOOL __OSIsReturnToIdle = FALSE;
static OSShutdownFunctionQueue ShutdownFunctionQueue;

#define ENQUEUE_INFO(info, queue)                       \
    {                                                   \
        OSShutdownFunctionInfo* __prev = (queue)->tail; \
        if (__prev == 0) {                              \
            (queue)->head = (info);                     \
        } else {                                        \
            __prev->next = (info);                      \
        }                                               \
        (info)->prev = __prev;                          \
        (info)->next = 0;                               \
        (queue)->tail = (info);                         \
    }

#define ENQUEUE_INFO_PRIO(info, queue)                                     \
    {                                                                      \
        OSShutdownFunctionInfo* __prev;                                    \
        OSShutdownFunctionInfo* __next;                                    \
        for (__next = (queue)->head;                                       \
             __next && (__next->priority <= (info)->priority);             \
             __next = __next->next)                                        \
            ;                                                              \
                                                                           \
        if (__next == 0) {                                                 \
            ENQUEUE_INFO(info, queue);                                     \
        } else {                                                           \
            (info)->next = __next;                                         \
            __prev = __next->prev;                                         \
            __next->prev = (info);                                         \
            (info)->prev = __prev;                                         \
            if (__prev == 0) {                                             \
                (queue)->head = (info);                                    \
            } else {                                                       \
                __prev->next = (info);                                     \
            }                                                              \
        }                                                                  \
    }

static void KillThreads(void) {
    OSThread* thread;
    OSThread* next;

    for (thread = __OSActiveThreadQueue.head; thread; thread = next) {
        next = thread->nextActive;

        switch (thread->state) {
        case 1:
        case 4:
            OSCancelThread(thread);
            break;
        default:
            break;
        }
    }
}

void OSRegisterShutdownFunction(OSShutdownFunctionInfo* info) {
    ENQUEUE_INFO_PRIO(info, &ShutdownFunctionQueue);
}

BOOL __OSCallShutdownFunctions(BOOL final, u32 event) {
    OSShutdownFunctionInfo* info;
    BOOL err;
    u32 priority = 0;

    err = FALSE;
    info = ShutdownFunctionQueue.head;
    while (info) {
        if (err && priority != info->priority) {
            break;
        }

        err |= !info->func(final, event);
        priority = info->priority;
        info = info->next;
    }

    err |= !__OSSyncSram();
    return err ? FALSE : TRUE;
}

// From zeldaret/tp libs/revolution/src/os/OSReset.c (CC0-1.0, commit c8fa8c9e).
void __OSShutdownDevices(u32 event) {
    int rc;
    BOOL disableRecalibration;
    BOOL doRecal;

    switch(event) {
    case 0:
    case 5:
    case 6:
        doRecal = FALSE;
        break;
    case 2:
    case 3:
    case 4:
    case 1:
    default:
        doRecal = TRUE;
        break;
    }

    __OSStopAudioSystem();

    if (!doRecal) {
        disableRecalibration = __PADDisableRecalibration(TRUE);
    }

    do {} while (!__OSCallShutdownFunctions(FALSE, event));
    do {} while (!__OSSyncSram());

    OSDisableInterrupts();

    rc = __OSCallShutdownFunctions(TRUE, event);
    ASSERTLINE(491, rc);

    LCDisable();
    if (!doRecal) {
        __PADDisableRecalibration(disableRecalibration);
    }

    KillThreads();
}

// From SMGCommunity/Petari src/RVL_SDK/os/OSReset.c (CC0-1.0, commit 6804bd31), the
// non-RMG __OSGetDiscState variant: OSRebootSystem, OSShutdownSystem, OSRestart and
// __OSReturnToMenu of this SDK build share the disc-state helper and end with
// __OSHotResetForError.
u8 __OSGetDiscState(u8 last) {
    u32 flags;

    if (__DVDGetCoverStatus() != DVD_COVER_CLOSED) {
        return 3;
    } else {
        if ((last == 1) && (__OSGetRTCFlags(&flags) && !flags)) {
            return 1;
        } else {
            return 2;
        }
    }
}

void OSRebootSystem(void) {
    OSStateFlags state;

    __OSStopPlayRecord();
    __OSUnRegisterStateEvent();
    __DVDPrepareReset();
    __OSReadStateFlags(&state);
    state.discState = __OSGetDiscState(state.discState);
    state.shutdownType = OS_STATE_FLAGS_SHUTDOWN_RESTART;
    __OSClearRTCFlags();
    __OSWriteStateFlags(&state);
    OSDisableScheduler();
    __OSShutdownDevices(1);
    __OSHotResetForError();
}

void OSShutdownSystem(void) {
    SCIdleModeInfo idleInfo;
    OSIOSRev iosRev;
    OSStateFlags state;

    memset(&idleInfo, 0, sizeof(idleInfo));

    SCInit();
    while (SCCheckStatus() == SC_STATUS_BUSY) {
    }

    SCGetIdleMode(&idleInfo);
    __OSStopPlayRecord();
    __OSUnRegisterStateEvent();
    __DVDPrepareReset();
    __OSReadStateFlags(&state);
    state.discState = __OSGetDiscState(state.discState);

    if (idleInfo.standby == TRUE) {
        state.shutdownType = OS_STATE_FLAGS_SHUTDOWN_IDLE;
    } else {
        state.shutdownType = OS_STATE_FLAGS_SHUTDOWN_POWER_OFF;
    }

    __OSClearRTCFlags();
    __OSWriteStateFlags(&state);
    __OSGetIOSRev(&iosRev);

    if (idleInfo.standby == TRUE) {
        __OSIsReturnToIdle = TRUE;

        OSDisableScheduler();
        __OSShutdownDevices(5);
        OSEnableScheduler();
        __OSLaunchMenu();
    } else {
        OSDisableScheduler();
        __OSShutdownDevices(2);
        __OSShutdownToSBY();
    }
}

void OSRestart(u32 resetCode) {
    u8 type = OSGetAppType();
    __OSStopPlayRecord();
    __OSUnRegisterStateEvent();

    if (type == OS_APP_TYPE_CHANNEL) {
        OSDisableScheduler();
        __OSShutdownDevices(4);
        OSEnableScheduler();
        __OSRelaunchTitle(resetCode);
    } else if (type == OS_APP_TYPE_DVD) {
        OSDisableScheduler();
        __OSShutdownDevices(4);
        OSEnableScheduler();
        __OSReboot(resetCode, bootThisDol);
    }

    OSDisableScheduler();
    __OSShutdownDevices(1);
    __OSHotResetForError();
}

void __OSReturnToMenu(u8 menuMode) {
    OSStateFlags state;

    __OSStopPlayRecord();
    __OSUnRegisterStateEvent();
    __DVDPrepareReset();
    __OSReadStateFlags(&state);
    state.discState = __OSGetDiscState(state.discState);
    state.shutdownType = OS_STATE_FLAGS_SHUTDOWN_RETURN_MENU;
    state.menuMode = menuMode;
    __OSClearRTCFlags();
    __OSWriteStateFlags(&state);
    OSDisableScheduler();
    __OSShutdownDevices(5);
    OSEnableScheduler();
    __OSLaunchMenu();
    OSDisableScheduler();
    __VISetRGBModeImm();
    __OSHotResetForError();
}

// From SMGCommunity/Petari src/RVL_SDK/os/OSReset.c (CC0-1.0, commit 6804bd31).
void OSReturnToMenu(void) {
    __OSReturnToMenu(0);
    OSPanic(__FILE__, 0x348, "OSReturnToMenu(): Falied to boot system menu.\n");
}

void OSReturnToDataManager(void) {
    __OSReturnToMenu(1);
    OSPanic(__FILE__, 858, "OSReturnToDataManager(): Falied to boot system menu.\n");
}

void OSReturnToSetting(u8 setting) {
    char* url;

    switch (setting) {
    case 1:
        url = "Calendar/Calendar_index.html";
        break;
    case 2:
        url = "Display/Display_index.html";
        break;
    case 3:
        url = "Sound/Sound_index.html";
        break;
    case 4:
        url = "Parental_Control/Parental_Control_index.html";
        break;
    case 5:
        url = "Internet/Internet_index.html";
        break;
    case 6:
        url = "WiiConnect24/Wiiconnect24_index.html";
        break;
    case 7:
        url = "Update/Update_index.html";
        break;
    default:
        OSReport("OSReturnToSetting(): You can't specify %d.  \n", setting);
        OSPanic(__FILE__, 0x394, "");
    }

    __OSReturnToMenul(1, url, NULL);
}

void __OSReturnToMenuForError(void) {
    OSStateFlags state;

    __OSReadStateFlags(&state);
    state.discState = 2;
    state.shutdownType = OS_STATE_FLAGS_SHUTDOWN_RETURN_MENU;
    __OSClearRTCFlags();
    __OSWriteStateFlags(&state);
    __OSLaunchMenu();
    OSDisableScheduler();
    __VISetRGBModeImm();
    __OSHotResetForError();
    OSPanic(__FILE__, 0x3BB, "__OSReturnToMenu(): Falied to boot system menu.\n");
}

void __OSHotResetForError(void) {
    if (__OSInNandBoot || __OSInReboot) {
        __OSInitSTM();
    }

    __OSHotReset();

    OSPanic(__FILE__, 0x3D3, "__OSHotReset(): Falied to reset system.\n");
}

u32 OSGetResetCode(void) {
    u32 code;

    if (__OSRebootParams.valid) {
        code = 0x80000000 | __OSRebootParams.restartCode;
    } else {
        code = (PI_HW_REGS[9] & 0xFFFFFFF8) >> 3;
    }

    return code;
}

// From SMGCommunity/Petari src/RVL_SDK/os/OSReset.c (CC0-1.0, commit 6804bd31):
// this SDK build reports a different line number.
void OSResetSystem(int, u32, int) {
    OSPanic(__FILE__, 1130, "OSResetSystem() is obsoleted. It doesn't work any longer.\n");
}

// From SMGCommunity/Petari src/RVL_SDK/os/OSReset.c (CC0-1.0, commit 6804bd31). Its
// code is dead-stripped from this DOL, only its string remains in .data.
u32 OSSetBootDol(u32 dolOffset) {
    OSPanic(__FILE__, __LINE__, "OSSetBootDol() is obsoleted. It doesn't work any longer.\n");
    return 0;
}
