#include <decomp.h>
#include <revolution/nwc24/NWC24Schedule.h>
#include <revolution/nwc24/internal/NWC24iSystem.h>
#include <revolution/os.h>

// RTC synchronization still uses the legacy unprototyped declaration.
int NWC24iSynchronizeRtcCounter();

void __OSInitNet(void) {
    s32 error;
    OSIOSRev rev;

    __OSGetIOSRev(&rev);
    if (rev.idLo <= 4 || rev.idLo == 9) {
        return;
    }

    error = NWC24iPrepareShutdown();
    if (error != 0) {
        if (error < 0) {
            OSReport("Failed to register network shutdown function. %d\n",
                     error);
        }

        error = NWC24SuspendScheduler();
        if (error < 0) {
            OSReport("Failed to suspend the WiiConnect24 scheduler. %d\n",
                     error);
        }
    }

    // This SDK build skips the RTC synchronization in the IPL and passes
    // FALSE, as in zeldaret/tp, Petari and DarkRTA/rb3.
    if (!__OSInIPL) {
        error = NWC24iSynchronizeRtcCounter(FALSE);
        if (error != 0) {
            OSReport(
                "Failed to synchronize time with network resource managers. %d\n",
                error);
        }
    }
}

// clang-format off
DECOMP_FORCEACTIVE(OSNet_c,
                   "NWC24iPrepareShutdown",
                   "/dev/net/kd/request",
                   "NWC24SuspendScheduler",
                   "NWC24ResumeScheduler",
                   "NWC24iRequestShutdown",
                   "NWC24Shutdown_: Give up!\n",
                   "NWC24iSetRtcCounter_",
                   "/dev/net/kd/time");
// clang-format on
