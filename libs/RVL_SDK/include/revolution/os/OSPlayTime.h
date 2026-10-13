#ifndef RVL_SDK_OS_PLAY_TIME_H
#define RVL_SDK_OS_PLAY_TIME_H
#include <revolution/types.h>

#include <revolution/esp/esp.h>
#ifdef __cplusplus
extern "C" {
#endif

// From Petari's revolution/os/OSPlayTime.h (CC0).
typedef enum {
    OSPLAYTIME_PERMANENT = 0,
    OSPLAYTIME_LIMITED = 1,
    OSPLAYTIME_LIMITED_LAUNCH = 4,
    OSPLAYTIME_OTHER = 9
} __OSPlayTimeType;

void __OSInitPlayTime(void);
BOOL OSPlayTimeIsLimited(void);
s32 __OSGetPlayTime(ESTicketView* ticket, __OSPlayTimeType* type, u32* playTime);
BOOL __OSWriteExpiredFlag(void);

#ifdef __cplusplus
}
#endif
#endif
