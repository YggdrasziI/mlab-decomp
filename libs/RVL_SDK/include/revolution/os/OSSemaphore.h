#ifndef REVOLUTION_OS_SEMAPHORE_H
#define REVOLUTION_OS_SEMAPHORE_H

#include <revolution/os/OSThread.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSSemaphore {
    volatile s32 count; // vs32 in rb3's OSSemaphore.h
    OSThreadQueue queue;
} OSSemaphore;

void OSInitSemaphore(OSSemaphore* sem, s32 count);
s32 OSWaitSemaphore(OSSemaphore* sem);
s32 OSSignalSemaphore(OSSemaphore* sem);

#ifdef __cplusplus
}
#endif

#endif  // REVOLUTION_OS_SEMAPHORE_H
