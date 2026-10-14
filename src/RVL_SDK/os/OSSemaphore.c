/* From TP (libs/dolphin/src/os/OSSemaphore.c), with our headers; OSWaitSemaphore and
 * OSSignalSemaphore have rb3's forms (src/sdk/RVL_SDK/src/os/OSSemaphore.c) */
#include <revolution/os.h>
#include <revolution/os/OSSemaphore.h>

void OSInitSemaphore(OSSemaphore* sem, s32 count) {
    BOOL enabled = OSDisableInterrupts();

    OSInitThreadQueue(&sem->queue);
    sem->count = count;
    OSRestoreInterrupts(enabled);
}

s32 OSWaitSemaphore(OSSemaphore* sem) {
    BOOL enabled;
    s32 count;

    enabled = OSDisableInterrupts();

    while ((count = sem->count) <= 0) {
        OSSleepThread(&sem->queue);
    }

    sem->count--;
    OSRestoreInterrupts(enabled);
    return count;
}

s32 OSTryWaitSemaphore(OSSemaphore* sem) {
    BOOL enabled;
    s32 count;

    enabled = OSDisableInterrupts();
    count = sem->count;
    if (sem->count > 0) {
        sem->count = sem->count - 1;
    }

    OSRestoreInterrupts(enabled);
    return count;
}

s32 OSSignalSemaphore(OSSemaphore* sem) {
    BOOL enabled;
    s32 count;

    enabled = OSDisableInterrupts();
    count = sem->count++;

    OSWakeupThread(&sem->queue);
    OSRestoreInterrupts(enabled);
    return count;
}

s32 OSGetSemaphoreCount(OSSemaphore* sem) {
    return sem->count;
}
